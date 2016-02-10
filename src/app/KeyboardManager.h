#pragma once

#include "app/Command.h"
#include "app/Context.h"
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_scancode.h>
#include <functional>
#include <map>
#include <vector>
#include <string>

namespace pg {

namespace ecs {
class Entity;
}

namespace system {
class ScriptSystem;
}

/**
* These enumerations correspond to SDL_Keycode values and can be used interchangeably,
* if both are converted to an integer first.
*
* See https://wiki.libsdl.org/SDLKeycodeLookup and
* http://hg.libsdl.org/SDL/file/default/include/SDL_keycode.h for documentation.
*/
enum Keycode {
    KeyNull = 0,
    KeyReturn = '\r',
    KeyEscape = '\033',
    KeyBackspace = '\b',
    KeyTab = '\t',
    KeySpace = ' ',
    KeyExclaim = '!',
    KeyQuoteDouble = '"',
    KeyHash = '#',
    KeyPercent = '%',
    KeyDollar = '$',
    KeyAmpersand = '&',

    KeyA = 'a',
    KeyB = 'b',
    KeyC = 'c',
    KeyD = 'd',
    KeyE = 'e',
    KeyF = 'f',
    KeyG = 'g',
    KeyH = 'h',
    KeyI = 'i',
    KeyJ = 'j',
    KeyK = 'k',
    KeyL = 'l',
    KeyM = 'm',
    KeyN = 'n',
    KeyO = 'o',
    KeyP = 'p',
    KeyQ = 'q',
    KeyR = 'r',
    KeyS = 's',
    KeyT = 't',
    KeyU = 'u',
    KeyV = 'v',
    KeyW = 'w',
    KeyX = 'x',
    KeyY = 'y',
    KeyZ = 'z',
    Key0 = '0',
    Key1 = '1',
    Key2 = '2',
    Key3 = '3',
    Key4 = '4',
    Key5 = '5',
    Key6 = '6',
    Key7 = '7',
    Key8 = '8',
    Key9 = '9',

    KeyF1 = SDL_SCANCODE_F1 | 0x40000000,
    KeyF2 = SDL_SCANCODE_F2 | 0x40000000,
    KeyF3 = SDL_SCANCODE_F3 | 0x40000000,
    KeyF4 = SDL_SCANCODE_F4 | 0x40000000,
    KeyF5 = SDL_SCANCODE_F5 | 0x40000000,
    KeyF6 = SDL_SCANCODE_F6 | 0x40000000,
    KeyF7 = SDL_SCANCODE_F7 | 0x40000000,
    KeyF8 = SDL_SCANCODE_F8 | 0x40000000,
    KeyF9 = SDL_SCANCODE_F9 | 0x40000000,
    KeyF10 = SDL_SCANCODE_F10 | 0x40000000,
    KeyF11 = SDL_SCANCODE_F11 | 0x40000000,
    KeyF12 = SDL_SCANCODE_F12 | 0x40000000
};

/**
 * @class KeyboardManager
 * @file KeyboardManager.h
 * @brief Use this class to register commands to certain keyboard events.
 * This class will handle what to do with an event.
 */
class KeyboardManager {
public:
    KeyboardManager() = default;
    ~KeyboardManager() = default;

    void registerKeyDownScriptCallback(std::string, ecs::Entity*);
    void registerKeyPressedScriptCallback(std::string, ecs::Entity*);
    void registerKeyUpScriptCallback(std::string, ecs::Entity*);
    void registerKeyDownCallback(Keycode, std::function<void()>);
    void registerKeyPressedCallback(Keycode, std::function<void()>);
    void registerKeyUpCallback(Keycode, std::function<void()>);
    /**
    * @brief Execute a command corresponding to a registered event.
    */
    void handleEvent(const SDL_Event& events);
    /**
    * @brief Execute registered key pressed commands.
    * This method must be called only once during the update loop.
    */
    void handleKeyPressedCallbacks();

    void setScriptHandler(system::ScriptSystem&);

private:

    struct CallbackData {
        explicit CallbackData(ecs::Entity* entity)
            : entities{},
            callback{ []() -> void {} } {
            entities.push_back(entity);
        }
        explicit CallbackData(std::function<void()> callable)
            : entities{},
            callback{ callable } {}
        std::vector<ecs::Entity*>   entities;
        std::function<void()>       callback;
    };

    void addToMap_(std::map<int, CallbackData>&, Keycode, ecs::Entity*);
    void addToMap_(std::map<int, CallbackData>&, Keycode, std::function<void()>);

    std::map<int, CallbackData> keyDownCallbacks_{};
    std::map<int, CallbackData> keyPressedCallbacks_{};
    std::map<int, CallbackData> keyUpCallbacks_{};
    system::ScriptSystem*      scriptSystem_{ nullptr };
};

}   // pg
