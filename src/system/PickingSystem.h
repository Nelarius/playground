#pragma once

#include "component/Include.h"

namespace pg {

struct Context;

namespace ecs {
class EventManager;
class EntityManager;
}

namespace system {

class PickingSystem : public ecs::System<PickingSystem>, public ecs::Receiver {
public:
    PickingSystem() = delete;
    explicit PickingSystem(Context& context);

    void configure(ecs::EventManager&) override;
    void receive(const ecs::ComponentAssignedEvent<component::Camera>&);
    void update(ecs::EntityManager&, ecs::EventManager&, float) override;

    // cast a ray from mouse coordinates
    bool rayCast(ecs::EntityManager&, ecs::EventManager&, int x, int y);

private:
    ecs::Entity cameraEntity_;
    Context&    context_;
};

}
}
