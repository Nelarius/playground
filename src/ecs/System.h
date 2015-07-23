
#pragma once

#include <cstddef>  // for std::size_t

namespace ce {
namespace ecs {

class SystemManager;

/**
 * @class BaseSystem
 * @author Muszynski Johann M
 * @date 06/10/15
 * @file System.h
 * @brief Prefer to use 
 */
class BaseSystem {
    public:
        using Family = std::size_t;
        
        BaseSystem() = default;
        virtual ~BaseSystem() = default;
        
        BaseSystem( const BaseSystem& )             = delete;
        BaseSystem& operator=( const BaseSystem& )  = delete;
        BaseSystem( BaseSystem&& )                  = delete;
        BaseSystem& operator=( BaseSystem&& )       = delete;
        
        /**
         * @brief 
         */
        virtual void configure() = 0;
        /**
         * @brief Apply the behavior of the system.
         * @param dt
         */
        virtual void update( float dt ) = 0;
        
    private:
        Family familyCounter;   // should this be private?
};

Family BaseSystem::familyCounter = 0u;

/**
 * @class System
 * @author Muszynski Johann M
 * @date 06/10/15
 * @file System.h
 * @brief Use this class for implementing systems.
 */
template<typename Derived> 
class System: public BaseSystem {
    public:
        System();
        virtual ~System();
        
    private:
        // friend class SystemManager;
        
        static Family family() {
            static Family family{ familyCounter++ };
            return family;
        }

};

}   // namespace ecs
}   // namespace ce 

