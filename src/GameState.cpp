#include "GameState.h"
#include "app/AppStateStack.h"
#include "opengl/VertexArrayObject.h"
#include "opengl/VertexArrayObjectFactory.h"
#include "manager/MeshManager.h"
#include "system/Debug.h"
//#include "system/Render.h"
#include "component/Include.h"
#include "utils/Log.h"
#include "utils/Assert.h"
#include "lua/LuaState.h"
extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}
#include <LuaBridge/LuaBridge.h>
#include "lua/BindLua.h"
#include <cmath>
#include <vector>
#include <unordered_map>

using ce::AppStateStack;
using ce::GameState;
using ce::AppState;

GameState::GameState( Context& context, AppStateStack& stack ) 
:   AppState( context, stack ),
    events_(),
    entities_( events_ ),
    systems_( events_, entities_ ){
	//systems_.add<ce::system::Render>( context );
	systems_.add<ce::system::Debug>();
    systems_.configure<ce::system::Debug>();
}

void GameState::parseScene_() {
    ce::LuaState lua{ false };
    ce::BindVector( lua );
    lua.execute( "data/scene.lua" );
    
}

void GameState::activate() {
    context_.shaderManager.addShader( "data/diffuse.vert.glsl", GL_VERTEX_SHADER );
    context_.shaderManager.addShader( "data/diffuse.frag.glsl", GL_FRAGMENT_SHADER );
    context_.shaderManager.compile( "diffuse" );
    
    context_.shaderManager.addShader( "data/ambient.vert.glsl", GL_VERTEX_SHADER );
    context_.shaderManager.addShader( "data/ambient.frag.glsl", GL_FRAGMENT_SHADER );
    
    parseScene_();
}

bool GameState::update( float dt ) {
    return false;
}

bool GameState::handleEvent( const SDL_Event& event ) { 
    if ( event.type == SDL_QUIT ) {
        requestStackClear_();
        context_.running = false;
    } 
    return false; 
}

void GameState::render( float dt ) {
    /*
     * Iterate over Renderable components here
     * */
	//systems_.update<ce::system::Render>( double (dt) );
}

