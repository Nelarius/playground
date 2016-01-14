#include "system/Ui.h"
#include "system/Events.h"
#include "GL/glew.h"
#include "imgui/imgui.h"
#include <GL/glew.h>
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

    ImGui::Begin( "System settings" );

    if ( ImGui::TreeNode( "Renderer") ) {
        ImGui::Text("OpenGL info:");
        ImGui::Text("  GL_VERSION: %s", (const char*)glGetString(GL_VERSION));
        ImGui::Text("  GLSL_VERSION: %s", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
        ImGui::Text("  GL_VENDOR: %s", (const char*)glGetString(GL_VENDOR));
        ImGui::Text("  GL_RENDERER: %s", (const char*)glGetString(GL_RENDERER));
        ImGui::Checkbox( "bounding box", &debugRendererOn );
        events.emit< ToggleDebugRenderer >( debugRendererOn );

        ImGui::TreePop();
    }

    ImGui::End();
}

}   // system
}   // pg
