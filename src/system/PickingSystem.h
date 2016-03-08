#pragma once

#include "component/Include.h"
#include "ecs/Entity.h"

namespace pg {

struct Context;

namespace ecs {
class EventManager;
class EntityManager;
}

namespace system {

class PickingSystem : public ecs::System, public ecs::Receiver {
public:
    PickingSystem() = delete;
    explicit PickingSystem(Context& context);

    void configure(ecs::EventManager&) override;
    void receive(const ecs::ComponentAssignedEvent<component::Camera>&);
    void update(ecs::EntityManager&, ecs::EventManager&, float) override;

    // cast a ray from mouse coordinates
    ecs::Entity rayCast(ecs::EntityManager&, ecs::EventManager&, float x, float y);

private:
    ecs::Entity cameraEntity_;
    Context&    context_;
};

}
}
