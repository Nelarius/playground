#include "GameState.h"
#include "app/AppStateStack.h"
#include "opengl/VertexArrayObject.h"
#include "opengl/VertexArrayObjectFactory.h"
#include "manager/MeshManager.h"
#include "system/Debug.h"
#include "system/Render.h"
#include "system/Physics2d.h"
#include "system/Collision.h"
#include "component/Components.h"
#include "utils/Log.h"
#include "utils/Assert.h"
extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}
#include <LuaBridge/LuaBridge.h>
#include <cmath>
#include <vector>
#include <unordered_map>

using ce::AppStateStack;
using ce::GameState;
using ce::AppState;

namespace ex = entityx;


GameState::GameState( Context& context, AppStateStack& stack ) 
:   AppState( context, stack ) {
	systems_.add<ce::system::Render>( context );
	systems_.add<ce::system::Debug>();
    systems_.add<ce::system::Physics2d>( context );
    systems_.add<ce::system::Collision>();
	systems_.configure();
}

void GameState::parseScene_() {    
    /*YAML::Node scene = YAML::LoadFile( "data/scene.yaml" );
    for ( const auto& node: scene ) {
        ex::Entity entity = entities_.create();
        
        // transform
        if ( node["entity"]["transform"] ) {
            YAML::Node transform = node["entity"]["transform"];
            glm::vec3 pos{};
            glm::vec3 sca{};
            for ( int i = 0; i < 3; i++ ) {
                sca[i] = transform["scale"][i].as<float>();
            }
            glm::quat rot = FromAxisAngle(
                transform["rotation"][0].as<float>(),
                transform["rotation"][1].as<float>(),
                transform["rotation"][2].as<float>(),
                transform["rotation"][3].as<float>()
            );
            for ( int i = 0; i < 3; i++ ) {
                pos[i] = transform["position"][i].as<float>();
            }
			LOG(ce::LogLevel::Info)<<transform["rotation"][0].as<float>()<<" "<<transform["rotation"][1].as<float>()<<" "<<transform["rotation"][2].as<float>()<<" "<<transform["rotation"][3].as<float>();
            entity.assign<ce::component::Transform>( pos, rot, sca );
        }
        
        // renderable
        if ( node["entity"]["renderable"] ) {
            YAML::Node renderable = node["entity"]["renderable"];
            ce::BorrowedBufferObject buffer = context_.meshManager.get( 
                renderable["mesh"].as<std::string>()
            );
            ce::BorrowedProgram program = context_.shaderManager.get(
                renderable["material"]["shader"].as<std::string>()
            );
            std::unordered_map<std::string, float> unif{};
            for ( auto it = renderable["material"].begin(); it != renderable["material"].end(); it++ ) {
                if ( it->first.as<std::string>() == "shader" ) {
                    continue;
                }
                unif.emplace( it->first.as<std::string>(), it->second.as<float>() );
            }
            
            ce::VertexArrayObjectFactory factory{ buffer, program };
            factory.addStandardAttribute( ce::VertexAttribute::Vertex );
            //factory.addStandardAttribute( ce::VertexAttribute::Normal );
            ce::VertexArrayObject vao = factory.getVao();
            
            entity.assign<ce::component::Renderable>( buffer, program, vao, unif );
        }
        
        // camera
        if ( node["entity"]["camera"] ) {
            YAML::Node camera = node["entity"]["camera"];
            entity.assign<ce::component::Camera>(
                camera["fov"].as<float>(),
                camera["near_plane"].as<float>(),
                camera["far_plane"].as<float>(),
                camera["perspective"].as<bool>(),
                camera["active"].as<bool>()
            );
        }
        
       // body
        if ( node["entity"]["body"] ) {
            YAML::Node body = node["entity"]["body"];
            glm::vec3 v{};
            for ( int i = 0; i < 3; i++ ) {
                v[i] = body["velocity"][i].as<float>();
            }
            entity.assign<ce::component::Body>(
                body["mass"].as<float>(),
                body["friction"].as<float>(),
                v
            );
        }
        
        // wall collider
        if ( node["entity"]["wall_collider"] ) {
            YAML::Node wall = node["entity"]["wall_collider"];
            glm::vec3 start{
                wall["start"][0].as<float>(),
                wall["start"][1].as<float>(),
                wall["start"][2].as<float>()
            };
            glm::vec3 end{
                wall["end"][0].as<float>(),
                wall["end"][1].as<float>(),
                wall["end"][2].as<float>()
            };
            entity.assign<ce::component::WallCollider>( start, end );
        }
        
        // circle collider
        if ( node["entity"]["circle_collider"] ) {
            YAML::Node circle = node["entity"]["circle_collider"];
            entity.assign<ce::component::CircleCollider>( circle["radius"].as<float>() );
        }
    }*/
}

void GameState::activate() {
    context_.shaderManager.addShader( "data/diffuse.vert.glsl", GL_VERTEX_SHADER );
    context_.shaderManager.addShader( "data/diffuse.frag.glsl", GL_FRAGMENT_SHADER );
    context_.shaderManager.compile( "diffuse" );
    
    context_.shaderManager.addShader( "data/ambient.vert.glsl", GL_VERTEX_SHADER );
    context_.shaderManager.addShader( "data/ambient.frag.glsl", GL_FRAGMENT_SHADER );
    context_.shaderManager.compile( "ambient" );
    
    parseScene_();
}

bool GameState::update( float dt ) {
    systems_.update<ce::system::Physics2d>( double( dt ) );
    systems_.update<ce::system::Collision>( double( dt ) );
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
	systems_.update<ce::system::Render>( double (dt) );
}

