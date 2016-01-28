#pragma once

#include "utils/Assert.h"

namespace pg {

template< typename T >
class Locator {
public:
    static T* get() {
        ASSERT(service_ != nullptr);
        return service_;
    }

    static void set(T* service) {
        service_ = service;
    }

private:
    static T* service_;
};

template< typename T >
T* Locator<T>::service_ = nullptr;

}