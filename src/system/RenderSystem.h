
#pragma once

#include "ecs/Include.h"
#include "component/Include.h"
#include "system/Events.h"
#include "app/Context.h"
#include "opengl/Program.h"
#include "math/Vector.h"
#include "math/Quaternion.h"
#include "math/Geometry.h"
#include "opengl/BufferObject.h"
#include <vector>

namespace pg {
namespace system {

struct CameraInfo {
    math::Frustumf frustum;
    math::Vec3f position;
    math::Quatf orientation;
};

class RenderSystem : public ecs::System, public ecs::Receiver {
public:
    RenderSystem() = delete;
    explicit RenderSystem(Context& context);

    void configure(ecs::EventManager&) override;
    void update(ecs::EntityManager&, ecs::EventManager&, float) override;
    void receive(const ecs::ComponentAssignedEvent<component::Camera>&);
    void receive(const ecs::ComponentAssignedEvent<component::PointLight>&);

    CameraInfo activeCameraInfo() const;

private:

    // camera position passed as parameter
    void setSpecularUniforms_(const math::Vec3f&, opengl::Program*);

    // render state entities
    ecs::Entity     cameraEntity_;
    ecs::Entity     lightEntity_;   // for now the world shall have one light
    // render state math
    math::Matrix4f  defaultProjection_;

    Context& context_;
    bool    debug_;
};

}
}
