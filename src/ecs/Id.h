
#pragma once

#include <cstdint>

namespace pg {
namespace ecs {

class Id {
public:
    constexpr Id() = default;
    Id(const Id&) = default;
    Id(Id&&) = default;
    Id& operator=(const Id&) = default;
    Id& operator=(Id&&) = default;
    explicit Id(uint64_t id)
        : id_(id)
    {}
    Id(uint32_t index, uint32_t version)
        : id_(uint64_t(index) | uint64_t(version) << 32UL)
    {}
    ~Id() = default;

    inline uint64_t id() const { return id_; }

    inline bool operator==(const Id& rhs) const { return id_ == rhs.id_; }
    inline bool operator!=(const Id& rhs) const { return id_ != rhs.id_; }
    inline bool operator<(const Id& rhs) const { return id_ < rhs.id_; }
    inline bool operator>(const Id& rhs) const { return id_ > rhs.id_; }

    inline uint32_t index() const { return id_ & 0xffffffffUL; }
    inline uint32_t version() const { return id_ >> 32; }

private:
    uint64_t id_{ 0u };
};

const Id Invalid{};


}   // namespace ecs
}   // namespace ce