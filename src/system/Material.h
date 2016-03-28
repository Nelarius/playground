
#pragma once

#include "math/Vector.h"
#include <string>

namespace pg {
namespace system {

enum class MaterialType {
    Ambient,
    Diffuse,
    Specular
};

struct Material {
    math::Vec3f baseColor;
    math::Vec3f ambientColor;
    math::Vec3f specularColor;
    float       shininess;
};

}
}
