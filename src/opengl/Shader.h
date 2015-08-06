
#pragma once 

#include <GL/glew.h>
#include <memory>

namespace pg {
namespace opengl {
    
/// \brief Contains an OpenGL shader object.
class Shader {
    public:
        Shader() = delete;
        /// \brief Construct a shader from a string source.
        Shader( const std::string& shaderSource, GLenum shaderType );
        ~Shader();

        //make uncopyable for now
        Shader( const Shader& )             = delete;
        Shader( Shader&& )                  = delete;
        Shader& operator=( const Shader& )  = delete;
        Shader& operator=( Shader&& )       = delete;

        /// \brief Get the OpenGL handle.
        GLuint object() const;

    private:
        GLuint object_{ 0u };
};

}
}
