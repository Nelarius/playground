#include "opengl/Texture.h"
#include "opengl/BufferObject.h"
#include "opengl/Enum.h"

namespace pg {
namespace opengl {

Texture::Texture(GLenum type)
    : object_{ 0 },
    old_{ 0 },
    type_{ type } {
    glGenTextures(1, &object_);
}

Texture::~Texture() {
    glDeleteTextures(1, &object_);
}

void Texture::setStore(GLenum internalFormat, const BufferObject& object) {
    this->bind();
    glTexBuffer(type_, internalFormat, object.object());
    this->unbind();
}

void Texture::allocateStore(GLenum internalFormat, uint32_t width, uint32_t height) {
    this->bind();
    // for now, only 2d textures are allowed
    // this API will have to change eventually to allow arbitrary textures
    //glTexImage2D( type_,  );
    //these are hardcoded for now
    glTexParameteri(type_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(type_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(type_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(type_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    this->unbind();
}

void Texture::bind() {
    glGetIntegerv(GetBindingTarget(type_), &old_);
    glBindTexture(type_, object_);
}

void Texture::unbind() {
    glBindTexture(type_, old_);
}

GLuint Texture::object() const {
    return object_;
}


}
}