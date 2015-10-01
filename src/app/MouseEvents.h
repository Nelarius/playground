#pragma once

#include "app/Command.h"
#include <vector>
#include <utility>
#include <unordered_map>

namespace pg {
/**
 * @class MouseEvents
 * @author Nelarius
 * @date 30/09/2015
 * @file MouseEvents.h
 * @brief Bind commands to mouse events.
 * Internally uses SDL. Mouse button ID's are denoted by integers. Get the corresponding value
 * by doing SDL_BUTTON_LEFT, SDL_BUTTON_MIDDLE, SDL_BUTTON_RIGHT. Get the correct mask by
 * writing SDL_BUTTON( SDL_BUTTON_LEFT ), etc.
 */
class MouseEvents{
    public:
        MouseEvents()   = default;
        ~MouseEvents()  = default;
        
        void set( int button, const Command& command );
        void update();
    
    private:
        uint32_t    previousState_; // the mouse state mask
        std::unordered_map< int, Command >  pressed_{};
        std::unordered_map< int, Command >  released_{};
};

}