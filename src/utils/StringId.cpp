#include "utils/StringId.h"
#include "utils/Assert.h"
#include <cstring>
#include <utility>

namespace {

// this seems to be pretty okay for hashing strings, used in libstdc++
// lifted from here https://en.wikipedia.org/wiki/MurmurHash
#define ROT32(x, y) ((x << y) | (x >> (32 - y)))
uint32_t murmur3_32(const char *key, uint32_t len, uint32_t seed) {
    static const uint32_t c1 = 0xcc9e2d51;
    static const uint32_t c2 = 0x1b873593;
    static const uint32_t r1 = 15;
    static const uint32_t r2 = 13;
    static const uint32_t m = 5;
    static const uint32_t n = 0xe6546b64;

    uint32_t hash = seed;

    const int nblocks = len / 4;
    const uint32_t *blocks = (const uint32_t *)key;
    int i;
    uint32_t k;
    for (i = 0; i < nblocks; i++) {
        k = blocks[i];
        k *= c1;
        k = ROT32(k, r1);
        k *= c2;

        hash ^= k;
        hash = ROT32(hash, r2) * m + n;
    }

    const uint8_t *tail = (const uint8_t *)(key + nblocks * 4);
    uint32_t k1 = 0;

    switch (len & 3) {
    case 3:
        k1 ^= tail[2] << 16;
    case 2:
        k1 ^= tail[1] << 8;
    case 1:
        k1 ^= tail[0];

        k1 *= c1;
        k1 = ROT32(k1, r1);
        k1 *= c2;
        hash ^= k1;
    }

    hash ^= len;
    hash ^= (hash >> 16);
    hash *= 0x85ebca6b;
    hash ^= (hash >> 13);
    hash *= 0xc2b2ae35;
    hash ^= (hash >> 16);

    return hash;
}
#undef ROT32

}

namespace pg {

StringId::Database* StringId::database_ = nullptr;

StringId::StringId(const char* str) {
    PG_ASSERT(database_);
    const uint32_t seed = 4u;   // random number
    std::size_t len = strlen(str);
    hash_ = murmur3_32(str, len, seed);
    std::vector<char> strVec(len + 1u, '\0');
    memcpy(strVec.data(), str, len);
    database_->insert(std::make_pair(hash_, strVec));
}

bool StringId::operator==(const StringId& rhs) const {
    return hash_ == rhs.hash_;
}

bool StringId::operator!=(const StringId& rhs) const {
    return hash_ != rhs.hash_;
}

const char* StringId::cString() const {
    auto it = database_->find(hash_);
    PG_ASSERT(it != database_->end());
    return it->second.data();
}

void StringId::setDatabase(Database* db) {
    database_ = db;
}

}
