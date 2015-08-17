
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

/*
 * Looking something up from a small map is three orders of magnitude
 * slower than looking it up from a vector. 1 000 lookups from a map containing
 * five elements takes 0.1 ms at both the O2 and O3 optimization levels.
 * 
 * This time is equivalent to 7962 fps. Neglibible time cost for small-medium scenes.
 * 
 * unordered_map is slightly faster in look-up times of known keys. It cannot be iterated over 
 * in order.
 * */

/**
 * @class Renderable
 * @author Muszynski Johann M
 * @date 03/18/15
 * @file Renderable.h
 * @brief 
 */
struct Renderable {
    opengl::BufferObject* vbo;
    opengl::Program* shader;
    opengl::VertexArrayObject vao;
    //std::unordered_map<std::string, float>  uniforms;
    system::Material material;
};



}   //namespace component
}   //namespace ce
