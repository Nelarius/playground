#pragma once

#include "utils/Assert.h"
#include "mm_json.h"

#include <cstring>
#include <cstdlib>
#include <vector>

namespace pg {

struct JsonToken;

class JsonParser {
public:
    explicit JsonParser(const char* fileName);
    JsonParser() = delete;
    ~JsonParser();

    pg::JsonToken query(const char* q) const;

    pg::JsonToken query(const pg::JsonToken& token, const char* q) const;

private:
    int               numTokens_;
    json_token*       tokens_;
    std::vector<char> json_;
};

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
        if (token_) {
            if (token->type == JSON_STRING) {
                std::size_t count = indexToFirstDelim(token->str, '\"');
                PG_ASSERT(count <= TokenBufferSize_);
                std::memset(buffer_, 0, TokenBufferSize_);
                std::memcpy(buffer_, token->str, count);
            }
        }
    }
    JsonToken() = delete;
    ~JsonToken() = default;

    template<typename T>
    T as() const {
        static_assert(false, "This should be fully specialized.");
    }

    inline operator bool() const {
        return token_ != NULL;
    }

private:

    friend class JsonParser;

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
inline int JsonToken::as() const {
    PG_ASSERT(token_->type == JSON_NUMBER);
    return int(std::atof(token_->str));
}

template<>
inline unsigned int JsonToken::as() const {
    PG_ASSERT(token_->type == JSON_NUMBER);
    return unsigned int(std::atof(token_->str));
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

struct Context;

void readScene(Context& context, const char* file);
void writeScene(Context& context, const char* file);

}
