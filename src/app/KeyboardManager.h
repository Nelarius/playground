#pragma once

#include <SDL2/SDL_events.h>
#include <functional>
#include <vector>
#include <unordered_map>

namespace pg {

namespace detail {
    
class Command {
    public:
        Command() = default;
        Command( SDL_Scancode code, std::function< void(void) > callback );
        ~Command() = default;
        
        void operator()();
        
        SDL_Scancode scancode() const;
        
    private:
        SDL_Scancode                scancode_;
        std::function< void(void) > callback_;
};

}   // detail

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
        
        void addInput( SDL_Scancode code, std::function< void(void) > callback );
        
        void handleInput();
        
    private:
        std::vector< detail::Command >  commands_{};
};

}   // pg