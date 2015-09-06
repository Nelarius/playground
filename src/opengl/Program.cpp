#include "opengl/Program.h"
#include "utils/Assert.h"
#include "utils/Exception.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cstdlib>  //for std::exit

namespace pg {
namespace opengl {

Program::Program( const std::vector<std::unique_ptr<Shader>>& shaders ) {
    this->link( shaders );
}

Program::~Program() {
    glDeleteProgram( object_ );
    object_ = 0;
}

void Program::link( const std::vector<std::unique_ptr<Shader>>& shaders ) {
    if ( shaders.size() == 0u ) {
        std::cerr << "No shaders were provided to create the program.\n";
        std::exit( EXIT_SUCCESS );
    }
    
    object_ = glCreateProgram();
    if ( object_ == 0 ) {
        throw PlaygroundException( "glCreateProgram() failed" );
    }

    for ( auto& shader: shaders ) {
        glAttachShader( object_, shader->object() );
    }

    glLinkProgram( object_ );

    //check the linking status
    GLint status;
    glGetProgramiv( object_, GL_LINK_STATUS, &status );
    if ( status == GL_FALSE ) {
        std::string msg("Program linking failure: ");

        GLint infoLogLength;
        glGetProgramiv( object_, GL_INFO_LOG_LENGTH, &infoLogLength );
        char* infoLogStr = new char[ infoLogLength + 1 ];
        glGetProgramInfoLog( object_, infoLogLength, NULL, infoLogStr );
        msg += infoLogStr;
        delete [] infoLogStr;

        glDeleteProgram( object_ );
        object_ = 0;
        throw PlaygroundException( msg );
    }
}

GLuint Program::object() const {
    return object_;
}

GLint Program::attribute( const GLchar* attribName ) const {
    ASSERT( attribName, "error: attribName was NULL" );

    GLint attrib = glGetAttribLocation( object_, attribName );
    //ASSERT( attrib != -1, "error: program attribute not found" );
    ASSERT( attrib != -1, std::string("error: program attribute not found: ") + attribName );

    return attrib;
}

GLint Program::uniform( const GLchar* uniformName ) const {
    ASSERT( uniformName, "error: uniformName was NULL" );

    GLint uniform = glGetUniformLocation( object_, uniformName );
    #ifdef DEBUG
    std::string msg( "error: program uniform " );
    msg += uniformName;
    msg += " not found";
    ASSERT( uniform != -1, msg.c_str() );
    #endif // DEBUG
    return uniform;
}

GLint Program::subroutineUniform( const GLchar* uniformName, GLenum shaderType ) const {
    ASSERT( uniformName, "error: subroutine uniform name was null" );
    GLint uniform = glGetSubroutineUniformLocation( object_, shaderType, uniformName );
    #ifdef DEBUG
    std::string msg( "error: subroutine uniform " );
    msg += uniformName;
    msg += " not found";
    ASSERT( uniform != -1, msg.c_str() );
    #endif  // DEBUG
    return uniform;
}

GLuint Program::subroutineIndex( const GLchar* functionName, GLenum shaderType ) const {
    ASSERT( functionName, "error: subroutine function name was null" );
    GLuint index = glGetSubroutineIndex( object_, shaderType, functionName );
    #ifdef DEBUG
    std::string msg("error: active subroutine " );
    msg += functionName;
    msg += " not found";
    ASSERT( index != GL_INVALID_INDEX, msg.c_str() );
    #endif // DEBUG
    return index;
}

void Program::use() const {
    glUseProgram( object_ );
}

bool Program::isInUse() const {
    GLint currentProgram = 0;
    glGetIntegerv( GL_CURRENT_PROGRAM, &currentProgram );
    return ( currentProgram == (GLint) object_ );
}

void Program::stopUsing() const {
    ASSERT( isInUse(), "error: cannot stop using because program is not in use" );
    glUseProgram( 0 );
}

void Program::setUniform( const GLchar* name, GLint v0 ) const {
    ASSERT( isInUse(), "Program::setUniform(i)> program not in use" );
    glUniform1i( uniform(name), v0 );
}

void Program::setUniform( const GLchar* name, GLint v0, GLint v1 ) const {
    ASSERT( isInUse(), "Program::setUniform(2i)> program not in use" );
    glUniform2i( uniform(name), v0, v1);
}

void Program::setUniform( const GLchar* name, GLint v0, GLint v1, GLint v2 ) const {
    ASSERT( isInUse(), "Program::setUniform(3i)> program not in use" );
    glUniform3i( uniform(name), v0, v1, v2 );
}

void Program::setUniform( const GLchar* name, GLint v0, GLint v1, GLint v2, GLint v3 ) const {
    ASSERT( isInUse(), "Program::setUniform(4i)> program not in use" );
    glUniform4i( uniform(name), v0, v1, v2, v3 );
}

void Program::setUniform( const GLchar* name, GLfloat v0 ) const {
    ASSERT( isInUse(), "Program::setUniform(f)> program not in use" );
    glUniform1f( uniform(name), v0);
}

void Program::setUniform( const GLchar* name, GLfloat v0, GLfloat v1 ) const {
    ASSERT( isInUse(), "Program::setUniform(2f)> program not in use" );
    glUniform2f( uniform(name), v0, v1 );
}

void Program::setUniform( const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2 ) const {
    ASSERT( isInUse(), "Program::setUniform(3f)> program not in use" );
    glUniform3f( uniform(name), v0, v1, v2 );
}

void Program::setUniform( const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3 ) const
{
    ASSERT( isInUse(), "Program::setUniform(4f)> program not in use" );
    glUniform4f( uniform(name), v0, v1, v2, v3 );
}

void Program::setUniform( const GLchar* name, const glm::vec2& v ) const
{
    ASSERT( isInUse(), "Program::setUniform> program not in use" );
    glUniform2fv( uniform(name), 1, glm::value_ptr( v ) );
}

void Program::setUniform( const GLchar* name, const glm::vec3& v ) const
{
    ASSERT( isInUse(), "Program::setUniform> program not in use" );
    glUniform3fv( uniform(name), 1, glm::value_ptr( v ) );
}

void Program::setUniform( const GLchar* name, const glm::vec4& v ) const
{
    ASSERT( isInUse(), "Program::setUniform> program not in use" );
    glUniform4fv( uniform(name), 1, glm::value_ptr( v ) );
}

void Program::setUniform( const GLchar* name, const pg::math::Vector2f& v ) const {
    ASSERT( isInUse(), "Program::setUniform(Vector2f)> program not in use" );
    glUniform2fv( uniform(name), 1, v.data );
}

void Program::setUniform( const GLchar* name, const pg::math::Vector3f& v ) const {
    ASSERT( isInUse(), "Program::setUniform(Vector3f)> program not in use" );
    glUniform3fv( uniform(name), 1, v.data );
}

void Program::setUniform( const GLchar* name, const pg::math::Vector4f& v ) const {
    ASSERT( isInUse(), "Program::setUniform(Vector4f)> program not in use" );
    glUniform4fv( uniform(name), 1, v.data );
}

void Program::setUniform( const GLchar* name, const glm::mat3& m, GLboolean transpose ) const {
    ASSERT( isInUse(), "Program::setUniform> program not in use" );
    glUniformMatrix3fv( uniform(name), 1, transpose, glm::value_ptr( m ) );
}

void Program::setUniform( const GLchar* name, const glm::mat4& m, GLboolean transpose ) const {
    ASSERT( isInUse(), "Program::setUniform> program not in use" );
    glUniformMatrix4fv( uniform(name), 1, transpose, glm::value_ptr( m ) );
}

void Program::setUniform( const GLchar* name, const pg::math::Matrix3f& M ) const {
    ASSERT( isInUse(), "Program::setUniform(Matrix3f)> program not in use" );
    pg::math::CMMatrix3f N { M };
    glUniformMatrix3fv( uniform(name), 1, false, N.data );
}

void Program::setUniform( const GLchar* name, const pg::math::Matrix4f& M ) const {
    ASSERT( isInUse(), "Program::setUniform(Matrix4f)> program not in use" );
    pg::math::CMMatrix4f N { M };
    glUniformMatrix4fv( uniform(name), 1, false, N.data );
}

}   // opengl
}   // pg
