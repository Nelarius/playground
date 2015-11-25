
#pragma once

#include "opengl/BufferObject.h"
#include "utils/Container.h"
#include "component/BoundingBox.h"
#include <memory>
#include <unordered_map>
#include <utility>
#include <string>
#include <cstddef>

namespace pg {

/**
 * @class MeshManager
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
        opengl::BufferObject*   get( const std::string& file ) const;
        /**
        * @brief 
        * @param file 
        * @returns The bounding box corresponding to the given file name.
        */
        component::BoundingBox  getBoundingBox( const std::string& file ) const;
        /**
         * @brief Clear all elements.
         */
        void            clear();
        /**
         * @brief Get the number of elements.
         */
        std::size_t     size() const;

    private:
        mutable Container< opengl::BufferObject >                           buffer_{};
        mutable std::unordered_map< std::string, opengl::BufferObject* >    resources_{};
        mutable std::unordered_map< std::string, component::BoundingBox >   boxes_{};
};

}

