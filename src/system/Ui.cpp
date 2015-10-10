#include "system/Ui.h"
#include "GL/glew.h"
#include "3rdparty/imgui.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_clipboard.h>
#include <SDL2/SDL_timer.h>

namespace pg {
namespace system {

Ui::Ui( Context& context )
:   System<Ui>(),
    ImGuiRenderer( context )
    {}

void Ui::update( ecs::EntityManager&, ecs::EventManager&, float dt ) {
    newFrame_( dt );
    
    // place your Imgui logic herestatic float f = 0.0f;
    float f = 0.0f;
    bool show_test_window = true;
    bool show_another_window = false;
    ImGui::Text("Hello, world!");
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    if (ImGui::Button("Test Window")) show_test_window ^= 1;
    if (ImGui::Button("Another Window")) show_another_window ^= 1;
    ImGui::Render();
}

}   // system
}   // pg
