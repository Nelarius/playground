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
    : System< RenderSystem >(),
    cameraEntity_{},
    lightEntity_{},
    defaultProjection_{},
    context_{ context },
    debug_{ false } {
    defaultProjection_ = math::Matrix4f::perspective(70.0f, 1.5f, 0.1f, 100.0f);
}

void RenderSystem::configure(ecs::EventManager& events) {
    events.subscribe< ecs::ComponentAssignedEvent<component::Camera> >(*this);
    events.subscribe< ecs::ComponentAssignedEvent<component::PointLight> >(*this);
}

void RenderSystem::receive(const ecs::ComponentAssignedEvent< component::Camera >& event) {
    cameraEntity_ = event.entity;
}

void RenderSystem::receive(const ecs::ComponentAssignedEvent< component::PointLight >& event) {
    lightEntity_ = event.entity;
}

void RenderSystem::update(
    ecs::EntityManager& entities,
    ecs::EventManager& events,
    float dt
    ) {
    math::Matrix4f cameraMatrix{ defaultProjection_ };
    math::Vec3f cameraPos{};
    math::Vec3f lightPos{};
    math::Vec3f lightIntensity{};
    float attenuation = 1.0f;
    float ambientCoefficient = 0.5f;

    if (cameraEntity_.isValid()) {
        float aspectRatio = float(context_.window->width()) / context_.window->height();
        auto transform = cameraEntity_.component< component::Transform >();
        auto view = math::Matrix4f::translation(transform->position)
            * math::Matrix4f::rotation(transform->rotation)
            * math::Matrix4f::scale(transform->scale);
        auto camera = cameraEntity_.component< component::Camera >();
        auto proj = math::Matrix4f::perspective(
            camera->verticalFov,
            aspectRatio,
            camera->nearPlane,
            camera->farPlane
            );
        cameraMatrix = proj * view.inverse();
    }

    if (lightEntity_.isValid()) {
        lightPos = lightEntity_.component< component::Transform >()->position;
        lightIntensity = lightEntity_.component< component::PointLight >()->intensity;
        attenuation = lightEntity_.component< component::PointLight >()->attenuation;
        ambientCoefficient = lightEntity_.component< component::PointLight >()->ambientCoefficient;
    }

    {
        opengl::Program* shader = context_.shaderManager.get("specular");
        opengl::UseProgram use(*shader);

        /*
        * Then, iterate over renderables
        */
        for (ecs::Entity entity : entities.join< component::Transform, component::Renderable>()) {
            auto renderable = entity.component< component::Renderable>();
            auto transform = entity.component< component::Transform>();
            shader->setUniform(
                "model",
                math::Matrix4f::translation(transform->position)
                * math::Matrix4f::rotation(transform->rotation)
                * math::Matrix4f::scale(transform->scale)
                );
            shader->setUniform("camera", cameraMatrix);
            for (const auto& it : renderable->material.uniforms) {
                shader->setUniform(it.first.c_str(), it.second);
            }
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

void RenderSystem::setSpecularUniforms_(const math::Vec3f& pos, opengl::Program* p) {
    p->setUniform("cameraPosition", pos);
}

}
}