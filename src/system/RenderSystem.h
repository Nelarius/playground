
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

using namespace component;
using namespace math;

struct CameraInfo {
    Frustumf frustum;
    Vec3f position;
    Quatf orientation;
};

class RenderSystem : public ecs::System, public ecs::Receiver {
public:
    RenderSystem() = delete;
    explicit RenderSystem(Context& context);

    void configure(ecs::EventManager&) override;
    void update(ecs::EntityManager&, ecs::EventManager&, float) override;
    void receive(const ecs::ComponentAssignedEvent<Camera>&);
    void receive(const ecs::ComponentAssignedEvent<PointLight>&);

    CameraInfo activeCameraInfo() const;

private:

    struct DefaultState {
        Matrix4f         projection;
        DirectionalLight light;
    };

    // camera position passed as parameter
    void setSpecularUniforms_(const Vec3f&, opengl::Program*);

    // render state entities
    ecs::Entity     cameraEntity_;
    ecs::Entity     lightEntity_;   // for now the world shall have one light and it shall be a directional light

    // render state math
    Matrix4f  defaultProjection_;
    DirectionalLight defaultLight_;
    DefaultState defaultState_;

    Context& context_;
    bool    debug_;
};

}
}
