#pragma once

#include "app/Context.h"
#include "3rdparty/imgui.h"
#include "opengl/BufferObject.h"
#include "opengl/Texture.h"
#include "opengl/Program.h"

namespace pg {
namespace system {

class ImGuiRenderer {
    public:
        ImGuiRenderer() = delete;
        ImGuiRenderer( Context& context );
        virtual ~ImGuiRenderer();
        
        void update( float dt );
        
        // public function callbacks need to be set here
        // the idea would be to have ImGuiRenderer do something with a mouse click
        // I would bind the callback to a real time mouse input handler
        
    protected:
        void initialize_();
        void shutdown_();
        void createDeviceObjects_();
        void newFrame_( float dt );
        
        Context&            context_;
        static uint32_t     refCount_;
};
    
}   // system
}   // pg
