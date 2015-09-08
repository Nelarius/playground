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
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_syswm.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_SCISSOR_TEST);
        glActiveTexture(GL_TEXTURE0);
        
        GLint oldTexture;
        glGetIntegerv( GL_TEXTURE_BINDING_2D, &oldTexture );
        
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
        glBindTexture( GL_TEXTURE_2D, oldTexture );
        glDisable( GL_SCISSOR_TEST );
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
