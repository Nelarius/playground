
#pragma once

#include "system/Material.h"
#include "opengl/VertexArrayObject.h"
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
    opengl::VertexArrayObject vao;
    //std::unordered_map<std::string, float>  uniforms;
    system::Material material;
};

}   //namespace component
}   //namespace ce
