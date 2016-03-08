#include "app/Context.h"
#include "component/Camera.h"
#include "system/PickingSystem.h"
#include "system/Events.h"
#include "math/Geometry.h"
#include "math/Intersection.h"
#include "math/Quaternion.h"
#include "utils/Log.h"

namespace pg {
namespace system {

PickingSystem::PickingSystem(Context& context)
    : System(),
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

ecs::Entity PickingSystem::rayCast(ecs::EntityManager& entities, ecs::EventManager& events, float x, float y) {
    auto camera = cameraEntity_.component<component::Camera>();
    auto cameraTransform = cameraEntity_.component<component::Transform>();
    float aspectRatio = float(context_.window->width()) / context_.window->height();
    math::Frustumf frustum{ camera->verticalFov, aspectRatio, camera->nearPlane, camera->farPlane };
    math::Rayf ray = math::generateCameraRay(cameraTransform->position, cameraTransform->rotation, frustum, x, y);
    math::Rayf testRay = ray;

    events.emit<RenderDebugLine>(ray.origin, ray.direction * 50.f, 5.f);
    bool result = false;

    float smallest = std::numeric_limits<float>::max();
    ecs::Entity target{};

    for (ecs::Entity entity : entities.join<component::Transform, math::AABoxf>()) {
        auto transform = entity.component<component::Transform>();
        auto aabb = entity.component<math::AABoxf>();
        math::Vec3f min = aabb->min.hadamard(transform->scale) + transform->position;
        math::Vec3f max = aabb->max.hadamard(transform->scale) + transform->position;
        math::Vec3f center = 0.5f * (min + max);
        math::Spheref sphere{ center, (center - min).norm() };

        if (math::rayIntersectsSphere(testRay, sphere)) {
            result = math::rayIntersectsAABox(
                ray,
                *aabb,
                transform->position, transform->rotation, transform->scale
                );
            if (ray.t < smallest) {
                smallest = ray.t;
                target = entity;
            }
        }
    }
    return target;
}

}
}
