
#pragma once

#include "opengl/VertexArrayObject.h"
#include "opengl/Program.h"
#include "opengl/BufferObject.h"
#include "manager/MeshManager.h"
#include "manager/ShaderManager.h"
#include <vector>
#include <string>
#include <cstdlib>  //for std::size_t

namespace ce {

/**
 * @class VertexArrayObjectFactory
 * @author Johann
 * @date 05/26/15
 * @file VertexArrayObjectFactory.h
 * @brief Use this class to create instances of VertexArrayObject.
 * When using the factory, remember to bind the corresponding BufferObject and Program!
 */
class VertexArrayObjectFactory {
    public:
        VertexArrayObjectFactory() = delete;
        /**
         * @brief Construct the factory with the buffer object and shader program you want to associate with your vertex array object.
         * @param buffer The buffer object to use as the data source.
         * @param shader The shader in which the attributes lie.
         */
        explicit VertexArrayObjectFactory( BorrowedBufferObject buffer, BorrowedProgram shader );
        ~VertexArrayObjectFactory() = default;
        
        /**
         * @brief 
         * @param attrib The name of the attribute in the shader.
         * @param size The number of components in the attribute
         * @param type The type of each component
         * @param normalized Specify whether the components should be normalized (GL_TRUE) or not (GL_FALSE).
         * @param stride The byte offset of consecutive attributes. Assumes tightly packed data if this is zero.
         * @param offset The pointer offset in bytes to this attribute's location within the buffer.
         * Do not use this method and addStandardAttribute for the same vao! Incorrect results will follow.
         */
        void addAttribute(
            const std::string& attrib, GLint size, GLenum type,
            GLboolean normalized, GLsizei stride, std::size_t offset = 0u
        );
        
        /**
         * @brief Add a standard attribute such as vertex, normal, or color.
         * @param attribute The standard attribute to add.
         * Do not use this method and addAttribute for the same vao! Incorrect results will follow.
         * The corresponding shader attributes must be called the following:
         * VertexAttribute::Vertex: vertex
         * VertexAttribute::Normal: normal
         * VertexAttribute::Color: color
         * VertexAttribute::UVCoordinate: uvcoordinate 
         * Do not use this method if the buffer will contain unused elements.
         * This method has no way of figuring out whether an element is unused or not,
         * and the stride will be incorrect. Use addAttribute instead to specify a 
         * custom stride.
         */
        void addStandardAttribute( VertexAttribute attribute );
        
        /**
         * @brief Construct a vertex array object after adding attributes to it.
         * @return The constructed object. 
         */
        VertexArrayObject getVao();
        
    private:
        struct Attribute {
            std::string     name;
            GLint           size;
            GLenum          type;
            GLboolean       normalized;
            GLsizei         stride;
            std::size_t     offset;
            
            Attribute() = default;
            Attribute(
                const std::string& _name,
                int _size,
                GLenum _type,
                bool norm,
                GLsizei _stride,
                std::size_t _offset
            )
            :   name( _name ),
                size( _size ),
                type( _type ),
                normalized( norm ),
                stride( _stride ),
                offset( _offset )
                {}
        };
        
        void enableStandardAttributes_();
        void enableAttributes_();
        int  accumulateAttributes_( const std::vector<Attribute>& ) const;
        
        std::vector<Attribute>  attributeQueue_{};
        std::vector<Attribute>  standardAttributeQueue_{};
        BorrowedBufferObject    buffer_;
        BorrowedProgram         program_;

};

}

