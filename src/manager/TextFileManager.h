#pragma once

#include "filesentry/FileWatcher.h"
#include "ecs/Include.h"
#include <string>
#include <set>
#include <cstdlib>

namespace pg {

struct Context;

class TextFileManager {
public:
    TextFileManager(Context& context);
    ~TextFileManager() = default;

    // this method loads the source file if it doesn't exist already
    // this is to used when resources are loaded
    const std::string& get(const std::string& file) const;
    // this method does not
    // this is to be used within a system, like the script handler
    // they should not be accessing uids that aren't loaded
    const std::string& get(std::size_t uid) const;
    void reload(const std::string& file);
    bool contains(const std::string& file) const;
    bool contains(std::size_t) const;
    std::size_t id(const std::string&) const;
    void clear();

    void update();
    void addWatch(const std::string& dir, bool recursive = false);

private:
    Context&                                                context_;
    mutable std::unordered_map<std::size_t, std::string>    strings_;
    fs::FileWatcher                                         watcher_;
    std::set< std::string >                                 filesToReload_;
};

}