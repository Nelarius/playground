#include "opengl/VertexArrayObjectFactory.h"
#include "opengl/Enum.h"
#include "opengl/BufferObject.h"
#include "opengl/Program.h"
#include "utils/Log.h"
#include "utils/Assert.h"

using ce::VertexArrayObjectFactory; 

VertexArrayObjectFactory::VertexArrayObjectFactory( ce::BufferObject* buffer, ce::Program* program )
:   attributeQueue_(),
    standardAttributeQueue_(),
    buffer_( buffer ),
    program_( program ) {}

void VertexArrayObjectFactory::addAttribute(
    const std::string& attrib,
    GLint size, 
    GLenum type,
    GLboolean normalized, 
    GLsizei stride, 
    std::size_t offset ) {
    if ( !standardAttributeQueue_.empty() ) {
        LOG( ce::LogLevel::Error ) << "Adding custom and standard attributes to the same vertex array object is prohibited!";
        return;
    }
    attributeQueue_.emplace_back(
        attrib,
        size,
        type,
        normalized,
        stride,
        offset
    );
}

void VertexArrayObjectFactory::addStandardAttribute( ce::VertexAttribute attribute ) {
    if ( !attributeQueue_.empty() ) {
        LOG( ce::LogLevel::Error ) << "Adding standard and custom attributes to the same vertex array object is prohibited!";
        return;
    }
    GLint program;
    glGetIntegerv( GL_CURRENT_PROGRAM, &program );
    
    switch( attribute ) {
        case ce::VertexAttribute::Vertex: {
            //GLint attrib = glGetAttribLocation( program, "vertex" );
            standardAttributeQueue_.emplace_back(
                "vertex", 3, GL_FLOAT, GL_FALSE, 0, 0u
            );
            break;
        }
        case ce::VertexAttribute::Normal: {
            //GLint attrib = glGetAttribLocation( program, "normal" );
            standardAttributeQueue_.emplace_back(
                "normal", 3, GL_FLOAT, GL_FALSE, 0, 0u
            );
            break;
        }
        case ce::VertexAttribute::Color: {
            //GLint attrib = glGetAttribLocation( program, "color" );
            standardAttributeQueue_.emplace_back(
                "color", 3, GL_FLOAT, GL_FALSE, 0, 0u 
            );
            break;
        }
        case ce::VertexAttribute::UVCoordinate: {
            //GLint attrib = glGetAttribLocation( program, "uvcoordinate" );
            standardAttributeQueue_.emplace_back(
                "uvcoordinate", 2, GL_FLOAT, GL_FALSE, 0, 0u
            );
            break;
        }
        
        default: LOG( ce::LogLevel::Error ) << "Trying to add incorrect attribute to vertex array object.";
    }
}

ce::VertexArrayObject VertexArrayObjectFactory::getVao() {
    buffer_->bind();
    program_->use();
    
    // for custom attributes, the stride may be larger than the actual number
    // of elements used. Hence, we need to get the number of elements per
    // index needs to be deduced from the stride.
    int elements =  standardAttributeQueue_.empty() ? 
                    attributeQueue_[0].stride / ce::SizeOfGlType( attributeQueue_[0].type ) :
                    accumulateAttributes_( standardAttributeQueue_ );
    ce::VertexArrayObject vao{ elements };
    if ( !standardAttributeQueue_.empty() ) {
        vao.bind();
        enableStandardAttributes_();
        standardAttributeQueue_.clear();
        vao.unbind();
    } else if ( !attributeQueue_.empty() ) {
        vao.bind();
        enableAttributes_();
        attributeQueue_.clear();
        vao.unbind();
    }
    buffer_->unbind();
    program_->stopUsing();
    return vao;
}

/*
 * This class creates a vertex attribute pointer for each attribute,
 * and then enables the array.
 * 
 * Does not clear the queue.
 * */
void VertexArrayObjectFactory::enableStandardAttributes_() {
    int strideAccumulator{};
    /*
     * Get the total stride
     * */
    for ( auto& attrib: standardAttributeQueue_ ) {
        strideAccumulator += attrib.size;
    }
    /*
     * enable the attributes
     * */
    std::size_t offsetAccumulator{ 0u };
    for ( auto& attrib: standardAttributeQueue_ ) {
        GLint index = glGetAttribLocation( program_->object(), attrib.name.c_str() );
        ASSERT( index != -1, std::string("VertexArrayObjectFactory::enableAttributes_> invalid attribute location for ") + attrib.name );
        glVertexAttribPointer(
            index,
            attrib.size,
            attrib.type,
            attrib.normalized,
            strideAccumulator * sizeof(float),
            ( const GLvoid* ) ( offsetAccumulator * sizeof(float) )
        );
        offsetAccumulator += attrib.size;
        glEnableVertexAttribArray( index );
        
    }
}

void VertexArrayObjectFactory::enableAttributes_() {
    for ( auto& attrib: attributeQueue_ ) {
        GLint index = glGetAttribLocation( program_->object(), attrib.name.c_str() );
        ASSERT( index != -1, std::string("VertexArrayObjectFactory::enableAttributes_> invalid attribute location for ") + attrib.name );
        glVertexAttribPointer(
            index,
            attrib.size,
            attrib.type,
            attrib.normalized,
            attrib.stride,
            ( const GLvoid* ) attrib.offset
        );
        glEnableVertexAttribArray( index );
    }
}

int VertexArrayObjectFactory::accumulateAttributes_( const std::vector<VertexArrayObjectFactory::Attribute>& attributes ) const {
    int accumulator{ 0 };
    for ( auto& attrib: attributes ) {
        accumulator += attrib.size;
    }
    return accumulator;
}
