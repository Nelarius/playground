
#pragma once 

#include "opengl/Shader.h"
#include "math/Vector.h"
#include "math/Matrix.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace ce {

/// @brief A wrapper around an OpenGL shader object.
/// 
/// The shader can be linked at construction time from a vector of individually compiled shader objects,
/// or at a later time by calling the link-method.
class Program {
    public:
        Program() = delete;
        /// @brief Construct the program from a vector of shader objects
        Program( const std::vector<std::unique_ptr<Shader>>& shaders );
        ~Program();

        //make uncopyable for now
        Program( const Program& )               = delete;
        Program( Program&& )                    = delete;
        Program& operator=( const Program& )    = delete;
        Program& operator=( Program&& )         = delete;
        
        
        /**
         * @brief Link the complete shader program from compiled shader stages.
         * @param shaders
         */
        void link( const std::vector<std::unique_ptr<Shader>>& shaders );

        /// @brief Get the OpenGL handle for this shader program.
        GLuint object() const;
        
        /**
         * @brief Get the location of a shader attribute within a shader.
         * @param attribName
         */
        GLint attribute( const GLchar* attribName ) const;
        /**
         * @brief Get the location of a uniform variable within shader.
         * @param uniformName The null-terminated string containing the name of the uniform variable whose location is to be queries.
         */
        GLint uniform( const GLchar* uniformName ) const;

        GLint subroutineUniform( const GLchar* uniformName, GLenum shaderType ) const;

        GLuint subroutineIndex( const GLchar* functionName, GLenum shaderType ) const;

        /// @brief Use this shader.
        void use() const;
        bool isInUse() const;
        void stopUsing() const;
        
        /// A wrapper for glUniform*i
        void setUniform( const GLchar*, GLint ) const;
        void setUniform( const GLchar*, GLint, GLint ) const;
        void setUniform( const GLchar*, GLint, GLint, GLint ) const;
        void setUniform( const GLchar*, GLint, GLint, GLint, GLint ) const;
        /// A wrapper for glUniform*f
        void setUniform( const GLchar*, GLfloat ) const;
        void setUniform( const GLchar*, GLfloat, GLfloat ) const;
        void setUniform( const GLchar*, GLfloat, GLfloat, GLfloat ) const;
        void setUniform( const GLchar*, GLfloat, GLfloat, GLfloat, GLfloat ) const;
        /// A wrapper for glUniform*fv
        void setUniform( const GLchar*, const glm::vec2& ) const;
        void setUniform( const GLchar*, const glm::vec3& ) const;
        void setUniform( const GLchar*, const glm::vec4& ) const;
        void setUniform( const GLchar*, const ce::Vector2f& ) const;
        void setUniform( const GLchar*, const ce::Vector3f& ) const;
        void setUniform( const GLchar*, const ce::Vector4f& ) const;

        /// A wrapper for glUniformMatrix*fv
        void setUniform( const GLchar*, const glm::mat3&, GLboolean transpose = GL_FALSE ) const;
        void setUniform( const GLchar*, const glm::mat4&, GLboolean transpose = GL_FALSE ) const;
        void setUniform( const GLchar*, const ce::Matrix3f& ) const;
        void setUniform( const GLchar*, const ce::Matrix4f& ) const;
        
    private:
        GLuint  object_{ 0u };
};


}   //namespace ce

