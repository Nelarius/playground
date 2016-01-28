#include "opengl/Framebuffer.h"

namespace pg {
namespace opengl {

Framebuffer::Framebuffer()
    : old_{ 0u },
    object_{ 0u } {
    glGenFramebuffers(1, &object_);
}

Framebuffer::~Framebuffer() {
    glDeleteFramebuffers(1, &object_);
}

void Framebuffer::bind() {
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_);
    glBindFramebuffer(GL_FRAMEBUFFER, object_);
}

void Framebuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, old_);
}

void Framebuffer::attach(const Texture& t, GLenum attachment) {
    bind();
    glFramebufferTexture(GL_FRAMEBUFFER, attachment, t.object(), 0);
    unbind();
}

GLuint Framebuffer::object() const {
    return object_;
}

}
}