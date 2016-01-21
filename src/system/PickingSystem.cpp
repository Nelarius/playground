#include "app/Context.h"
#include "system/PickingSystem.h"
#include "system/Events.h"
#include "ecs/Include.h"
#include "math/Geometry.h"
#include "math/Intersection.h"
#include "math/Quaternion.h"
#include "utils/Log.h"

namespace pg {
namespace system {

PickingSystem::PickingSystem(Context& context)
    : System<PickingSystem>(),
    cameraEntity_{},
    context_{ context } {}

void PickingSystem::configure(ecs::EventManager& events) {
    events.subscribe<ecs::ComponentAssignedEvent<component::Camera>>(*this);
}

void PickingSystem::receive(const ecs::ComponentAssignedEvent<component::Camera>& event) {
    cameraEntity_ = event.entity;
}

void PickingSystem::update(ecs::EntityManager& entities, ecs::EventManager& events, float dt) {
    // do nothing
}

bool PickingSystem::rayCast(ecs::EntityManager& entities, ecs::EventManager& events, int x, int y) {
    // ray calculations go here

    math::Vector3f eye = cameraEntity_.component<component::Transform>()->position;
    float halfFov = 0.5f * cameraEntity_.component<component::Camera>()->verticalFov;
    float ar = float(context_.window->width()) / context_.window->height();
    float near = cameraEntity_.component<component::Camera>()->nearPlane;
    float ntan = near * tan(halfFov);
    math::Vector3f pixelCoord{
        ntan * (float(x) * 2.f / context_.window->width() - 1.f),
        ntan * ar * (1.f - float(y) * 2.f / context_.window->height()),
        near
    };
    pixelCoord = cameraEntity_.component<component::Transform>()->rotation.rotate(math::Vector4f(pixelCoord, 0.f));
    pixelCoord.normalize();

    events.emit<RenderDebugLine>(eye, pixelCoord * 50.f, 5.f);

    math::Ray ray{ eye, pixelCoord, std::numeric_limits<float>::max()};
    bool result = false;

    for (ecs::Entity entity : entities.join<component::Transform, math::AABox>()) {
        auto transform = entity.component<component::Transform>();
        auto aabb = entity.component<math::AABox>();
        math::Matrix4f TRS =
            math::Matrix4f::Translation(transform->position)
            * math::Matrix4f::Rotation(transform->rotation)
            * math::Matrix4f::Scale(transform->scale);
        result = math::rayIntersectsAABox(ray, *(aabb.operator->()), TRS);
    }
    return result;
}

}
}
