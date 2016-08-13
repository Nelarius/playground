#include "VertexAttributes.h"
#include <vector>

namespace {

int attributeTypeToGlType(pg::opengl::AttributeType type) {
    switch(type) {
    case pg::opengl::AttributeType::Float: return GL_FLOAT;
    case pg::opengl::AttributeType::Int:   return GL_INT;
    case pg::opengl::AttributeType::Ubyte: return GL_UNSIGNED_BYTE;
    default: PG_ASSERT(false); return 0u;
    }
}

unsigned int attributeTypeInBytes(pg::opengl::AttributeType type) {
    switch (type) {
    case pg::opengl::AttributeType::Float:  return 4u;
    case pg::opengl::AttributeType::Int:    return 4u;
    case pg::opengl::AttributeType::Ubyte:  return 1u;
    default: PG_ASSERT(false); return 0u;
    }
}

}

namespace pg {
namespace opengl {

Attribute::Attribute(unsigned int index, AttributeType type, unsigned int attribCount)
    :   isUsed_(true),
        index_(index),
        type_(type),
        elementCount_(attribCount) {}

Attribute::Attribute(AttributeType type, unsigned int attribCount)
    :   isUsed_(false),
        index_(0u),
        type_(type),
        elementCount_(attribCount) {}

unsigned int Attribute::byteCount() const {
    return elementCount_ * attributeTypeInBytes(type_);
}

VertexAttributes::VertexAttributes(std::initializer_list<Attribute> attribs)
    :   refCount_(nullptr),
        object_(0u),
        previousObject_(0u),
        elementsPerIndex_(0u) {

    refCount_ = new unsigned;
    *refCount_ = 1u;

    glGenVertexArrays(1, &object_);
    PG_ASSERT(object_ != 0u);
    bind();
    unsigned int bytes = 0u;
    std::vector<unsigned int> offsets(attribs.size());
    for (const auto& attrib : attribs) {
        offsets.push_back(bytes);
        bytes += attrib.byteCount();
        elementsPerIndex_ += attrib.elementCount();
    }
    int i = 0;
    for (const auto& attrib : attribs) {
        if (attrib.isUsed()) {
            glVertexAttribPointer(
                GLuint(attrib.index()),
                attrib.elementCount(),
                attributeTypeToGlType(attrib.type()),
                GL_FALSE,
                bytes,
                (const void*)offsets[i]
            );
            glEnableVertexAttribArray(attrib.index());
        }
        ++i;
    }
    unbind();
}

VertexAttributes::VertexAttributes(const VertexAttributes& other)
    :   refCount_(other.refCount_),
        object_(other.object_),
        previousObject_(other.previousObject_),
        elementsPerIndex_(other.elementsPerIndex_) {
    retain_();
}

VertexAttributes::VertexAttributes(VertexAttributes&& other)
    :   refCount_(other.refCount_),
        object_(other.object_),
        previousObject_(other.previousObject_),
        elementsPerIndex_(other.elementsPerIndex_) {
    other.refCount_ = nullptr;
    other.object_ = 0u;
    other.previousObject_ = 0u;
    other.elementsPerIndex_ = 0u;
}

VertexAttributes& VertexAttributes::operator=(const VertexAttributes& rhs) {
    release_();
    refCount_ = rhs.refCount_;
    object_ = rhs.object_;
    previousObject_ = rhs.previousObject_;
    elementsPerIndex_ = rhs.elementsPerIndex_;
    retain_();

    return *this;
}

VertexAttributes& VertexAttributes::operator=(VertexAttributes&& rhs) {
    release_();
    refCount_ = rhs.refCount_;
    object_ = rhs.object_;
    previousObject_ = rhs.previousObject_;
    elementsPerIndex_ = rhs.elementsPerIndex_;
    rhs.refCount_ = nullptr;
    rhs.object_ = 0u;
    rhs.previousObject_ = 0u;
    rhs.elementsPerIndex_ = 0u;

    return *this;
}

VertexAttributes::~VertexAttributes() {
    release_();
}

void VertexAttributes::bind() {
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &previousObject_);
    glBindVertexArray(object_);
}

void VertexAttributes::unbind() {
    glBindVertexArray(previousObject_);
}

void VertexAttributes::retain_() {
    PG_ASSERT(*refCount_);
    *refCount_ += 1u;
}

void VertexAttributes::release_() {
    if (refCount_) {
        *refCount_ -= 1u;
        if (*refCount_ == 0u) {
            glDeleteVertexArrays(1, &object_);
            delete refCount_;
            refCount_ = nullptr;
        }
    }
}

}
}
