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
    
    //ImGui::Text( "Hello, world!" );
    
    ImGui::Render();
}

}   // system
}   // pg
