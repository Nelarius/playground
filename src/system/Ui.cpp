#include "system/Ui.h"
#include "GL/glew.h"
#include "3rdparty/imgui.h"
#include <GL/glew.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_clipboard.h>
#include <SDL2/SDL_timer.h>
#include <string>

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
        ui_( dt );
    }
    
    ImGui::Render();
}

void Ui::toggleDisplay() {
    display_ = !display_;
}

void Ui::ui_( float dt ) {
    // place your Imgui logic here
    ImGui::Begin( "GL info" );
    ImGui::Text( "GL_VERSION: %s", ( const char* ) glGetString( GL_VERSION ) );
    ImGui::Text( "GLSL_VERSION: %s", ( const char* ) glGetString( GL_SHADING_LANGUAGE_VERSION ) );
    ImGui::Text( "GL_VENDOR: %s", ( const char* ) glGetString( GL_VENDOR ) );
    ImGui::Text( "GL_RENDERER: %s", ( const char* ) glGetString( GL_RENDERER ) );
    ImGui::End();
}

}   // system
}   // pg
