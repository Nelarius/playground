#include "opengl/VertexArrayObject.h"
#include "utils/Assert.h"
#include "utils/Log.h"

namespace pg {
namespace opengl {
VertexArrayObject::VertexArrayObject( int elements )
:   object_( 0u ),
    old_( 0 ),
    elements_( elements ),
    refCount_( nullptr ) {
    glGenVertexArrays( 1, &object_ );  
    refCount_ = new unsigned;
    *refCount_ = 1u;
}

VertexArrayObject::VertexArrayObject( const VertexArrayObject& other )
:   object_( other.object_ ),
    old_( other.old_ ),
    elements_( other.elements_ ),
    refCount_( other.refCount_ ) {
    retain_();
}

VertexArrayObject& VertexArrayObject::operator=( const VertexArrayObject& rhs ) {
    release_();
    object_ = rhs.object_;
    old_ = rhs.old_;
    elements_ = rhs.elements_;
    refCount_ = rhs.refCount_;
    retain_();
    return *this;
}

VertexArrayObject::VertexArrayObject( VertexArrayObject&& other )
:   object_( other.object_ ),
    old_( other.old_ ),
    elements_( other.elements_ ),
    refCount_( other.refCount_ ) {
    other.object_ = 0u;
    other.refCount_ = nullptr;
}

VertexArrayObject& VertexArrayObject::operator=( VertexArrayObject&& rhs ) {
    release_();
    object_ = rhs.object_;
    old_ = rhs.old_;
    elements_ = rhs.elements_;
    refCount_ = rhs.refCount_;
    // we don't retain, because we take the object & reference from rhs
    rhs.object_ = 0u;
    rhs.refCount_ = nullptr;
    return *this;
}

VertexArrayObject::~VertexArrayObject() {
    release_();
}

GLuint VertexArrayObject::object() const {
    return object_;
}

int VertexArrayObject::elementsPerIndex() const {
    return elements_;
}

void VertexArrayObject::bind() {
    glGetIntegerv( GL_VERTEX_ARRAY_BINDING, &old_ );
    glBindVertexArray( object_ );
}

void VertexArrayObject::unbind() {
    glBindVertexArray( old_ );
}

void VertexArrayObject::retain_() {
    ASSERT( refCount_ );
    *refCount_ += 1u;
}

void VertexArrayObject::release_() {
    if ( refCount_ ) {
        *refCount_ -= 1u;
        if ( *refCount_ == 0u ) {
            glDeleteVertexArrays( 1, &object_ );
            delete refCount_;
            refCount_ = nullptr;
        }
    }
}



}
}