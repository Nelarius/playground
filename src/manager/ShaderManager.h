
#pragma once

#include <opengl/Program.h>
#include <opengl/Shader.h>
#include <utils/Borrowed.h>
#include <GL/glew.h>
#include <vector>
#include <map>
#include <memory>

namespace ce {

/*
 * What should the interface be? Should it manage memory?
 * Or should the scene be responsible for freeing memory?
 * 
 * If we don't want this class to manage memory, then we should change also
 * MeshManager to implement the "get" interface.
 * */
    
typedef Borrowed<Program>	BorrowedProgram;

class ShaderManager {
    public:        
        ShaderManager() = default;
        ~ShaderManager() = default;
        
        /**
         * @brief Add a shader stage into the compilation pipeline.
         * @param file The file containing the shader source code.
         * @param type The type of shader.
         * 
         * Valid types are GL_COMPUTE_SHADER, GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER,
         * GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER.
         */
        void            addShader( const std::string file, GLenum type );
        
        /**
         * @brief Compile the shaders currently in the pipeline to the given tag.
         * @param tag The tag with which you can get the shader
         */
        void            compile( const std::string& tag );
        
        /**
         * @brief Get the shader with the given tag.
         * @param tag
         */
        BorrowedProgram get( const std::string& tag );
        
        /**
         * @brief Delete all contained shaders.
         */        
        void            clear();
        
    private:
        std::map<const std::string, std::unique_ptr<Program>>   resources_{};
        std::vector<std::unique_ptr<Shader>>                    shaderStages_{};

};

}

