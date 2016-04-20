#pragma once

#include "utils/Assert.h"
#include "mm_json.h"

#include <cstring>
#include <cstdlib>

namespace pg {
namespace detail {

inline std::size_t indexToFirstDelim(const char* str, char delim) {
    for (std::size_t i = 0u;; ++i) {
        if (str[i] == delim) {
            return i;
        }
    }
}

struct JsonToken {
    explicit JsonToken(json_token* token)
        : token_{ token } {
        if (token->type == JSON_STRING) {
            std::size_t count = indexToFirstDelim(token->str, '\"');
            PG_ASSERT(count <= TokenBufferSize_);
            std::memset(buffer_, 0, TokenBufferSize_);
            std::memcpy(buffer_, token->str, count);
        }
    }
    JsonToken() = delete;
    ~JsonToken() = default;

    template<typename T>
    T as() const {
        PG_ASSERT(false);
    }

    inline operator bool() const {
        return token_ == NULL;
    }

private:
    static const int TokenBufferSize_{32};
    json_token* token_;
    char        buffer_[TokenBufferSize_];
};

template<>
inline double JsonToken::as() const {
    PG_ASSERT(token_->type == JSON_NUMBER);
    return std::atof(token_->str);
}

template<>
inline float JsonToken::as() const {
    PG_ASSERT(token_->type == JSON_NUMBER);
    return float(std::atof(token_->str));
}

template<>
inline const char* JsonToken::as() const {
    PG_ASSERT(token_->type == JSON_STRING);
    return buffer_;
}

template<>
inline bool JsonToken::as() const {
    PG_ASSERT(token_->type == JSON_TRUE || token_->type == JSON_FALSE);
    return token_->type == JSON_TRUE;
}

}

struct Context;

void readScene(Context& context, const char* file);
void writeScene(Context& context, const char* file);

}
