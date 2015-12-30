#include "app/KeyboardManager.h"
#include "ecs/Include.h"
#include "utils/StringBimap.h"
#include "utils/Log.h"
#include <cstdint>
#include <map>
#include <utility>

namespace {

pg::StringBimap<pg::Keycode> initMap() {
    pg::StringBimap<pg::Keycode> bimap{};

#define PG_KEYBOARD_MAPPING(identifier) bimap.insert(pg::Keycode::identifier, #identifier)
    PG_KEYBOARD_MAPPING(KeyNull);
    PG_KEYBOARD_MAPPING(KeyReturn);
    PG_KEYBOARD_MAPPING(KeyEscape);
    PG_KEYBOARD_MAPPING(KeyBackspace);
    PG_KEYBOARD_MAPPING(KeyTab);
    PG_KEYBOARD_MAPPING(KeySpace);
    PG_KEYBOARD_MAPPING(KeyExclaim);
    PG_KEYBOARD_MAPPING(KeyQuoteDouble);
    PG_KEYBOARD_MAPPING(KeyHash);
    PG_KEYBOARD_MAPPING(KeyPercent);
    PG_KEYBOARD_MAPPING(KeyDollar);
    PG_KEYBOARD_MAPPING(KeyAmpersand);

    PG_KEYBOARD_MAPPING(KeyA);
    PG_KEYBOARD_MAPPING(KeyB);
    PG_KEYBOARD_MAPPING(KeyC);
    PG_KEYBOARD_MAPPING(KeyD);
    PG_KEYBOARD_MAPPING(KeyE);
    PG_KEYBOARD_MAPPING(KeyF);
    PG_KEYBOARD_MAPPING(KeyG);
    PG_KEYBOARD_MAPPING(KeyH);
    PG_KEYBOARD_MAPPING(KeyI);
    PG_KEYBOARD_MAPPING(KeyJ);
    PG_KEYBOARD_MAPPING(KeyK);
    PG_KEYBOARD_MAPPING(KeyL);
    PG_KEYBOARD_MAPPING(KeyM);
    PG_KEYBOARD_MAPPING(KeyN);
    PG_KEYBOARD_MAPPING(KeyO);
    PG_KEYBOARD_MAPPING(KeyP);
    PG_KEYBOARD_MAPPING(KeyQ);
    PG_KEYBOARD_MAPPING(KeyR);
    PG_KEYBOARD_MAPPING(KeyS);
    PG_KEYBOARD_MAPPING(KeyT);
    PG_KEYBOARD_MAPPING(KeyU);
    PG_KEYBOARD_MAPPING(KeyV);
    PG_KEYBOARD_MAPPING(KeyW);
    PG_KEYBOARD_MAPPING(KeyX);
    PG_KEYBOARD_MAPPING(KeyY);
    PG_KEYBOARD_MAPPING(KeyZ);
    PG_KEYBOARD_MAPPING(Key0);
    PG_KEYBOARD_MAPPING(Key1);
    PG_KEYBOARD_MAPPING(Key2);
    PG_KEYBOARD_MAPPING(Key3);
    PG_KEYBOARD_MAPPING(Key4);
    PG_KEYBOARD_MAPPING(Key5);
    PG_KEYBOARD_MAPPING(Key6);
    PG_KEYBOARD_MAPPING(Key7);
    PG_KEYBOARD_MAPPING(Key8);
    PG_KEYBOARD_MAPPING(Key9);

    PG_KEYBOARD_MAPPING(KeyF1);
    PG_KEYBOARD_MAPPING(KeyF2);
    PG_KEYBOARD_MAPPING(KeyF3);
    PG_KEYBOARD_MAPPING(KeyF4);
    PG_KEYBOARD_MAPPING(KeyF5);
    PG_KEYBOARD_MAPPING(KeyF6);
    PG_KEYBOARD_MAPPING(KeyF7);
    PG_KEYBOARD_MAPPING(KeyF8);
    PG_KEYBOARD_MAPPING(KeyF9);
    PG_KEYBOARD_MAPPING(KeyF10);
    PG_KEYBOARD_MAPPING(KeyF11);
    PG_KEYBOARD_MAPPING(KeyF12);
#undef PG_KEYBOARD_MAPPING
    return bimap;
}

const pg::StringBimap<pg::Keycode>& keymapInstance() {
    static pg::StringBimap<pg::Keycode> instance = initMap();
    return instance;
}

}

namespace pg {

const std::string& toString(Keycode key) {
    return keymapInstance().at(key);
}

Keycode toEnum(std::string str) {
    return keymapInstance().at(str);
}

void KeyboardManager::addToMap_(std::map<int, CallbackData>& m, Keycode code, ecs::Entity* entity) {
    auto it = m.find(int(code));
    if (it != m.end()) {
        it->second.entities.push_back(entity);
    }
    else {
        m.insert(std::make_pair(int(code), CallbackData(entity)));
    }
}

void KeyboardManager::addToMap_(std::map<int, CallbackData>& m, Keycode code, std::function<void()> callable) {
    auto it = m.find(int(code));
    if (it != m.end()) {
        it->second.callback = callable;
    }
    else {
        m.insert(std::make_pair(int(code), CallbackData(callable)));
    }
}

void KeyboardManager::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        auto it = keyDownCallbacks_.find(int(event.key.keysym.sym));
        if (it != keyDownCallbacks_.end()) {
            it->second.callback();
        }
    }
    else if (event.type == SDL_KEYUP) {
        auto it = keyUpCallbacks_.find(int(event.key.keysym.sym));
        if (it != keyUpCallbacks_.end()) {
            it->second.callback();
        }
    }
}

void KeyboardManager::handleKeyPressedCallbacks() {
    // handle real time input
    const uint8_t* state = SDL_GetKeyboardState(NULL);
    for (auto& c : keyPressedCallbacks_) {
        // the key code is the index
        if (state[SDL_GetScancodeFromKey(int(c.first))]) {
            c.second.callback();
        }
    }
}

void KeyboardManager::registerKeyDownScriptCallback(std::string str, ecs::Entity* entity) {
    addToMap_(keyDownCallbacks_, toEnum(str), entity);
}

void KeyboardManager::registerKeyPressedScriptCallback(std::string str, ecs::Entity* entity) {
    addToMap_(keyPressedCallbacks_, toEnum(str), entity);
}

void KeyboardManager::registerKeyUpScriptCallback(std::string str, ecs::Entity* entity) {
    addToMap_(keyDownCallbacks_, toEnum(str), entity);
}

void KeyboardManager::registerKeyDownCallback(Keycode code, std::function<void()> callable) {
    addToMap_(keyDownCallbacks_, code, callable);
}

void KeyboardManager::registerKeyPressedCallback(Keycode code, std::function<void()> callable) {
    addToMap_(keyPressedCallbacks_, code, callable);
}

void KeyboardManager::registerKeyUpCallback(Keycode code, std::function<void()> callable) {
    addToMap_(keyUpCallbacks_, code, callable);
}

}
