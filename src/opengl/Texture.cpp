#include "opengl/Texture.h"
#include "opengl/BufferObject.h"
#include "opengl/Enum.h"

namespace pg {
namespace opengl {
    

Texture::Texture( GLenum type )
    :   object_( 0 ),
        old_( 0 ),
        type_( type ) {
    glGenTextures( 1, &object_ );
}

Texture::~Texture() {
    glDeleteTextures( 1, &object_ );
}

void Texture::setStore( GLenum internalFormat, const BufferObject& object ) {
    this->bind();
    glTexBuffer( type_, internalFormat, object.object() );
    this->unbind();
}

void Texture::bind() {
    glGetIntegerv( GetBindingTarget( type_ ), &old_ );
    glBindTexture( type_, object_ );
}

void Texture::unbind() {
    glBindTexture( type_, old_ );
}

GLuint Texture::object() const {
    return object_;
}


}
}