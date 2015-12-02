#pragma once

namespace pg {

template< typename T >
class Locator {
    public:
        static T* get() {
            return service_;
        }

        static void set( T* service ) {
            service_ = service;
        }

    private:
        static T* service_;
};

template< typename T >
T* Locator<T>::service_ = nullptr;

}