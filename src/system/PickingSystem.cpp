#include "app/Context.h"
#include "component/Camera.h"
#include "system/PickingSystem.h"
#include "system/Events.h"
#include "math/Geometry.h"
#include "math/Intersection.h"
#include "math/Quaternion.h"
#include "utils/Log.h"

namespace {

/*inline pg::math::Ray generateCameraRay(const pg::math::Vec3f& cameraPos, const pg::component::Camera& camera, int x, int y) {
    //
}*/

}

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

ecs::Entity PickingSystem::rayCast(ecs::EntityManager& entities, ecs::EventManager& events, int x, int y) {
    math::Vec3f eye = cameraEntity_.component<component::Transform>()->position;
    float halfFov = 0.5f * cameraEntity_.component<component::Camera>()->verticalFov;
    float ar = float(context_.window->width()) / context_.window->height();
    float near = cameraEntity_.component<component::Camera>()->nearPlane;
    float ntan = near * tan(halfFov);
    math::Vec3f pixelCoord{
        ntan * (float(x) * 2.f / context_.window->width() - 1.f),
        ntan * ar * (1.f - float(y) * 2.f / context_.window->height()),
        -near
    };

    //auto cameraRay = generateCameraRay(eye, *(cameraEntity_.component<component::Camera>().operator->()), x, y);

    auto temp = math::Matrix4f::rotation(cameraEntity_.component<component::Transform>()->rotation) * math::Vec4f(pixelCoord, 1.f);
    pixelCoord = math::Vec3f(temp.x, temp.y, temp.z);
    pixelCoord.normalize();

    events.emit<RenderDebugLine>(eye, pixelCoord * 50.f, 5.f);

    math::Ray ray{ eye, pixelCoord, std::numeric_limits<float>::max() };
    bool result = false;

    float smallest = std::numeric_limits<float>::max();
    ecs::Entity target{};

    for (ecs::Entity entity : entities.join<component::Transform, math::AABox>()) {
        auto transform = entity.component<component::Transform>();
        auto aabb = entity.component<math::AABox>();

        result = math::rayIntersectsAABox(
            ray,
            *(aabb.operator->()),
            transform->position, transform->rotation, transform->scale
            );
        if (ray.t < smallest) {
            smallest = ray.t;
            target = entity;
        }
    }
    return target;
}

}
}
