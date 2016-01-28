
#pragma once

#include "opengl/VertexAttribute.h"
#include <GL/glew.h>
#include <vector>
#include <cstdlib>

namespace pg {
namespace opengl {

/**
 * @class VertexArrayObject
 * @author Johann
 * @date 05/26/15
 * @file VertexArrayObject.h
 * @brief A wrapper class for OpenGL vertex array objects.
 * Objects of this class can be created conveniently using VertexArrayObjectFactory.
 */
class VertexArrayObject {
public:
    VertexArrayObject() = delete;
    /**
     * @brief Construct with the number of elements per index.
     * @param elements
     */
    explicit VertexArrayObject(int elements = 0);
    VertexArrayObject(const VertexArrayObject&);
    VertexArrayObject& operator=(const VertexArrayObject&);
    VertexArrayObject(VertexArrayObject&&);
    VertexArrayObject& operator=(VertexArrayObject&&);
    ~VertexArrayObject();

    /**
     * @brief Get the underlying OpenGL object handle.
     */
    GLuint object() const;

    /**
     * @brief Get the number of array elements in one index.
     */
    int elementsPerIndex() const;

    void bind();
    void unbind();

private:
    void retain_();
    void release_();

    GLuint      object_{ 0u };
    GLint       old_{ 0 };
    int         elements_{ 0 };
    unsigned*   refCount_{ nullptr };
};

}   // opengl 
}   // pg
