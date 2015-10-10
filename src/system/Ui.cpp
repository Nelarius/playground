#include "system/Ui.h"
#include "GL/glew.h"
#include "3rdparty/imgui.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_clipboard.h>
#include <SDL2/SDL_timer.h>

namespace {
    bool show_another_window{ false };
    float f{ 0.0f };
}

namespace pg {
namespace system {

Ui::Ui( Context& context )
:   System<Ui>(),
    ImGuiRenderer( context ),
    on_( false )
    {}

void Ui::update( ecs::EntityManager&, ecs::EventManager&, float dt ) {
    newFrame_( dt );
    
    if ( on_ ) {
        // place your Imgui logic here
        ImGui::Text("Hello, world!");
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        if (ImGui::Button("Another Window")) show_another_window ^= 1;
        
        if (show_another_window)
        {
            ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiSetCond_FirstUseEver);
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello");
            ImGui::End();
        }
    }
    
    ImGui::Render();
}

void Ui::toggleDisplay() {
    on_ = !on_;
}

}   // system
}   // pg
