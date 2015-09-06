#include "app/KeyboardManager.h"
#include <cstdint>

namespace pg {

namespace detail {
    
Command::Command( SDL_Scancode code, std::function< void(void) > callback )
:   scancode_( code ),
    callback_( callback )
    {}
    
void Command::operator()() {
    callback_();
}

SDL_Scancode Command::scancode() const {
    return scancode_;
}

}

void KeyboardManager::addInput( SDL_Scancode code, std::function< void(void) > callback ) {
    commands_.emplace_back( code, callback );
}

void KeyboardManager::handleInput() {
    const uint8_t* state = SDL_GetKeyboardState( NULL );
    for ( auto& c: commands_ ) {
        if ( state[ c.scancode() ] ) {
            c();
        }
    }
}

}
