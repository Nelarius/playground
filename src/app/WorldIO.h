
#pragma once 

#include "app/Context.h"
#include "ecs/Entity.h"
#include "ecs/Event.h"
#include "ecs/System.h"
#include <string>

namespace pg {
    
/**
 * @class WorldIO
 * @author Nelarius
 * @date 28/09/2015
 * @file WorldIO.h
 * @brief Read and write the world to file.
 */
class WorldIO {
    public:
        WorldIO( Context& context );
        WorldIO()   = delete;
        ~WorldIO()  = default;
        void read( 
            const std::string& file, 
            ecs::EntityManager&,
            ecs::EventManager&
        );
        
    private:
        Context& context_;
};

}