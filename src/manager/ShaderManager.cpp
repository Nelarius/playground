#include <manager/ShaderManager.h>
#include <utils/File.h>
#include <utils/Assert.h>

using ce::ShaderManager;

void ShaderManager::addShader( const std::string file, GLenum type ) {
    shaderStages_.emplace_back( new Shader( ce::FileToString( file ), type ) );
}

void ShaderManager::compile( const std::string& tag ) {
    //resources_.emplace( tag, std::make_unique<ce::Program>( shaderStages_ ) );
    auto index = buffer_.emplace( shaderStages_ );
    resources_.emplace( tag, &buffer_[index] );
    shaderStages_.clear();
}

ce::Program* ShaderManager::get( const std::string& tag ) {
    auto it = resources_.find( tag );
    
    #ifdef DEBUG
    std::string msg = "error: no such shader ";
    msg += tag;
    ASSERT( it != resources_.end(), msg.c_str() );
    #endif
    
    return it->second;
}

void ShaderManager::clear() {
    resources_.clear();
    shaderStages_.clear();
}
