
#pragma once

#include "utils/Log.h"
#include <GL/glew.h>
#include <cstdlib>  // for std::size_t 

namespace pg {
namespace opengl {

inline GLenum GetBindingTarget( GLenum type ) {
    switch( type ) {
        case GL_ARRAY_BUFFER:               return GL_ARRAY_BUFFER_BINDING;
        case GL_ELEMENT_ARRAY_BUFFER:       return GL_ELEMENT_ARRAY_BUFFER_BINDING;
        // for some reason, these are not avialable on ubuntu
        //case GL_COPY_READ_BUFFER:           return GL_COPY_READ_BUFFER_BINDING;
        //case GL_COPY_WRITE_BUFFER:          return GL_COPY_WRITE_BUFFER_BINDING;
        case GL_PIXEL_UNPACK_BUFFER:        return GL_PIXEL_UNPACK_BUFFER_BINDING;
        case GL_PIXEL_PACK_BUFFER:          return GL_PIXEL_PACK_BUFFER_BINDING;
        case GL_QUERY_BUFFER:               return GL_QUERY_BUFFER_BINDING;
        case GL_TRANSFORM_FEEDBACK_BUFFER:  return GL_TRANSFORM_FEEDBACK_BINDING;
        case GL_UNIFORM_BUFFER:             return GL_UNIFORM_BUFFER_BINDING;
        case GL_DRAW_INDIRECT_BUFFER:       return GL_DRAW_INDIRECT_BUFFER_BINDING;
        case GL_ATOMIC_COUNTER_BUFFER:      return GL_ATOMIC_COUNTER_BUFFER_BINDING;
        case GL_DISPATCH_INDIRECT_BUFFER:   return GL_DISPATCH_INDIRECT_BUFFER_BINDING;
        case GL_SHADER_STORAGE_BUFFER:      return GL_SHADER_STORAGE_BUFFER_BINDING;
        case GL_TEXTURE_BUFFER:             return GL_TEXTURE_BINDING_BUFFER;
        case GL_TEXTURE_1D:                 return GL_TEXTURE_BINDING_1D;
        case GL_TEXTURE_2D:                 return GL_TEXTURE_BINDING_2D;
        case GL_TEXTURE_3D:                 return GL_TEXTURE_BINDING_3D;
        case GL_TEXTURE_1D_ARRAY:           return GL_TEXTURE_BINDING_1D_ARRAY;
        case GL_TEXTURE_2D_ARRAY:           return GL_TEXTURE_BINDING_2D_ARRAY;
        case GL_TEXTURE_RECTANGLE:          return GL_TEXTURE_BINDING_RECTANGLE;
        case GL_TEXTURE_CUBE_MAP:           return GL_TEXTURE_BINDING_CUBE_MAP;
        case GL_TEXTURE_CUBE_MAP_ARRAY:     return GL_TEXTURE_BINDING_CUBE_MAP_ARRAY;
        case GL_TEXTURE_2D_MULTISAMPLE:     return GL_TEXTURE_BINDING_2D_MULTISAMPLE;
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:   return GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY;
        case GL_FRAMEBUFFER:                return GL_FRAMEBUFFER_BINDING;
        case GL_DRAW_FRAMEBUFFER:           return GL_DRAW_FRAMEBUFFER_BINDING;
        case GL_READ_FRAMEBUFFER:           return GL_READ_FRAMEBUFFER_BINDING;
        default:    LOG(pg::LogLevel::Error) << "An unknown GL binding target was queried!"; return 0;
    }
}

/**
 * @brief 
 * @param type The type to be queried.
 * @return Returns the size of hte type in bytes.
 * Valid OpenGL types are GL_FLOAT, GL_DOUBLE, GL_HALF_FLOAT, GL_BYTE, GL_UNSIGNED_BYTE,
 * GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_FIXED.
 */
inline std::size_t SizeOfGlType( GLenum type ) {
    switch ( type ) {
        case GL_FLOAT:          return 4u;
        case GL_DOUBLE:         return 8u;
        case GL_HALF_FLOAT:     return 2u;
        case GL_BYTE:           return 1u;
        case GL_UNSIGNED_BYTE:  return 1u;
        case GL_SHORT:          return 2u;
        case GL_UNSIGNED_SHORT: return 2u;
        case GL_INT:            return 4u;
        case GL_UNSIGNED_INT:   return 4u;
        case GL_FIXED:          return 4u;
        default:    LOG(pg::LogLevel::Error) << "An unknown GL type was queried!"; return 0;
    }
}

}   // opengl 
}   // pg
