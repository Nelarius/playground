#pragma once

#include "utils/StringId.h"
extern "C" {
#include <wren.h>
}

namespace pg {
namespace wren {

struct WrenRenderable {
    StringId model;
    StringId shader;
    float shininess;
    math::Vec3f baseColor;
    math::Vec3f ambientColor;
    math::Vec3f specularColor;
};

// math lib
void planeIntersection(WrenVM*);
void generateCameraRay(WrenVM*);

// NumberArray
void arrayPushBack(WrenVM*);
void arrayAt(WrenVM*);

// Entity
void set(WrenVM*);
void entityIndex(WrenVM*);
void entityVersion(WrenVM*);
void hasTransform(WrenVM*);
void hasRenderable(WrenVM*);
void hasCamera(WrenVM*);
void hasPointLight(WrenVM*);
void assignTransform(WrenVM*);
void assignRenderable(WrenVM*);
void setTransform(WrenVM*);
void getTransform(WrenVM*);
void getRenderable(WrenVM*);
void setRenderable(WrenVM*);
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
    std::uint32_t flag{ 0u };
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
void sliderFloat(WrenVM*);

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
void getQuatReal(WrenVM* vm);

// Ringbuffer
void ringBufferPushBack(WrenVM*);

// Vector
void scale2f(WrenVM* vm);
void scale3f(WrenVM* vm);
void scale4f(WrenVM* vm);

// systems
void castCameraRay(WrenVM* vm);
void addStaticDebugBox(WrenVM* vm);
void addTransientDebugBox(WrenVM* vm);
void addStaticDebugLine(WrenVM* vm);
void addTransientDebugLine(WrenVM* vm);

// components
void getTransformPosition(WrenVM* vm);
void getTransformRotation(WrenVM* vm);
void getTransformScale(WrenVM* vm);

}
}
