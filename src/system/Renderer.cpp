#include "system/Renderer.h"
#include "system/Material.h"
#include "opengl/VertexArrayObjectFactory.h"
#include "component/Include.h"
#include "utils/Log.h"
#include "utils/Assert.h"

#include <cmath>

namespace {

const float DegreesToRads = 3.141592653f / 180.0f;
const float RadsToDegrees = 180.0f / 3.141592653f;
const float Pi = 3.141592653f;

}

namespace pg {
namespace system {

Renderer::Renderer( Context& context )
:   System< Renderer >(),
    cameraEntity_{},
    lightEntity_{},
    defaultProjection_{},
    context_{ context },
    debug_{ false } {
    defaultProjection_ = math::Matrix4f::Perspective( 70.0f, 1.5f, 0.1f, 100.0f );
}

void Renderer::configure( ecs::EventManager& events ) {
    events.subscribe< ecs::ComponentAssignedEvent<component::Camera> >( *this );
    events.subscribe< ecs::ComponentAssignedEvent<component::PointLight> >( *this );
    events.subscribe< ToggleDebugRenderer >( *this );
}

void Renderer::receive( const ecs::ComponentAssignedEvent< component::Camera >& event ) {
    cameraEntity_ = event.entity;
}

void Renderer::receive( const ecs::ComponentAssignedEvent< component::PointLight >& event ) {
    lightEntity_ = event.entity;
}

void Renderer::receive( const ToggleDebugRenderer& debug ) {
    debug_ = debug.show;
}

void Renderer::update(
    ecs::EntityManager& entities,
    ecs::EventManager& events,
    float dt 
) {
    math::Matrix4f cameraMatrix{ defaultProjection_ };
    math::Vector3f cameraPos{};
    math::Vector3f lightPos{};
    math::Vector3f lightIntensity{};
    float attenuation = 1.0f;
    float ambientCoefficient = 0.5f;

    if ( cameraEntity_.isValid() ) {
        float aspectRatio = float( context_.window->width() ) / context_.window->height();
        auto transform = cameraEntity_.component< component::Transform >();
        auto view = math::Matrix4f::Translation( transform->position )
                    * math::Matrix4f::Rotation( transform->rotation )
                    * math::Matrix4f::Scale( transform->scale );
        auto camera = cameraEntity_.component< component::Camera >();
        auto proj = math::Matrix4f::Perspective( 
            camera->verticalFov, 
            aspectRatio,
            camera->nearPlane,
            camera->farPlane
        );
        cameraMatrix = proj * view.inverse();
    }

    if ( lightEntity_.isValid() ) {
        lightPos = lightEntity_.component< component::Transform >()->position;
        lightIntensity = lightEntity_.component< component::PointLight >()->intensity;
        attenuation = lightEntity_.component< component::PointLight >()->attenuation;
        ambientCoefficient = lightEntity_.component< component::PointLight >()->ambientCoefficient;
    }

     /*
      * Then, iterate over renderables
      */
    for ( ecs::Entity entity: entities.join< component::Transform, component::Renderable>() ) {
        auto renderable = entity.component< component::Renderable>();
        auto transform = entity.component< component::Transform>();
        auto shader = renderable->shader;
        shader->use();
        shader->setUniform( 
            "model",
            math::Matrix4f::Translation( transform->position )
            * math::Matrix4f::Rotation( transform->rotation )
            * math::Matrix4f::Scale( transform->scale )
        );
        shader->setUniform( "camera", cameraMatrix );
        for ( const auto& it: renderable->material.uniforms ) {
            shader->setUniform( it.first.c_str(), it.second );
        }
        shader->setUniform( "pointLight.position", lightPos );
        shader->setUniform( "pointLight.intensity", lightIntensity );
        shader->setUniform( "pointLight.attenuation", attenuation );
        shader->setUniform( "pointLight.ambientCoefficient", ambientCoefficient );
        renderable->vao.bind();
        glDrawArrays( GL_TRIANGLES, 0, renderable->vbo->count() / renderable->vao.elementsPerIndex() );
        renderable->vao.unbind();
        shader->stopUsing();
    }

    if ( debug_ ) {
        debugRender_( entities, events, cameraMatrix );
    }
}

void Renderer::setSpecularUniforms_( const math::Vector3f& pos, opengl::Program* p ) {
    p->setUniform( "cameraPosition", pos );
}

void Renderer::debugRender_( ecs::EntityManager& entities, ecs::EventManager& events, const math::Matrix4f& cameraMatrix ) {
    // create rendering state
    auto* shader = context_.shaderManager.get( "basic" );
    auto* vbo = context_.meshManager.get( "data/cube.obj" );

    opengl::VertexArrayObjectFactory factory{ vbo, shader };
    factory.addAttribute( "vertex", 3, GL_FLOAT, GL_FALSE, 24 );    // skip the normals
    auto vao = factory.getVao();

    shader->use();
    shader->setUniform( "color", math::Vector3f{ 1.0f, 0.2f, 0.2f } );
    shader->setUniform( "camera", cameraMatrix );
    // loop over bounding boxes
    for ( ecs::Entity entity: entities.join< component::Transform, component::BoundingBox >() ) {
        auto t = entity.component< component::Transform >();
        auto bb = entity.component< component::BoundingBox >();
        // calculate the center of each bounding box
        math::Vector3f min = t->scale.hadamard( bb->min );
        math::Vector3f max = t->scale.hadamard( bb->max );
        math::Vector3f center = 0.5f * ( min + max );
        math::Vector3f scale{ max.x - min.x, max.y - min.y, max.z - min.z };
        math::Matrix4f S = math::Matrix4f::Scale( scale );      // scale to current model dimensions
        math::Matrix4f T1 = math::Matrix4f::Translation( center );  // translate to model coordinates
        math::Matrix4f R = math::Matrix4f::Rotation( t->rotation ); // rotate to world coordinates
        math::Matrix4f T2 = math::Matrix4f::Translation( t->position ); // translate to world coordinates
        math::Matrix4f TRS = T2 * R * T1 * S;

        shader->use();
        shader->setUniform( "model", TRS );
        vao.bind();
        glDrawArrays( GL_LINES, 0, vbo->count() / vao.elementsPerIndex() );
        vao.unbind();
    }
    shader->stopUsing();
}

}
}
