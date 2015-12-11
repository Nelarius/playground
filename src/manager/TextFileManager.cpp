#include "manager/TextFileManager.h"
#include "app/Context.h"
#include "system/Events.h"
#include "utils/Assert.h"
#include "utils/Locator.h"
#include "utils/File.h"
#include "utils/Log.h"

namespace pg {

TextFileManager::TextFileManager( Context& context )
:   context_{ context },
    strings_{},
    watcher_{},
    filesToReload_{}
    {}

const std::string& TextFileManager::get( const std::string& file ) const {
    LOG_INFO << "TextFileManager fetching " << file;
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

const std::string& TextFileManager::get( std::size_t uid ) const {
    auto it = strings_.find( uid );
    ASSERT( it != strings_.end() );
    return it->second;
}

bool TextFileManager::contains( const std::string& file ) const {
    std::hash< std::string > hash;
    std::size_t id = hash( file );
    return contains( id );
}

bool TextFileManager::contains( std::size_t uid ) const {
    auto it = strings_.find( uid );
    if ( it != strings_.end() ) {
        return true;
    }
    return false;
}

std::size_t TextFileManager::id( const std::string& file ) const {
    std::hash< std::string > hash;
    return hash( file );
}

void TextFileManager::reload( const std::string& file ) {
    std::size_t uid = id( file );
    ASSERT( contains( uid ) );
    auto it = strings_.find( uid );
    try {
        it->second = FileToString( file );
    } catch( const PlaygroundException& e ) {
        LOG_ERROR << e.what();
    }
}

void TextFileManager::clear() {
    strings_.clear();
}

void TextFileManager::update() {
    watcher_.update();
    for ( auto& str: filesToReload_ ) {
        reload( str );
        Locator< ecs::EventManager >::get()->emit< system::TextFileUpdated >( id( str ) );
    }
    filesToReload_.clear();
}

void TextFileManager::addWatch( const std::string& dir, bool recursive ) {
    watcher_.addWatch( dir,
        [this]( fs::WatchID id, const std::string& dir, const std::string& file, fs::Action action ) -> void {
            std::string path = dir + "/" + file;
            if ( action == fs::Action::Modified ) {
                this->filesToReload_.insert( path );
            } else if ( action == fs::Action::Delete ) {
                LOG_INFO << path << " was removed.";
            } else {
                LOG_INFO << path << " was added.";
            }
        },
        recursive
    );
}

}