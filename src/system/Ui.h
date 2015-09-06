#pragma once

#include "app/Context.h"
#include "ecs/Include.h"
#include "3rdparty/imgui.h"

namespace pg {
namespace system {

class Ui: public ecs::System<Ui> {
    public:
        Ui() = delete;
        explicit Ui( Context& context );
        virtual ~Ui();
        
        void configure( ecs::EventManager& events ) override;
        void update( ecs::EntityManager&, ecs::EventManager&, float ) override;
        
    private:
        void initialize_();
        void shutdown_();
        void getEvents_();
        bool processEvent_( SDL_Event* );
        void getInput_( float dt );
        bool createDeviceObjects_();
        static void renderDrawLists_( ImDrawData* drawData );
        static void setClipboardText_( const char* text );
        static const char* getClipboardText_();
    
        Context&    context_;
};
    
}   // system
}   // pg


