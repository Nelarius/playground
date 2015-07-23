#include "opengl/Interface.h"
#include "utils/File.h"

using ce::Shader;

std::unique_ptr<Shader> ce::ShaderFromFile( const std::string& file, GLenum shaderType ) {
    return std::make_unique<Shader>( ce::FileToString(file), shaderType );
    //return Shader::ptr( new Shader( ce::FileToString(file), shaderType ) );
}
