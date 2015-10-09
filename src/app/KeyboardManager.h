#pragma once

#include "app/Command.h"
#include <SDL2/SDL_events.h>
#include <functional>
#include <vector>
#include <unordered_map>

namespace pg {

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
        
        void handleInput();
        
    private:
        std::vector< std::pair< SDL_Scancode, Command> >  commands_{};
};

}   // pg