#pragma once

#include "app/Command.h"
#include "app/Context.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <functional>
#include <vector>
#include <unordered_map>
#include <map>

namespace pg {
    
enum Keycode {
    KeyNull = 0,
    KeyA = 97,
    KeyB
};

/**
 * @class KeyboardManager
 * @author Johann
 * @date 06/09/2015
 * @file KeyboardManager.h
 * @brief Handle real-time input by binding functions to keys.
 */
class KeyboardManager {
    public:
        KeyboardManager() = default;
        ~KeyboardManager() = default;
        
        void addInput( SDL_Scancode code, const Command& command );
        
        void registerKeyDownCommand( SDL_Keycode, const Command& );
        /**
        * @brief register a real-time command for keys 97-127 (alphabetical keys) 
        */
        void registerKeyPressedCommand( SDL_Keycode, const Command& );
        void registerKeyUpCommand( SDL_Keycode, const Command& );
        
        void handleEvent( const SDL_Event& events );
        
    private:
        std::vector< std::pair< SDL_Scancode, Command> >  commands_{};
        
        std::unordered_map< SDL_Keycode, Command >          keyDownCommands_{};
        std::vector< std::pair< SDL_Keycode, Command > >    keyPressedCommands_{};
        std::unordered_map< SDL_Keycode, Command >          keyUpCommands_{};
};

}   // pg