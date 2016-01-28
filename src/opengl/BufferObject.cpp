#include "opengl/BufferObject.h"
#include "opengl/Enum.h"

namespace pg {
namespace opengl {

BufferObject::BufferObject(GLenum type)
    : type_(type) {
    glGenBuffers(1, &object_);
}

BufferObject::~BufferObject() {
    glDeleteBuffers(1, &object_);
}

void BufferObject::dataStore(GLsizeiptr count, GLsizei elementSize, const GLvoid* data, int usage) {
    size_ = elementSize;
    count_ = count;
    this->bind();
    glBufferData(type_, count_*size_, data, usage);
    this->unbind();
}

void* BufferObject::mapBufferRange(GLintptr offset, GLsizeiptr length, int accessFlag) {
    this->bind();
    return glMapBufferRange(type_, offset, length, accessFlag);
}

void BufferObject::unmapBuffer() {
    glUnmapBuffer(type_);
    this->unbind();
}

void BufferObject::bind() {
    glGetIntegerv(GetBindingTarget(type_), &old_);
    glBindBuffer(type_, object_);
}

void BufferObject::unbind() {
    glBindBuffer(type_, old_);
}

GLuint BufferObject::object() const {
    return object_;
}

GLenum BufferObject::type() const {
    return type_;
}

GLsizei BufferObject::size() const {
    return size_;
}

GLsizeiptr BufferObject::count() const {
    return count_;
}


}   // opengl
}   // pg