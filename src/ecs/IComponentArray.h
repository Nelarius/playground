#pragma once

#include <cstdint>

namespace pg {
namespace ecs {

/// The component arrays are meant to be used as follows. When creating an array, whether it comes
/// from a free list or not, componentArray.insert(id) must be called. This is to accommodate the
/// sparse array implementation.
///
/// Similarly, when the entity is destroyed, remove must be called.
class IComponentArray {
public:
    IComponentArray(uint32_t elementSize, uint32_t numElements);
    virtual void insert(uint32_t id) = 0;
    virtual void remove(uint32_t id) = 0;
    virtual uint8_t* at(uint32_t id) = 0;
};

class DenseArray : public IComponentArray {
public:
    DenseArray(uint32_t, uint32_t);
    void insert(uint32_t id) override;
    void remove(uint32_t id) override;
    uint8_t* at(uint32_t id) override;

private:
};

class SparseArray : public IComponentArray {
public:
    SparseArray(uint32_t, uint32_t);
    void insert(uint32_t id) override;
    void remove(uint32_t id) override;
    uint8_t* at(uint32_t id) override;

private:
};

}
}
