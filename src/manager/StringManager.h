#pragma once

#include <unordered_map>

namespace pg {

/**
 * @class StringManager
 * @author Nelarius
 * @date 25/09/2015
 * @file StringManager.h
 * @brief Holds the contents of text files, such as script files.
 */
class StringManager {
    public:
        StringManager() = default;
        ~StringManager() = default;
        
        const std::string& get( const std::string& file ) const;
        void clear();
        std::size_t size() const;
        
    private:
        mutable std::unordered_map<std::string, std::string>    strings_{};
};

}