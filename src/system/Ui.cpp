#include "system/Ui.h"
#include "system/Events.h"
#include "GL/glew.h"
#include "imgui/imgui.h"
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
    display_( false )
    {}

void Ui::update( ecs::EntityManager& entities, ecs::EventManager& events, float dt ) {
    if ( display_ ) {
        ui_( events, dt );
    }
}

void Ui::toggleDisplay() {
    display_ ^= 1;
}

void Ui::ui_( ecs::EventManager& events, float dt ) {
    static bool debugRendererOn = false;

    ImGui::Begin( "Control panel" );

    if ( ImGui::TreeNode( "Renderer") ) {
        ImGui::Checkbox( "bounding box", &debugRendererOn );
        events.emit< ToggleDebugRenderer >( debugRendererOn );

        ImGui::TreePop();
    }
    if ( ImGui::TreeNode( "OpenGL renderer") ) {
        ImGui::Text( "GL_VERSION: %s", ( const char* ) glGetString( GL_VERSION ) );
        ImGui::Text( "GLSL_VERSION: %s", ( const char* ) glGetString( GL_SHADING_LANGUAGE_VERSION ) );
        ImGui::Text( "GL_VENDOR: %s", ( const char* ) glGetString( GL_VENDOR ) );
        ImGui::Text( "GL_RENDERER: %s", ( const char* ) glGetString( GL_RENDERER ) );

        ImGui::TreePop();
    }

    ImGui::End();
}

}   // system
}   // pg
