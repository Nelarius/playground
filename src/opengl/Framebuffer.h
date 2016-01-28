
#pragma once

#include "opengl/Texture.h"
#include <GL/glew.h>

namespace pg {
namespace opengl {

/**
 * @class Framebuffer
 * @data 11/07/15
 * @brief A read-write framebuffer.
 * It is possible for a framebuffer object to be read-only or draw-only.
 * For now, this class will be bound to both, GL_FRAMEBUFFER. See the documentation
 * of glBindFramebuffer for more.
 */
class Framebuffer {
public:
    Framebuffer();
    Framebuffer(const Framebuffer&) = delete;
    Framebuffer(Framebuffer&&) = delete;
    Framebuffer& operator=(const Framebuffer&) = delete;
    Framebuffer& operator=(Framebuffer&&) = delete;
    ~Framebuffer();

    /**
     * @brief Attach a buffer to this frame buffer object
     * @param attachment The attachment type.
     * The attachment type has to be one of the following: GL_COLOR_ATTACHMENT,
     * GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT, GL_DEPTH_STENCIL_ATTACHMENT
     */
    void attach(const Texture& texture, GLenum attachment);

    void bind();
    void unbind();

    GLuint object() const;

private:
    GLint   old_;
    GLuint  object_;
};

}
}