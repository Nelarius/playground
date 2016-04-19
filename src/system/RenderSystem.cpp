#include "system/RenderSystem.h"
#include "system/Material.h"
#include "opengl/Use.h"
#include "opengl/VertexArrayObjectFactory.h"
#include "component/Include.h"
#include "utils/Log.h"
#include "utils/Assert.h"

#include <cmath>
#include <utility>

namespace {

const float DegreesToRads = 3.141592653f / 180.0f;
const float RadsToDegrees = 180.0f / 3.141592653f;
const float Pi = 3.141592653f;

}

namespace pg {
namespace system {

RenderSystem::RenderSystem(Context& context)
    : System(),
    cameraEntity_{},
    lightEntity_{},
    defaultProjection_{},
    defaultLight_{},
    defaultState_{},
    context_{ context },
    debug_{ false } {
    defaultProjection_ = Matrix4f::perspective(70.0f, 1.5f, 0.1f, 100.0f);
}

void RenderSystem::configure(ecs::EventManager& events) {
    events.subscribe< ecs::ComponentAssignedEvent<Camera> >(*this);
    events.subscribe< ecs::ComponentAssignedEvent<PointLight> >(*this);
}

void RenderSystem::receive(const ecs::ComponentAssignedEvent< Camera >& event) {
    cameraEntity_ = event.entity;
}

void RenderSystem::receive(const ecs::ComponentAssignedEvent< PointLight >& event) {
    lightEntity_ = event.entity;
}

void RenderSystem::update(
    ecs::EntityManager& entities,
    ecs::EventManager& events,
    float dt
    ) {
    Matrix4f cameraMatrix{ defaultProjection_ };
    Vec3f cameraPos{};
    Vec3f lightPos{};
    Vec3f lightIntensity{};
    float attenuation = 1.0f;
    float ambientCoefficient = 0.5f;

    if (cameraEntity_.isValid()) {
        float aspectRatio = float(context_.window->width()) / context_.window->height();
        auto transform = cameraEntity_.component< Transform >();
        auto view = Matrix4f::translation(transform->position)
            * Matrix4f::rotation(transform->rotation)
            * Matrix4f::scale(transform->scale);
        auto camera = cameraEntity_.component< Camera >();
        auto proj = Matrix4f::perspective(
            camera->verticalFov,
            aspectRatio,
            camera->nearPlane,
            camera->farPlane
            );
        cameraMatrix = proj * view.inverse();
    }

    if (lightEntity_.isValid()) {
        lightPos = lightEntity_.component< Transform >()->position;
        lightIntensity = lightEntity_.component< PointLight >()->intensity;
        attenuation = lightEntity_.component< PointLight >()->attenuation;
        ambientCoefficient = lightEntity_.component< PointLight >()->ambientCoefficient;
    }

    {
        opengl::Program* shader = context_.shaderManager.get("specular");
        opengl::UseProgram use(*shader);

        /*
        * Then, iterate over renderables
        */
        for (ecs::Entity entity : entities.join< Transform, Renderable>()) {
            auto renderable = entity.component< Renderable>();
            auto transform = entity.component< Transform>();
            shader->setUniform(
                "model",
                Matrix4f::translation(transform->position)
                * Matrix4f::rotation(transform->rotation)
                * Matrix4f::scale(transform->scale)
                );
            shader->setUniform("camera", cameraMatrix);
            /*for (const auto& it : renderable->material.uniforms) {
                shader->setUniform(it.first.c_str(), it.second);
            }*/
            shader->setUniform("shininess", renderable->material.shininess);
            shader->setUniform("base", renderable->material.baseColor);
            shader->setUniform("ambient", renderable->material.ambientColor);
            shader->setUniform("specularColor", renderable->material.specularColor);
            shader->setUniform("pointLight.position", lightPos);
            shader->setUniform("pointLight.intensity", lightIntensity);
            shader->setUniform("pointLight.attenuation", attenuation);
            shader->setUniform("pointLight.ambientCoefficient", ambientCoefficient);
            renderable->vao.bind();
            glDrawArrays(GL_TRIANGLES, 0, renderable->vbo->count() / renderable->vao.elementsPerIndex());
            renderable->vao.unbind();
        }
    }
}

void RenderSystem::setSpecularUniforms_(const Vec3f& pos, opengl::Program* p) {
    p->setUniform("cameraPosition", pos);
}

CameraInfo RenderSystem::activeCameraInfo() const {
    PG_ASSERT(cameraEntity_.isValid());
    auto camera = cameraEntity_.component<Camera>();
    auto transform = cameraEntity_.component<Transform>();
    float aspectRatio = float(context_.window->width()) / context_.window->height();
    Frustumf frustum{ camera->verticalFov, aspectRatio, camera->nearPlane, camera->farPlane };
    return CameraInfo{ frustum, transform->position, transform->rotation };
}

}
}
