
#pragma once 

#include <unordered_map>
#include <string>

namespace pg {
namespace system {
    
enum class MaterialType {
    Ambient,
    Diffuse,
    Specular
};

struct Material {
    MaterialType                            type;
    std::unordered_map<std::string, float>  uniforms;
};
    
}   // system
}   // pg
