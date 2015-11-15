#include "system/Renderer.h"
#include "system/Material.h"
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
    context_{ context } {
    defaultProjection_ = math::Matrix4f::Perspective( 70.0f, 1.5f, 0.1f, 100.0f );
}

void Renderer::configure( ecs::EventManager& events ) {
    events.subscribe< ecs::ComponentAssignedEvent<component::Camera> >( *this );
    events.subscribe< ecs::ComponentAssignedEvent<component::PointLight> >( *this );
}

void Renderer::receive( const ecs::ComponentAssignedEvent< component::Camera >& event ) {
    cameraEntity_ = event.entity;
}

void Renderer::receive( const ecs::ComponentAssignedEvent< component::PointLight >& event ) {
    lightEntity_ = event.entity;
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
}

void Renderer::setSpecularUniforms_( const math::Vector3f& pos, opengl::Program* p ) {
    p->setUniform( "cameraPosition", pos );
}

}
}
