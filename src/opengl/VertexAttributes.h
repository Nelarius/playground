#pragma once

#include "utils/Assert.h"
#include "GL/glew.h"
#include <initializer_list>

namespace pg {
namespace opengl {

enum class AttributeType {
    Float,
    Int,
    Ubyte
};

class VertexAttributes;

/// @brief Descbribes a single shader attribute (such as in vec3 aVertex in glsl).
class Attribute {
public:
    Attribute() = delete;
    Attribute(Attribute&&) = delete;
    Attribute& operator=(Attribute&&) = delete;
    /// Use this constructor if a segment of the buffer data is used in an attribute.
    ///
    /// @param index The index of the generic vertex attribute (the location in glsl).
    /// @param type The type of the attribute
    /// @param elementCount The number of elements of type type contained in the attribute
    Attribute(unsigned int index, AttributeType type, unsigned int elementCount);
    /// Use this constructor if the segment of the buffer contains data that isn't used in an attribute.
    Attribute(AttributeType type, unsigned int elementCount);

    inline bool isUsed() const {
        return isUsed_;
    }

    inline unsigned int index() const {
        PG_ASSERT(isUsed_);
        return index_;
    }

    inline unsigned int elementCount() const {
        return elementCount_;
    }

    unsigned int byteCount() const;

    inline AttributeType type() const {
        return type_;
    }

private:
    const bool            isUsed_;
    const unsigned int    index_;
    const AttributeType   type_;
    const unsigned int    elementCount_;
};

/// This class describes the content of an entire vertex buffer. It must be constructed
/// from individual attributes, in order.
class VertexAttributes {
public:
    VertexAttributes() = default;
    /// In order to obtain the correct vertex attributes, you must pass the correct
    /// attributes in order.
    /// NOTE: remember to bind the corresponding buffer object when you
    /// call the constructor!!
    VertexAttributes(std::initializer_list<Attribute> attributes);
    VertexAttributes(const VertexAttributes&);
    VertexAttributes(VertexAttributes&&);
    VertexAttributes& operator=(const VertexAttributes&);
    VertexAttributes& operator=(VertexAttributes&&);
    ~VertexAttributes();

    void bind();
    void unbind();

    inline unsigned elementsPerIndex() const {
        return elementsPerIndex_;
    }

private:
    void retain_();
    void release_();

    unsigned*   refCount_{ nullptr };
    GLuint      object_{ 0u };
    GLint       previousObject_{ 0u };
    unsigned    elementsPerIndex_{ 0u };
};

}
}
