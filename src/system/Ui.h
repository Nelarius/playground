#pragma once

#include "app/Context.h"
#include "ecs/Include.h"
#include "system/ImGuiRenderer.h"

namespace pg {
namespace system {

class Ui: public ecs::System<Ui>, public ImGuiRenderer {
    public:
        Ui() = delete;
        explicit Ui( Context& context );
        virtual ~Ui() = default;

        void update( ecs::EntityManager&, ecs::EventManager&, float ) override;

        void toggleDisplay();

    private:
        void ui_( ecs::EventManager&, float );
        bool display_;

};

}   // system
}   // pg

