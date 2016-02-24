#include "app/GameState.h"
#include "app/AppStateStack.h"
#include "app/WorldIO.h"
#include "system/DebugSystem.h"
#include "system/RenderSystem.h"
#include "system/ScriptSystem.h"
#include "system/UiSystem.h"
#include "system/Events.h"
#include "system/PickingSystem.h"
#include "system/DebugRenderSystem.h"
#include "utils/Locator.h"

#include <cmath>
#include <vector>
#include <unordered_map>
#include <string>

namespace pg {

GameState::GameState(Context& context, AppStateStack& stack)
    : AppState(context, stack),
    events_(),
    entities_(events_),
    systems_(events_, entities_) {
    Locator< ecs::EntityManager >::set(&entities_);
    Locator< ecs::EventManager >::set(&events_);
    Locator<MouseEvents>::set(&mouse_); // the Wren script needs these
}

void GameState::activate() {
    systems_.add< system::RenderSystem >(context_);
    systems_.add<system::DebugRenderSystem>(context_);
    systems_.add< system::PickingSystem >(context_);
    systems_.add< system::DebugSystem >();
    systems_.add< system::ScriptSystem >(context_, keyboard_, mouse_);
    systems_.add< system::UiSystem >(context_);
    systems_.configure< system::DebugSystem >();
    systems_.configure< system::RenderSystem >();
    systems_.configure<system::DebugRenderSystem>();
    systems_.configure<system::PickingSystem>();
    systems_.configure< system::ScriptSystem >();

    // NOTICE
    // this is a dirty hack to get ScriptSystem bound to Wren
    // I really need a way to set the bytes of a foreign object in a better way....
    Locator<system::ScriptSystem>::set(dynamic_cast<system::ScriptSystem*>(systems_.system<system::ScriptSystem>()));
    Locator<system::PickingSystem>::set(dynamic_cast<system::PickingSystem*>(systems_.system<system::PickingSystem>()));
    Locator<system::RenderSystem>::set(dynamic_cast<system::RenderSystem*>(systems_.system<system::RenderSystem>()));

    // the full capacity of the systems are used in parsing, so the systems must be configured and ready to go!
    WorldIO world(context_);
    world.read("data/scene.json", entities_, events_);

    keyboard_.registerKeyDownCallback(Keycode::KeyF1,
        [this]() -> void {
        auto ui = dynamic_cast<system::UiSystem*>(this->systems_.system< system::UiSystem >());
        ui->toggleDisplay();
    });
    keyboard_.registerKeyDownCallback(Keycode::KeyP,
        [this]() -> void {
        this->requestStackPush_(states::Pause);
    });

    mouse_.registerMouseDownCallback(MouseButton::Left,
        [this]() -> void {
        auto pick = dynamic_cast<system::PickingSystem*>(this->systems_.system<system::PickingSystem>());
        auto coords = mouse_.getMouseCoords();
        ecs::Entity target = pick->rayCast(entities_, events_, coords.x, coords.y);
        if (target.isValid()) {
            LOG_DEBUG << "HIT";
        }
        else {
            LOG_DEBUG << "MISS";
        }
    });
}

bool GameState::update(float dt) {
    keyboard_.handleKeyPressedCallbacks();
    mouse_.handleMousePressedCallbacks();
    systems_.update< system::ScriptSystem >(dt);
    return false;
}

bool GameState::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_QUIT) {
        requestStackClear_();
        context_.running = false;
    }
    keyboard_.handleEvent(event);
    mouse_.handleEvent(event);
    return false;
}

void GameState::render(float dt) {
    systems_.update< system::RenderSystem >(dt);
    systems_.update<system::DebugRenderSystem>(dt);
    systems_.update< system::UiSystem >(dt);
}

}
