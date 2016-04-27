#include "Json.h"
#include "File.h"
#define JSON_IMPLEMENTATION
#define JSON_STATIC
#include "mm_json.h"

namespace pg {

JsonParser::JsonParser(const char* fileName)
    : numTokens_(0),
    tokens_(nullptr),
    json_() {
    if (fileExists(fileName)) {
        std::ifstream file(fileName, std::ios::binary | std::ios::ate);
        std::size_t size = std::size_t(file.tellg());
        file.seekg(0, std::ios::beg);
        json_.resize(size + 1u);
        if (file.read(json_.data(), size)) {
            json_[size] = '\0';
            int read = 0;
            numTokens_ = json_num(json_.data(), json_.size());
            tokens_ = (json_token*)std::calloc(numTokens_, sizeof(json_token));
            json_load(tokens_, numTokens_, &read, json_.data(), json_.size());
        }
    }
}

JsonParser::~JsonParser() {
    std::free(tokens_);
}

pg::JsonToken JsonParser::query(const char* q) const {
    return pg::JsonToken(json_query(tokens_, numTokens_, q));
}

pg::JsonToken JsonParser::query(const pg::JsonToken& token, const char* q) const {
    return pg::JsonToken(json_query(token.token_, token.token_->sub, q));
}

}
