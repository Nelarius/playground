
#pragma once

#include "opengl/BufferObject.h"
#include "utils/Borrowed.h"
#include "utils/Container.h"
#include <memory>
#include <map>
#include <utility>
#include <string>
#include <cstddef>

namespace pg {

/**
 * @class MeshManager
 * @author Johann
 * @date 05/14/15
 * @file MeshManager.h
 * @brief This class manages the memory of mesh objects, stored internally as VertexBuffer objects.
 * This class parses mesh objects and creates the corresponding vertex buffer object.
 * Use this class to gain access to the vertex buffer object.
 */
class MeshManager {
    public:        
        MeshManager() = default;
        ~MeshManager() = default;        
        
        /**
         * @brief 
         * @param file The mesh file to get.
         */
        opengl::BufferObject*   get( const std::string& file );
        /**
         * @brief Clear all elements.
         */
        void            clear();
        /**
         * @brief Get the number of elements.
         */
        std::size_t     size() const;
        
    private:
        Container<opengl::BufferObject>                     buffer_{};
        std::map<const std::string, opengl::BufferObject*>  resources_{};
};

}

