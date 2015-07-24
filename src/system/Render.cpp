#include "system/Render.h"
#include "component/Include.h"
#include "utils/Log.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

using ce::system::Render;

namespace {
    
    const float DegreesToRads = 3.141592653f / 180.0f;
    const float RadsToDegrees = 180.0f / 3.141592653f;
    const float Pi = 3.141592653f;
    
    glm::mat4 ModelMatrixFromTransform( const ex::ComponentHandle<ce::component::Transform>& t ) {
        /*
         * The composite model matrix is C = TRS
         * */
        glm::mat4 scale = glm::scale( glm::mat4(), t->scale );
        glm::mat4 rotate = glm::mat4_cast( t->rotation );
        glm::mat4 translate = glm::translate( glm::mat4(), t->position );
        return translate * rotate * scale;
    }
    
}

Render::Render( ce::Context& context )
:   context_{ context }
    {}

void Render::update(
    ex::EntityManager& entities,
    ex::EventManager& events,
    ex::TimeDelta dt 
) {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    float aspectRatio = float( context_.window->width() ) / context_.window->height();
    glm::mat4 cameraMatrix{};
    /*
     * Iterate over cameras here, find the active one
     * */
    ex::ComponentHandle<ce::component::Camera>    camera;
    ex::ComponentHandle<ce::component::Transform> transform;
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
    for ( ex::Entity entity: entities.entities_with_components( transform, camera ) ) {
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
    #pragma GCC diagnostic pop
    
     /*
      * Then, iterate over renderables
      * */
    ex::ComponentHandle<ce::component::Renderable> renderable;
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
    for ( ex::Entity entity: entities.entities_with_components( transform, renderable ) ) {
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
    #pragma GCC diagnostic pop
    
}

