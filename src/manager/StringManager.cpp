#include "manager/StringManager.h"
#include "utils/Exception.h"
#include "utils/Assert.h"
#include "utils/File.h"
#include "utils/Log.h"

namespace pg {

const std::string& StringManager::get( const std::string& file ) const {
    std::size_t uid = id( file );
    auto it = strings_.find( uid );
    if ( it != strings_.end() ) {
        return it->second;
    }

    std::string str{""};
    try {
        str = FileToString( file );
    } catch( const PlaygroundException& e ) {
        LOG_ERROR << e.what();
    }
    return strings_.emplace( uid, str ).first->second;
}

bool StringManager::contains( const std::string& file ) const {
    std::hash< std::string > hash;
    std::size_t id = hash( file );
    return contains( id );
}

bool StringManager::contains( std::size_t uid ) const {
    auto it = strings_.find( uid );
    if ( it != strings_.end() ) {
        return true;
    }
    return false;
}

std::size_t StringManager::id( const std::string& file ) const {
    std::hash< std::string > hash;
    return hash( file );
}

void StringManager::reload( const std::string& file ) {
    std::size_t uid = id( file );
    ASSERT( contains( uid ) );
    auto it = strings_.find( uid );
    try {
        it->second = FileToString( file );
    } catch( const PlaygroundException& e ) {
        LOG_ERROR << e.what();
    }
}

void StringManager::clear() {
    strings_.clear();
}

}