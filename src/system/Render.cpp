#include "system/Render.h"
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
    context_{ context }
    {}

void Render::update(
    ecs::EntityManager& entities,
    ecs::EventManager& events,
    float dt 
) {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    float aspectRatio = float( context_.window->width() ) / context_.window->height();
    glm::mat4 cameraMatrix{};
    /*
     * Iterate over cameras here, find the active one
     * */
    for ( ecs::Entity entity: entities.join<component::Transform, component::Camera>() ) {
        auto camera = entity.component<component::Camera>();
        auto transform = entity.component<component::Transform>();
        if ( camera->active ) {
            glm::mat4 view = ModelMatrixFromTransform( transform );
            glm::mat4 proj;
            if ( camera->viewPerspective ) {
                proj = glm::perspective( camera->verticalFov, aspectRatio, camera->nearPlane, camera->farPlane );
            } else {
                // construct orthogonal projection matrix here
            }
            cameraMatrix = proj * glm::inverse( view );
            break;
        }
    }
    
     /*
      * Then, iterate over renderables
      * */
    for ( ecs::Entity entity: entities.join<component::Transform, component::Renderable>() ) {
        auto renderable = entity.component<component::Renderable>();
        auto transform = entity.component<component::Transform>();
        auto shader = renderable->shader;
        shader->use();
        shader->setUniform( 
            "model", 
            ModelMatrixFromTransform(
                transform
            )
        );
        shader->setUniform( "camera", cameraMatrix );
        for ( const auto& it: renderable->uniforms ) {
            shader->setUniform( it.first.c_str(), it.second );
        }
        renderable->vao.bind();
        glDrawArrays( GL_TRIANGLES, 0, renderable->vbo->count() / renderable->vao.elementsPerIndex() );
        renderable->vao.unbind();
        shader->stopUsing();
    }
}

}
}
