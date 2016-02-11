#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>

namespace pg {

// A string interning class. Uses murmurhash internally.
class StringId {
public:

    using Database = std::unordered_map<uint32_t, std::vector<char>>;

    explicit StringId(const char*);
    StringId(const StringId&)               = default;
    StringId& operator=(const StringId&)    = default;
    StringId(StringId&&)                    = default;
    StringId& operator=(StringId&&)         = default;
    ~StringId()                             = default;

    bool operator==(const StringId& rhs) const;
    bool operator!=(const StringId& rhs) const;

    const char* cString() const;
    uint32_t    hash()    const;
    static void setDatabase(Database*);

private:
    uint32_t            hash_;
    static Database*    database_;
};

}
