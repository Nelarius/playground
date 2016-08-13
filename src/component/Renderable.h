
#pragma once

#include "system/Material.h"
#include "opengl/VertexAttributes.h"
#include "opengl/BufferObject.h"
#include "opengl/Program.h"
#include <memory>
#include <unordered_map>
#include <string>

namespace pg {
namespace component {

struct Renderable {
    opengl::BufferObject* vbo;
    opengl::Program* shader;
    opengl::VertexAttributes attributes;
    system::Material material;
};

}
}
