#include "app/GameState.h"
#include "app/AppStateStack.h"
#include "app/SceneIO.h"
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
    : AppState(context, stack) {
    Locator< ecs::EntityManager >::set(&context_.entityManager);
    Locator< ecs::EventManager >::set(&context_.eventManager);
    Locator<MouseEvents>::set(&mouse_); // the Wren script needs these
}

void GameState::activate() {
    context_.systemManager.add< system::RenderSystem >(context_);
    context_.systemManager.add<system::DebugRenderSystem>(context_);
    context_.systemManager.add< system::PickingSystem >(context_);
    context_.systemManager.add< system::DebugSystem >();
    context_.systemManager.add< system::ScriptSystem >(context_, keyboard_, mouse_);
    context_.systemManager.add< system::UiSystem >(context_);
    context_.systemManager.configure< system::DebugSystem >();
    context_.systemManager.configure< system::RenderSystem >();
    context_.systemManager.configure<system::DebugRenderSystem>();
    context_.systemManager.configure<system::PickingSystem>();
    context_.systemManager.configure< system::ScriptSystem >();

    // NOTICE
    // this is a dirty hack to get ScriptSystem bound to Wren
    // I really need a way to set the bytes of a foreign object in a better way....
    Locator<system::ScriptSystem>::set(&context_.systemManager.system<system::ScriptSystem>());
    Locator<system::PickingSystem>::set(&context_.systemManager.system<system::PickingSystem>());
    Locator<system::RenderSystem>::set(&context_.systemManager.system<system::RenderSystem>());
    Locator<system::DebugRenderSystem>::set(&context_.systemManager.system<system::DebugRenderSystem>());

    keyboard_.registerKeyDownCallback(Keycode::KeyF1,
        [this]() -> void {
        auto ui = this->context_.systemManager.system< system::UiSystem >();
        ui.toggleDisplay();
    });
    keyboard_.registerKeyDownCallback(Keycode::KeyP,
        [this]() -> void {
        this->requestStackPush_(states::Pause);
    });

    readScene(context_, "scene.json");
}

bool GameState::update(float dt) {
    keyboard_.handleKeyPressedCallbacks();
    mouse_.handleMousePressedCallbacks();
    context_.systemManager.update< system::ScriptSystem >(dt);
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
    context_.systemManager.update< system::RenderSystem >(dt);
    context_.systemManager.update<system::DebugRenderSystem>(dt);
    context_.systemManager.update< system::UiSystem >(dt);
}

}
