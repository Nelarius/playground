#include "system/Ui.h"
#include "GL/glew.h"
#include "3rdparty/imgui.h"
#include <GL/glew.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_clipboard.h>
#include <SDL2/SDL_timer.h>
#include <string>

namespace {
    bool show_another_window{ false };
    float f{ 0.0f };
}

namespace pg {
namespace system {

Ui::Ui( Context& context )
:   System<Ui>(),
    ImGuiRenderer( context ),
    display_( false )
    {}

void Ui::update( ecs::EntityManager&, ecs::EventManager&, float dt ) {
    newFrame_( dt );
    
    if ( display_ ) {
        ui_();
    }
    
    ImGui::Render();
}

void Ui::toggleDisplay() {
    display_ = !display_;
}

void Ui::ui_( float dt ) {
    // place your Imgui logic here
    ImGui::Text( std::string( std::string( "GL_VERSION: " ) + ( const char* ) glGetString(GL_VERSION) ).c_str() );
    ImGui::Text( std::string( std::string( "GLSL_VERSION: ") + ( const char* ) glGetString(GL_SHADING_LANGUAGE_VERSION) ).c_str() );
    ImGui::Text( std::string( std::string( "GL_VENDOR: " ) + ( const char* ) glGetString(GL_VENDOR) ).c_str() );
    ImGui::Text( std::string( std::string( "GL_RENDERER: " ) + ( const char* ) glGetString(GL_RENDERER) ).c_str() );

}

}   // system
}   // pg
