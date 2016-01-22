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
    static bool debugBoundingBoxes = false;
    static bool debugLines = false;

    ImGui::Begin( "System settings" );

    if ( ImGui::TreeNode( "Renderer") ) {
        ImGui::Text("OpenGL info:");
        ImGui::Text("  GL_VERSION: %s", (const char*)glGetString(GL_VERSION));
        ImGui::Text("  GLSL_VERSION: %s", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
        ImGui::Text("  GL_VENDOR: %s", (const char*)glGetString(GL_VENDOR));
        ImGui::Text("  GL_RENDERER: %s", (const char*)glGetString(GL_RENDERER));

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Debug renderer")) {
        ImGui::Checkbox("bounding box", &debugBoundingBoxes);
        ImGui::Checkbox("debug lines", &debugLines);
        events.emit<system::ShowDebugBoxes>(debugBoundingBoxes);
        events.emit<system::ShowDebugLines>(debugLines);

        ImGui::TreePop();
    }

    ImGui::End();
}

}   // system
}   // pg
