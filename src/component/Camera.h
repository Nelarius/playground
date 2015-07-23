
#pragma once

namespace ce  {
namespace component {

struct Camera {
    Camera() = default;
    ~Camera() = default;
    Camera( 
        float vfov, float nplane, float fplane,
        bool perspective, bool active = true
    )
    :   verticalFov( vfov ),
        nearPlane( nplane ),
        farPlane( fplane ),
        viewPerspective( perspective ),
        active( active)
        {}
    
    float   verticalFov{ 1.13f };   //corresponds to about 65 degrees
    float   nearPlane{ 0.01f };
    float   farPlane{ 1000.0f };
    bool    viewPerspective{ true };
    bool    active{ true };
};


}   //namespace component
}   //namespace ce
