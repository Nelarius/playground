
#pragma once

#include "opengl/BufferObject.h"
#include "utils/Borrowed.h"
#include "utils/Bundle.h"
#include <memory>
#include <map>
#include <utility>
#include <string>
#include <cstddef>

namespace ce {

typedef Borrowed<BufferObject>  BorrowedBufferObject;

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
        BufferObject*   get( const std::string& file );
        /**
         * @brief Clear all elements.
         */
        void            clear();
        /**
         * @brief Get the number of elements.
         */
        std::size_t     size() const;
        
    private:
        Bundle<BufferObject>                        buffer_;
        std::map<const std::string, BufferObject*>  resources_{};
};

}

