#pragma once

extern "C" {
#include <wren.h>
}

namespace pg {
namespace wren {

// NumberArray
void arrayPushBack(WrenVM*);
void arrayAt(WrenVM*);

// Entity
void set(WrenVM*);
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

// Imgui
struct ImGuiWindowFlag {
    uint32_t flag{ 0u };
};

// ImGui functions
void begin(WrenVM*);
void beginWithFlags(WrenVM*);
void setWindowPos(WrenVM*);
void setWindowSize(WrenVM*);
void text(WrenVM*);
void dummy(WrenVM*);
void textColored(WrenVM*);
void bulletText(WrenVM*);
void button(WrenVM*);
void buttonSized(WrenVM*);
void plotNumberArray(WrenVM*);
void plotNumberArrayWithOffset(WrenVM*);
void plotNumberArrayWithOffsetAndSize(WrenVM*);
void plotRingBuffer(WrenVM*);
void plotRingBufferWithSize(WrenVM*);

// ImGuiWindowFlags functions
void setTitleBar(WrenVM*);
void unsetTitleBar(WrenVM*);
void setResize(WrenVM*);
void unsetResize(WrenVM*);
void setMove(WrenVM*);
void unsetMove(WrenVM*);
void setShowBorders(WrenVM*);
void unsetShowBorders(WrenVM*);

// Quaternion
void conjugate(WrenVM* vm);
void inverse(WrenVM* vm);
void multiply(WrenVM* vm);
void axis(WrenVM* vm);

// Ringbuffer
void ringBufferPushBack(WrenVM*);

// Vector
void hadamard2f(WrenVM* vm);
void plus2f(WrenVM* vm);
void minus2f(WrenVM* vm);
void cross3f(WrenVM* vm);
void hadamard3f(WrenVM* vm);
void plus3f(WrenVM* vm);
void minus3f(WrenVM* vm);
void hadamard4f(WrenVM* vm);
void plus4f(WrenVM* vm);
void minus4f(WrenVM* vm);

}
}
