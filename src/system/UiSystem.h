#pragma once

#include "app/Context.h"
#include "ecs/Include.h"
#include "system/ImGuiRenderer.h"

namespace pg {
namespace system {

class UiSystem : public ecs::System {
public:
    UiSystem() = delete;
    explicit UiSystem(Context& context);
    virtual ~UiSystem() = default;

    void update(ecs::EntityManager&, ecs::EventManager&, float) override;

    void toggleDisplay();

private:
    void ui_(ecs::EventManager&, float);
    bool display_;

};

}   // system
}   // pg

