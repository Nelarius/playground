#pragma once

#include "ecs/Include.h"
#include "component/Include.h"
#include "math/Geometry.h"
#include "system/Events.h"
#include "opengl/BufferObject.h"
#include <vector>
#include <utility>

namespace pg {

struct Context;

namespace system {

class DebugRenderSystem : public ecs::System, public ecs::Receiver {
public:
    DebugRenderSystem() = delete;
    explicit DebugRenderSystem(Context& context);
    virtual ~DebugRenderSystem();
    void configure(ecs::EventManager&) override;
    void update(ecs::EntityManager&, ecs::EventManager&, float) override;
    void receive(const ecs::ComponentAssignedEvent<component::Camera>&);
    void receive(const ShowDebugLines&);
    void receive(const ShowBoundingBoxes&);
    void receive(const ShowDebugBoxes&);
    void receive(const RenderDebugLine&);
    void receive(const RenderDebugBox&);
    void addDebugLine(const RenderDebugLine&);
    void addDebugBox(const RenderDebugBox&);

private:
    template<typename T>
    void updateTransientElements_(std::vector<float>& lifeTimes, std::vector<T>& elements, float dt) {
        if (lifeTimes.size() != 0u) {
            for (auto i = lifeTimes.size() - 1u; i-- > 0u;) {
                lifeTimes[i] -= dt;
                if (lifeTimes[i] < 0.f) {
                    std::swap(lifeTimes[i], lifeTimes.back());
                    lifeTimes.pop_back();
                    std::swap(elements[i], elements.back());
                    elements.pop_back();
                }
            }
        }
    }

    Context&                    context_;
    ecs::Entity                 cameraEntity_;
    math::Matrix4f              defaultProjection_;

    // lines
    std::vector<math::Linef>    staticDebugLines_;
    std::vector<math::Linef>    transientDebugLines_;
    std::vector<float>          lineLifeTimes_;

    // boxes
    std::vector<RenderDebugBox> staticDebugBoxes_;
    std::vector<RenderDebugBox> transientDebugBoxes_;
    std::vector<float>          boxLifeTimes_;

    opengl::BufferObject        lineBuffer_;
    opengl::VertexAttributes    lineBufferArray_;

    bool    showLines_;
    bool    showBoundingBoxes_;
    bool    showDebugBoxes_;
    GLuint  cubeVbo_;
    GLuint  cubeVao_;
};

}
}
