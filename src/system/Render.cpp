#include "system/Render.h"
#include "system/Material.h"
#include "component/Include.h"
#include "utils/Log.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

namespace {
    
    const float DegreesToRads = 3.141592653f / 180.0f;
    const float RadsToDegrees = 180.0f / 3.141592653f;
    const float Pi = 3.141592653f;
    
    /*
     * These structs correspond to data types in the shaders
     * */
    struct DirectionalLight {
        pg::math::Vector3f direction;
        pg::math::Vector3f intensity;
        float ambientCoefficient;
    };
    
    glm::mat4 ModelMatrixFromTransform( const pg::ecs::ComponentHandle<pg::component::Transform>& t ) {
        /*
         * The composite model matrix is C = TRS
         * */
        glm::mat4 scale = glm::scale( glm::mat4(), glm::vec3( t->scale.x, t->scale.y, t->scale.z ) );
        //glm::mat4 rotate = glm::mat4_cast( t->rotation );
        glm::mat4 rotate = glm::mat4();
        glm::mat4 translate = glm::translate( glm::mat4(), glm::vec3( t->position.x, t->position.y, t->position.z ) );
        return translate * rotate * scale;
    }
}

namespace pg {
namespace system {

Render::Render( Context& context )
:   System<Render>(),
    cameraEntity_{},
    defaultProjection_{},
    context_{ context }
    {}
    
void Render::configure( ecs::EventManager& events ) {
    events.subscribe< CameraAdded >( *this );
}

void Render::receive( const CameraAdded& event ) {
    cameraEntity_ = event.entity;
}

void Render::update(
    ecs::EntityManager& entities,
    ecs::EventManager& events,
    float dt 
) {
    glm::mat4 cameraMatrix{};
    glm::vec3 cameraPos;
    if ( cameraEntity_.isValid() ) {
        float aspectRatio = float( context_.window->width() ) / context_.window->height();
        glm::mat4 view = ModelMatrixFromTransform( cameraEntity_.component< component::Transform >() );
        auto c = cameraEntity_.component< component::Camera >();
        glm::mat4 proj = glm::perspective( c->verticalFov, aspectRatio, c->nearPlane, c->farPlane );
        cameraMatrix = proj * glm::inverse( view );
    }
    
     /*
      * Then, iterate over renderables
      * */
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
        if ( renderable->material.type == MaterialType::Specular ) {
            setSpecularUniforms_( cameraPos, shader );
        }
        renderable->vao.bind();
        glDrawArrays( GL_TRIANGLES, 0, renderable->vbo->count() / renderable->vao.elementsPerIndex() );
        renderable->vao.unbind();
        shader->stopUsing();
    }
}

void Render::setSpecularUniforms_( const glm::vec3& pos, opengl::Program* p ) {
    p->setUniform( "cameraPosition", pos );
}

}
}
