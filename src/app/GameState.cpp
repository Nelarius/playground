#include "app/GameState.h"
#include "app/AppStateStack.h"
#include "app/WorldIO.h"
#include "system/Debug.h"
#include "system/Render.h"
#include "system/Scripter.h"
#include "system/Ui.h"
#include "system/Events.h"

#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_mouse.h>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <string>

namespace pg {

GameState::GameState( Context& context, AppStateStack& stack ) 
:   AppState( context, stack ),
    events_(),
    entities_( events_ ),
    systems_( events_, entities_ ),
    keyboard_(),
    mouse_() 
    {}

void GameState::activate() {
    // everything here should eventually go into a loading state
    context_.shaderManager.addShader( "data/diffuse.vert.glsl", GL_VERTEX_SHADER );
    context_.shaderManager.addShader( "data/diffuse.frag.glsl", GL_FRAGMENT_SHADER );
    context_.shaderManager.compile( "diffuse" );
    
    context_.shaderManager.addShader( "data/ambient.vert.glsl", GL_VERTEX_SHADER );
    context_.shaderManager.addShader( "data/ambient.frag.glsl", GL_FRAGMENT_SHADER );
    context_.shaderManager.compile( "ambient" );
    
    context_.shaderManager.addShader( "data/specular.vert.glsl", GL_VERTEX_SHADER );
    context_.shaderManager.addShader( "data/specular.frag.glsl", GL_FRAGMENT_SHADER );
    context_.shaderManager.compile( "specular" );
    
    context_.shaderManager.addShader( "data/panel.vert.glsl", GL_VERTEX_SHADER );
    context_.shaderManager.addShader( "data/panel.frag.glsl", GL_FRAGMENT_SHADER );
    context_.shaderManager.compile( "panel" );
    
    systems_.add< system::Render >( context_ );
    systems_.add< system::Debug >();
    systems_.add< system::Scripter >();
    systems_.add< system::Ui >( context_ );
    systems_.configure< system::Debug >();
    systems_.configure< system::Render >();
    systems_.configure< system::Scripter >();
    
    // the full capacity of the systems are used in parsing, so the systems must be configured and ready to go!
    WorldIO world( context_ );
    world.read( "data/scene.json", entities_, events_ );
    
    keyboard_.registerKeyDownCommand( Keycode::KeyQ, Command(
      [ this ]() -> void {
          auto ui = this->systems_.system< system::Ui >();
          ui->toggleDisplay();
      },
      std::function<void()>()
    ) );
  
    mouse_.setPressCallback( 
        SDL_BUTTON_LEFT, 
        Command(
            [ this ] () -> void {
                auto ui = this->systems_.system< system::Ui >();
                ui->mouseButtonPressed( SDL_BUTTON_LEFT );
            },
            std::function<void()>()
        )
    );
    mouse_.setReleaseCallback(
        SDL_BUTTON_LEFT,
        Command(
            [ this ] () -> void {
                auto ui = this->systems_.system< system::Ui >();
                ui->mouseButtonReleased( SDL_BUTTON_LEFT );
            },
            std::function<void()>()
        )
    );
}

bool GameState::update( float dt ) {
    keyboard_.handleKeyPressedCommands();
    mouse_.update();
    systems_.update<system::Scripter>( dt );
    return false;
}

bool GameState::handleEvent( const SDL_Event& event ) { 
    if ( event.type == SDL_QUIT ) {
        requestStackClear_();
        context_.running = false;
    }
    keyboard_.handleEvent( event );
    return false; 
}

void GameState::render( float dt ) {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    systems_.update<system::Render>( dt );
    systems_.update<system::Ui>( dt );
}

}
