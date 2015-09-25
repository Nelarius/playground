#include "manager/StringManager.h"
#include "utils/Exception.h"
#include "utils/File.h"

namespace pg {

const std::string& StringManager::get( const std::string& file ) const {
    auto it = strings_.find( file );
    if ( it != strings_.end() ) {
        return it->second;
    }
    
    std::string str{""};
    try {
        str = FileToString( file );
    } catch( const PlaygroundException& e ) {
        str = "error";
    }
    return strings_.emplace( file, str ).first->second;
}

}