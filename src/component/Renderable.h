
#pragma once

#include "manager/MeshManager.h"
#include "manager/ShaderManager.h"
#include "opengl/VertexArrayObject.h"
#include "opengl/BufferObject.h"
#include <memory>
#include <unordered_map>
#include <string>

namespace ce {
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
    Renderable() = default;
    ~Renderable() = default;
    Renderable(
        BufferObject* vbo,
        Program* prog,
        const VertexArrayObject& vao,
        const std::unordered_map<std::string, float>& unif
    )
    :   vbo( vbo ),
        shader( prog ),
        vao( vao ),
        uniforms( unif )
        {}
    
    // a borrowed pointer is not managed here
    BufferObject*                           vbo{ nullptr };
    Program*                                shader{ nullptr };
    VertexArrayObject                       vao{ 0 };
    std::unordered_map<std::string, float>  uniforms{ 10 }; // initialize with the number of initial buckets, to save on allocation time
};



}   //namespace component
}   //namespace ce
