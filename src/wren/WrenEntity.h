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
void listenToKeyDown(WrenVM*);
void listenToKeyPressed(WrenVM*);
void listenToKeyUp(WrenVM*);
void listenToMouseDown(WrenVM*);
void listenToMousePressed(WrenVM*);
void listenToMouseUp(WrenVM*);
void mouseX(WrenVM*);
void mouseY(WrenVM*);
void mouseDx(WrenVM*);
void mouseDy(WrenVM*);

}
}
