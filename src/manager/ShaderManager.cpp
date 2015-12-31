#include "utils/Exception.h"
#include "manager/ShaderManager.h"
#include "utils/File.h"
#include "utils/Assert.h"

namespace pg {

void ShaderManager::addShader( const std::string file, GLenum type ) {
    try {
        shaderStages_.emplace_back( new opengl::Shader( pg::FileToString( file ), type ) );
    } catch ( const PlaygroundException& ) {
        // this is kinda lame. It looks like I'm ignoring the exception, when in fact it is logged.
    }
}

void ShaderManager::compile( const std::string& tag ) {
    try {
        auto index = buffer_.emplace( shaderStages_ );
        resources_.emplace( tag, &buffer_[index] );
    } catch( const PlaygroundException& ) {
        LOG_ERROR << "Exception in compiling " << tag << " shader.";
    }
    shaderStages_.clear();
}

opengl::Program* ShaderManager::get( const std::string& tag ) const {
    auto it = resources_.find( tag );
    
    ASSERT( it != resources_.end() );
    
    return it->second;
}

void ShaderManager::clear() {
    resources_.clear();
    shaderStages_.clear();
}

}
