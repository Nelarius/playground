#pragma once

#include <unordered_map>

namespace pg {

/**
 * @class StringManager
 * @file StringManager.h
 * @brief Holds the contents of text files, such as script files.
 */
class StringManager {
    public:
        StringManager() = default;
        ~StringManager() = default;

        const std::string& get( const std::string& file ) const;
        void reload( const std::string& file );
        bool contains( const std::string& file ) const;
        bool contains( std::size_t ) const;
        std::size_t id( const std::string& ) const;
        void clear();

    private:
        mutable std::unordered_map<std::size_t, std::string>    strings_{};
};

}