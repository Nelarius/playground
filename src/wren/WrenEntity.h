#pragma once

extern "C" {
    #include <wren.h>
}

namespace pg {
namespace wren {

void set( WrenVM* );
void entityIndex(WrenVM*);
void entityVersion(WrenVM*);
void setTransform(WrenVM*);
void hasTransform(WrenVM*);
void hasRenderable(WrenVM*);
void hasCamera(WrenVM*);
void hasPointLight(WrenVM*);
void createEntity(WrenVM*);
void entityCount(WrenVM*);

}
}
