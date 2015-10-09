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

// link to where I got some of the code from:
// https://github.com/ocornut/imgui/blob/master/examples/opengl3_example/imgui_impl_glfw_gl3.cpp#L31

namespace {
    // unfortunate global state
    pg::opengl::Program*            gShader{ nullptr };
    pg::opengl::BufferObject*       gVbo{ nullptr };
    pg::opengl::VertexArrayObject*  gVao{ nullptr };
    pg::opengl::Texture*            gFont{ nullptr };
    
    
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

void ImGuiRenderer::render_() {
    //
}

void ImGuiRenderer::initialize_() {
    //
}

void ImGuiRenderer::createDeviceObjects_() {
    //
}

void ImGuiRenderer::newFrame_( float dt ) {
    //
}

}   // system
}   // pg
