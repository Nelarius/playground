
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace ce {
namespace component {

struct Transform {
    Transform() = default;
    ~Transform() = default;
    Transform( const glm::vec3& position )
    :   position( position )
        {}
    Transform(
        const glm::vec3& position,
        const glm::quat& rotation,
        const glm::vec3& scale
    )
    :   position( position ),
        rotation( rotation ),
        scale( scale )
        {}
    
    glm::vec3 position{ 0.0f, 0.0f, 0.0f };
    glm::quat rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
    glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
};

}   //namespace component
}   //namespace ce

