#pragma once

#include "utils/Assert.h"
#include <utility>

namespace pg {

template <typename T>
class StringBimap {
    static_assert(!std::is_same<T, std::string>::value, "Type different from std::string expected");

public:
    StringBimap() = default;
    ~StringBimap() = default;

    void insert(T t, const std::string& str) {
        toStringMap_.emplace(std::make_pair(t, str));
        toValueMap_.emplace(std::make_pair(str, t));
    }

    const std::string& at(T t) const {
        auto it = toStringMap_.find(t);
        ASSERT(it != toStringMap_.end());
        return it->second;
    }

    T at(const std::string& str) const {
        auto it = toValueMap_.find(str);
        ASSERT(it != toValueMap_.end());
        return it->second;
    }

private:
    std::map<T, std::string>    toStringMap_{};
    std::map<std::string, T>    toValueMap_{};
};

}
