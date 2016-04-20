#include "SceneIO.h"
#include "utils/Assert.h"
#include "utils/File.h"
#include "utils/Log.h"
#define JSON_IMPLEMENTATION
#define JSON_STATIC
#include "mm_json.h"

#include <cstdio>
#include <vector>
#include <string>
#include <fstream>

namespace {

class JsonParser {
public:
    explicit JsonParser(const char* fileName)
    :   numTokens_(0),
        tokens_(nullptr),
        json_() {
        if (pg::fileExists(fileName)) {
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
    JsonParser() = delete;
    ~JsonParser() {
        std::free(tokens_);
    }

    inline pg::detail::JsonToken query(const char* q) const {
        return pg::detail::JsonToken(json_query(tokens_, numTokens_, q));
    }

private:
    int               numTokens_;
    json_token*       tokens_;
    std::vector<char> json_;
};

}

namespace pg {

/*
 The new scene format looks as follows.
 {
    "entities" : [
        {
            "transform" : {
                "position" : [],
                "rotation" : [],
                "scale" : []
            }
        }
    ]
 }*/
void readScene(Context& context, const char* file) {
    JsonParser json(file);

    // parse while the scene array has tokens
    char queryString[64];
    for (int i = 0;; ++i) {
        sprintf(queryString, "entities[%i]", i);
        auto entity = json.query(queryString);
        if (!entity) {
            break;
        }
    }
}

void writeScene(Context& context, const char* file) {
    //
}

}
