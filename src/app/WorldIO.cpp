#include "app/WorldIO.h"
#include "opengl/VertexArrayObject.h"
#include "opengl/VertexArrayObjectFactory.h"
#include "manager/MeshManager.h"
#include "system/Material.h"
#include "system/Debug.h"
#include "system/Render.h"
#include "system/Scripter.h"
#include "system/Ui.h"
#include "system/Events.h"
#include "component/Include.h"
#include "utils/Log.h"
#include "utils/Assert.h"
#include "lua/LuaState.h"
#include "lua/LuaBridge.h"
#include "lua/BindLua.h"
#include "utils/File.h"
#include "3rdparty/json11/json11.hpp"

namespace pg {

WorldIO::WorldIO( Context& context )
:   context_( context )
    {}

void WorldIO::read( 
                        const std::string& file,
                        ecs::EntityManager& entities,
                        ecs::EventManager& events
                    ) {

    auto json = pg::FileToString( "data/scene.json" );
    std::string error{""};
    auto scene = json11::Json::parse( json, error ).array_items();

    for ( auto object: scene ) {
        ecs::Entity entity = entities.create();
        auto transform = object["transform"];
        auto renderable = object["renderable"];
        auto script = object["script"];
        auto camera = object["camera"];
        auto pointLight = object["pointLight"];

        if ( !transform.is_null() ) {
            auto contents = transform.object_items();
            auto pos = contents["position"].array_items();
            auto rot = contents["rotation"].array_items();
            auto sca = contents["scale"].array_items();
            entity.assign< component::Transform >(
                math::Vector3f( pos[0].number_value(), pos[1].number_value(), pos[2].number_value() ),
                math::Quatf( rot[0].number_value(), rot[1].number_value(), rot[2].number_value(), rot[3].number_value() ),
                math::Vector3f( sca[0].number_value(), sca[1].number_value(), sca[2].number_value() )
            );
        }   // transform

        if ( !renderable.is_null() ) {
            auto contents = renderable.object_items();
            opengl::BufferObject* buffer = context_.meshManager.get( contents["model"].string_value() );
            opengl::Program* shader{ nullptr };
            opengl::VertexArrayObject vao{ 0 };
            system::Material mat;

            if ( !contents[ "specular"].is_null() ) {
                auto specular = contents[ "specular" ].object_items();
                std::unordered_map<std::string, float> uniforms{};
                uniforms.emplace( "shininess", specular[ "shininess" ].number_value() );
                auto scolor = specular[ "specularColor" ].array_items();
                math::Vector3f specColor(
                    scolor[0].number_value(), scolor[1].number_value(), scolor[2].number_value()
                );
                auto acolor = specular[ "ambientColor" ].array_items();
                math::Vector3f surfColor(
                    acolor[0].number_value(), acolor[1].number_value(), acolor[2].number_value()
                );
                uniforms.emplace( "specColor_r", specColor.r );
                uniforms.emplace( "specColor_g", specColor.g );
                uniforms.emplace( "specColor_b", specColor.b );
                uniforms.emplace( "ambientColor_r", surfColor.r );
                uniforms.emplace( "ambientColor_g", surfColor.g );
                uniforms.emplace( "ambientColor_b", surfColor.b );
                
                mat.type = system::MaterialType::Specular;
                mat.uniforms = uniforms;
                shader = context_.shaderManager.get( "specular" );
                opengl::VertexArrayObjectFactory factory{ buffer, shader };
                factory.addStandardAttribute( opengl::VertexAttribute::Vertex );
                factory.addStandardAttribute( opengl::VertexAttribute::Normal );
                vao = factory.getVao();
            }
            opengl::VertexArrayObjectFactory factory{ buffer, shader };
            entity.assign<component::Renderable>( buffer, shader, vao, mat );
        }   //renderable

        if ( !pointLight.is_null() ) {
            // add the point light here
        }

        if ( !camera.is_null() ) {
            auto contents = camera.object_items();
            // cast doubles (numbers) to float, to avoid narrowing errors
            entity.assign< component::Camera >(
                (float) contents["fov"].number_value(),
                (float) contents["nearPlane"].number_value(),
                (float) contents["farPlane"].number_value(),
                true,
                true
            );
        }   //camera
        
        if ( !script.is_null() ) {
            auto file = script.string_value();
            LuaState lua{ false };
            BindAll( lua.get() );
            lb::push( lua.get(), entity );
            lua_setglobal( lua.get(), "entity" );
            lua.execute( file );
            entity.assign< component::Script >( lua );
        }   // script
    }
}

}