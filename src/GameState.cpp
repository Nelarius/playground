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
            lb::LuaRef camera = table["camera"];
            entity.assign<component::Camera>(
                camera["fov"].cast<float>(),
                camera["nearPlane"].cast<float>(),
                camera["farPlane"].cast<float>(),
                camera["perspective"].cast<bool>(),
                camera["active"].cast<bool>()
            );
        }   // camera component
        
        if ( table["transform"] ) {
            lb::LuaRef transform = table["transform"];
            entity.assign<component::Transform>(
                transform["position"].cast<math::Vector3f>(),
                transform["scale"].cast<math::Vector3f>()
            );
        }   // transform component
        
        if ( table["renderable"] ) {
            lb::LuaRef renderable = table["renderable"];
            opengl::BufferObject* buffer = context_.meshManager.get( renderable["model"] );
            opengl::Program* shader{ nullptr };
            opengl::VertexArrayObject vao{ 0 };
            std::unordered_map<std::string, float> uniforms{};
            if ( renderable["ambient"] ) {
                shader = context_.shaderManager.get( "ambient" );
                lb::LuaRef ambient = renderable["ambient"];
                math::Vector3f color = ambient["color"];
                uniforms.emplace( "color_r", color.r );
                uniforms.emplace( "color_g", color.g );
                uniforms.emplace( "color_b", color.b );
                opengl::VertexArrayObjectFactory factory{ buffer, shader };
                factory.addAttribute( "vertex", 3, GL_FLOAT, GL_FALSE, 6*sizeof(float) );
                vao = factory.getVao();
            } else if ( renderable["diffuse"] ) {
                shader = context_.shaderManager.get( "diffuse" );
                opengl::VertexArrayObjectFactory factory{ buffer, shader };
                factory.addStandardAttribute( opengl::VertexAttribute::Vertex );
                factory.addStandardAttribute( opengl::VertexAttribute::Normal );
                vao = factory.getVao();
            } else if ( renderable["specular"] )  {
                shader = context_.shaderManager.get( "specular" );
                opengl::VertexArrayObjectFactory factory{ buffer, shader };
                factory.addStandardAttribute( opengl::VertexAttribute::Vertex );
                factory.addStandardAttribute( opengl::VertexAttribute::Normal );
                vao = factory.getVao();
            }
            opengl::VertexArrayObjectFactory factory{ buffer, shader };
            entity.assign<component::Renderable>( buffer, shader, vao, uniforms );
        }   // renderable component
        
    }   // iterate over entities
}

void GameState::activate() {
    context_.shaderManager.addShader( "data/diffuse.vert.glsl", GL_VERTEX_SHADER );
    context_.shaderManager.addShader( "data/diffuse.frag.glsl", GL_FRAGMENT_SHADER );
    context_.shaderManager.compile( "diffuse" );
    
    context_.shaderManager.addShader( "data/ambient.vert.glsl", GL_VERTEX_SHADER );
    context_.shaderManager.addShader( "data/ambient.frag.glsl", GL_FRAGMENT_SHADER );
    context_.shaderManager.compile( "ambient" );
    
    context_.shaderManager.addShader( "data/specular.vert.glsl", GL_VERTEX_SHADER );
    context_.shaderManager.addShader( "data/specular.frag.glsl", GL_FRAGMENT_SHADER );
    context_.shaderManager.compile( "specular" );
    
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
