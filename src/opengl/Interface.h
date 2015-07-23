#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include "opengl/Shader.h"  // contains the include for <memory>
#include <string>
#include <memory>

//an interface with the file system
//load resources, etc.
namespace ce {

/// \brief Load a shader from a file.
/// \return Pointer to the shader.
std::unique_ptr<Shader> ShaderFromFile( const std::string& file, GLenum shaderType );

}

#endif // INTERFACE_H_INCLUDED
