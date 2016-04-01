
#pragma once

#include "opengl/BufferObject.h"
#include "utils/Container.h"
#include "math/Geometry.h"
#include <memory>
#include <unordered_map>
#include <utility>
#include <string>
#include <cstddef>

namespace pg {

/**
 * This class parses mesh objects and creates the corresponding vertex buffer object.
 * Use this class to gain access to the vertex buffer object.
 */
class MeshManager {
public:
    MeshManager() = default;
    ~MeshManager() = default;

    // OpenGL needs to be initialized before this is called
    void initialize();

    /**
     * @param file The mesh file to get.
     */
    opengl::BufferObject*   get(const std::string& file) const;
    /**
    * @param file
    * @returns The bounding box corresponding to the given file name.
    */
    math::AABoxf getBoundingBox(const std::string& file) const;
    /**
     * Remove all the managed mesh objects from GPU and system memory.
     */
    void            clear();
    /**
     * @brief Get the number of elements.
     */
    std::size_t     size() const;

private:
    mutable Container< opengl::BufferObject >                           buffer_{};
    mutable std::unordered_map< std::string, opengl::BufferObject* >    resources_{};
    mutable std::unordered_map< std::string, math::AABoxf >             boxes_{};
};

}
