#include "GameState.h"
#include "app/AppStateStack.h"
#include "opengl/VertexArrayObject.h"
#include "opengl/VertexArrayObjectFactory.h"
#include "manager/MeshManager.h"
#include "system/Debug.h"
#include "system/Render.h"
#include "system/Scripter.h"
#include "component/Include.h"
#include "utils/Log.h"
#include "utils/Assert.h"
#include "lua/LuaState.h"
#include "lua/LuaBridge.h"
#include "lua/BindLua.h"
#include <cmath>
#include <vector>
#include <unordered_map>
#include <string>

#include <iostream>

namespace pg {

GameState::GameState( Context& context, AppStateStack& stack ) 
:   AppState( context, stack ),
    events_(),
    entities_( events_ ),
    systems_( events_, entities_ ){
	systems_.add<system::Render>( context );
	systems_.add<system::Debug>();
    systems_.add<system::Scripter>();
    systems_.configure<system::Debug>();
    systems_.configure<system::Scripter>();
}

void GameState::loadScene_() {
    LuaState lua{ false };
    BindVector( lua.get() );
    lua.execute( "data/scene.lua" );
    
    lb::LuaRef entities = lb::getGlobal( lua.get(), "entities" );
    for ( lb::Iterator iter( entities ); !iter.isNil(); ++iter ) {
        auto entity = entities_.create();
        lb::LuaRef table = *iter;
        
        if ( table["script"] ) {
            std::string scriptFile = table["script"].cast<std::string>();
            LuaState lua{ false };
            BindAll( lua.get() );
            lb::push( lua.get(), entity );
            lua_setglobal( lua.get(), "entity" );
            lua.execute( scriptFile );
            entity.assign<component::Script>( lua );
        }   // script component
        if ( table["camera"] ) {
            //
        }   // camera component
        if ( table["transform"] ) {
            entity.assign<component::Transform>(
                std::initializer_list<float>{ 0.0f, 0.0f, 0.0f },
                std::initializer_list<float>{ 1.0f, 1.0f, 1.0f }
            );
        }   // transform component
        if ( table["renderable"] ) {
            lb::LuaRef renderable = table["renderable"];
            if ( renderable["ambient"] ) {
                //
            } else if ( renderable["diffuse"] ) {
                //
            }
        }   // renderable component
    }   // iterate over entities
}

void GameState::activate() {
    context_.shaderManager.addShader( "data/diffuse.vert.glsl", GL_VERTEX_SHADER );
    context_.shaderManager.addShader( "data/diffuse.frag.glsl", GL_FRAGMENT_SHADER );
    context_.shaderManager.compile( "diffuse" );
    
    context_.shaderManager.addShader( "data/ambient.vert.glsl", GL_VERTEX_SHADER );
    context_.shaderManager.addShader( "data/ambient.frag.glsl", GL_FRAGMENT_SHADER );
    
    loadScene_();
}

bool GameState::update( float dt ) {
    systems_.update<system::Scripter>( dt );
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
	systems_.update<system::Render>( dt );
}

}
