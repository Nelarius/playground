#include "system/ImGuiRenderer.h"
#include "manager/ShaderManager.h"
#include "opengl/VertexArrayObject.h"
#include "opengl/VertexArrayObjectFactory.h"
#include "math/Matrix.h"
#include "utils/Log.h"
#include <GL/glew.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_clipboard.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_syswm.h>

// link to where I got some of the code from:
// https://github.com/ocornut/imgui/blob/master/examples/opengl3_example/imgui_impl_glfw_gl3.cpp#L31

namespace {
    // unfortunate global state
    pg::opengl::Program*            gShader{ nullptr };
    pg::opengl::BufferObject*       gVbo{ nullptr };
    pg::opengl::VertexArrayObject*  gVao{ nullptr };
    pg::opengl::Texture*            gFont{ nullptr };
    
    void RenderDrawLists( ImDrawData* drawData ) {
        // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled
        GLint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
        GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
        GLint last_blend_src; glGetIntegerv(GL_BLEND_SRC, &last_blend_src);
        GLint last_blend_dst; glGetIntegerv(GL_BLEND_DST, &last_blend_dst);
        GLint last_blend_equation_rgb; glGetIntegerv(GL_BLEND_EQUATION_RGB, &last_blend_equation_rgb);
        GLint last_blend_equation_alpha; glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &last_blend_equation_alpha);
        
        GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
        GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
        GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
        GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);
    
        // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_SCISSOR_TEST);
        glActiveTexture(GL_TEXTURE0);
        
        ImGuiIO& io = ImGui::GetIO();
        
        float fbHeight = io.DisplaySize.y * io.DisplayFramebufferScale.y;
        drawData->ScaleClipRects( io.DisplayFramebufferScale );
        
        const float orthoProjection[4][4] = { 
            { 2.0f/io.DisplaySize.x,  0.0f,                   0.0f, 0.0f },
            { 0.0f,                   -2.0f/io.DisplaySize.y, 0.0f, 0.0f },
            { 0.0f,                   0.0f,                   -1.0f, 0.0f },
            { -1.0f,                  1.0f,                   0.0f, 1.0f }
        };
        gShader->use();
        glUniformMatrix4fv( gShader->uniform( "MOrtho" ), 1, GL_FALSE, &orthoProjection[0][0] );
        for ( int n = 0; n < drawData->CmdListsCount; n++ ) {
            const ImDrawList* commandList = drawData->CmdLists[n];
            const ImDrawIdx* idxBuffer = &commandList->IdxBuffer.front();
            
            // set vertex data
            gVbo->dataStore(
                ( GLsizeiptr ) commandList->VtxBuffer.size(), 
                sizeof( ImDrawVert ), 
                ( GLvoid* ) &commandList->VtxBuffer.front(),
                GL_STREAM_DRAW
            );
            
            gVao->bind();
            
            for ( int cmdIndex = 0; cmdIndex < commandList->CmdBuffer.size(); cmdIndex++ ) {
                const ImDrawCmd* pcmd = &commandList->CmdBuffer[ cmdIndex ];
                if ( pcmd->UserCallback ) {
                    pcmd->UserCallback( commandList, pcmd );
                } else {
                    glBindTexture( GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId );
                    glScissor(
                        (int)  pcmd->ClipRect.x, 
                        (int)( fbHeight - pcmd->ClipRect.w ),
                        (int)( pcmd->ClipRect.z - pcmd->ClipRect.x ), 
                        (int)( pcmd->ClipRect.w - pcmd->ClipRect.y )
                    );
                    glDrawElements( 
                        GL_TRIANGLES, 
                        ( GLsizei ) pcmd->ElemCount,
                        GL_UNSIGNED_SHORT,
                        idxBuffer
                    );
                }
                idxBuffer += pcmd->ElemCount;
            }
        }
        gVao->unbind();
        gShader->stopUsing();
        glUseProgram(last_program);
        glBindTexture(GL_TEXTURE_2D, last_texture);

        glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
        glBlendFunc(last_blend_src, last_blend_dst);
        if (last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
        if (last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
        if (last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
        if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
    }
    
    void SetClipboardText( const char* text ) {
        SDL_SetClipboardText( text );
    }
    
    const char* GetClipboardText() {
        return SDL_GetClipboardText();
    }
}

namespace pg {
namespace system {

uint32_t ImGuiRenderer::refCount_ = 0u;

ImGuiRenderer::ImGuiRenderer( Context& context )
:   context_( context ) {
    initialize_();
    refCount_++;
}

ImGuiRenderer::~ImGuiRenderer() {
    if ( --refCount_ == 0u ) {
        ImGui::Shutdown();
        LOG_DEBUG2 << "Destructing global ImGuiRenderer resources";
        delete gVbo;
        delete gVao;
        delete gFont;
    }
    LOG_DEBUG2 << "Done destructing ImGuiRenderer resources.";
}

void ImGuiRenderer::mouseButtonPressed( int button ) {
    int i;
    switch ( button ) {
        case SDL_BUTTON_LEFT: i = 0; break;
        case SDL_BUTTON_MIDDLE: i = 1; break;
        case SDL_BUTTON_RIGHT: i = 2; break;
        default: return;
    }
    ImGuiIO& io = ImGui::GetIO();
    io.MouseDown[i] = true;
}

void ImGuiRenderer::mouseButtonReleased( int button ) {
    int i;
    switch( button ) {
        case SDL_BUTTON_LEFT: i = 0; break;
        case SDL_BUTTON_MIDDLE: i = 1; break;
        case SDL_BUTTON_RIGHT: i = 2; break;
        default: return;
    }
    ImGuiIO& io = ImGui::GetIO();
    io.MouseDown[i] = false;
}

void ImGuiRenderer::initialize_() {
    
    ImGuiIO& io = ImGui::GetIO();
    io.KeyMap[ImGuiKey_Tab] = SDLK_TAB; // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
    io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
    io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
    io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
    io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
    io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
    io.KeyMap[ImGuiKey_Delete] = SDLK_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = SDLK_BACKSPACE;
    io.KeyMap[ImGuiKey_Enter] = SDLK_RETURN;
    io.KeyMap[ImGuiKey_Escape] = SDLK_ESCAPE;
    io.KeyMap[ImGuiKey_A] = SDLK_a;
    io.KeyMap[ImGuiKey_C] = SDLK_c;
    io.KeyMap[ImGuiKey_V] = SDLK_v;
    io.KeyMap[ImGuiKey_X] = SDLK_x;
    io.KeyMap[ImGuiKey_Y] = SDLK_y;
    io.KeyMap[ImGuiKey_Z] = SDLK_z;
    
    io.RenderDrawListsFn = RenderDrawLists;
    io.GetClipboardTextFn = GetClipboardText;
    io.SetClipboardTextFn = SetClipboardText;
    
    #ifdef _WIN32
        SDL_Window* window = context_.window->SDLwindow();
        SDL_SysWMinfo wmInfo;
        SDL_VERSION( &wmInfo.version );
        SDL_GetWindowWMInfo( window, &wmInfo );
        io.ImeWindowHandle = wmInfo.info.win.window;
    #endif
    
    createDeviceObjects_();
    // handle callbacks
}

void ImGuiRenderer::createDeviceObjects_() {
    if ( !gShader ) {
        gShader = context_.shaderManager.get( "panel" );
    }
    if ( !gVbo ) {
        gVbo = new opengl::BufferObject( GL_ARRAY_BUFFER );
    }
    if ( ! gVao ) {
        auto factory = opengl::VertexArrayObjectFactory{ gVbo, gShader };
        // 2 * 4 bytes
        factory.addAttribute( "position", 2, GL_FLOAT, GL_FALSE, 20, 0 ); 
        // another 2 * 4 bytes
        factory.addAttribute( "uv", 2, GL_FLOAT, GL_FALSE, sizeof( ImDrawVert ), 8 ); 
        // 4 * 1 byte
        factory.addAttribute( "color", 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof( ImDrawVert ), 16 );
        // total size of ImDrawVert is 20 bytes ( I checked )
        gVao = new opengl::VertexArrayObject( 0 );
        *gVao = factory.getVao();
    }
    if ( !gFont ) {
        gFont = new opengl::Texture( GL_TEXTURE_2D );
        unsigned char* pixels;
        int width, height;
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->GetTexDataAsRGBA32( &pixels, &width, &height );
        gFont->bind();
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
        gFont->unbind();
        io.Fonts->TexID = (void*)(intptr_t)gFont->object();
        // cleanup
        // don't clear the input if you want to append new fonts later
        io.Fonts->ClearInputData();
        io.Fonts->ClearTexData();
    }
}

void ImGuiRenderer::newFrame_( float dt ) {
    SDL_Window* window = context_.window->SDLwindow();

    ImGuiIO& io = ImGui::GetIO();
    
    io.MousePos = ImVec2( float( context_.mouse().x ), float( context_.mouse().y ) );
    
    io.DeltaTime = dt;

    // Setup display size (every frame to accommodate for window resizing)
    int w, h;
    SDL_GetWindowSize( window, &w, &h );
    io.DisplaySize = ImVec2( float( w ), float( h ) );
    // how do I get this in SDL?
    io.DisplayFramebufferScale = ImVec2( 1.0f, 1.0f );
    
    // Hide OS mouse cursor if ImGui is drawing it
    SDL_ShowCursor(io.MouseDrawCursor ? 0 : 1);
    
    ImGui::NewFrame();
}

}   // system
}   // pg
