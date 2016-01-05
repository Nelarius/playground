#include "imgui/imgui.h"
#include "wren/WrenImgui.h"
#include "math/Vector.h"
#include "utils/BasicStorage.h"
#include "utils/RingBuffer.h"
#include "utils/Log.h"
#include <vector>

namespace pg {
namespace wren {

void begin( WrenVM* vm ) {
    ImGui::Begin( (const char*)wrenGetSlotString( vm, 1 ), NULL, 0 );
}

void beginWithFlags( WrenVM* vm ) {
    ImGui::Begin(
        (const char*)wrenGetSlotString( vm, 1 ),
        NULL,
        *(uint32_t*)wrenGetSlotForeign( vm, 2 )
    );
}

void setWindowPos( WrenVM* vm ) {
    ImGui::SetNextWindowPos( *(const ImVec2*)wrenGetSlotForeign( vm, 1 ) );
}

void setWindowSize( WrenVM* vm ) {
    ImGui::SetNextWindowSize( *(const ImVec2*)wrenGetSlotForeign( vm, 1 ) );
}

void text( WrenVM* vm ) {
    const char* str = wrenGetSlotString( vm, 1 );
    ImGui::Text( str );
}

void dummy( WrenVM* vm ) {
    const math::Vector2f* v = (const math::Vector2f*)wrenGetSlotForeign( vm, 1 );
    ImGui::Dummy( (const ImVec2&)(*v) );
}

void textColored( WrenVM* vm ) {
    // do nothing for now
    //const math::Vector2f* v = (const math::Vector2f*)wrenGetSlotForeign( vm, 1 );
    //const char* t = wrenGetSlotString( vm, 2 );
    //ImGui::TextColored();
}

void bulletText( WrenVM* vm ) {
    ImGui::BulletText( (const char*)wrenGetSlotString( vm, 1 ) );
}

void button( WrenVM* vm ) {
    ImGui::Button( (const char*)wrenGetSlotString( vm, 1 ) );
}

void buttonSized( WrenVM* vm ) {
    ImGui::Button(
        (const char*)wrenGetSlotString( vm, 1 ),
        *(const ImVec2*)wrenGetSlotForeign( vm, 2 )
    );
}

void plotNumberArray( WrenVM* vm ) {
    const std::vector<float>* array = (const std::vector<float>*)wrenGetSlotForeign( vm, 2 );
    ImGui::PlotLines(
        wrenGetSlotString( vm, 1 ),
        array->data(),
        int( wrenGetSlotDouble( vm, 3 ) ),
        0, NULL, FLT_MAX, FLT_MAX, ImVec2(140, 80), sizeof(float)
    );
}

void plotNumberArrayWithOffset( WrenVM* vm ) {
    const std::vector<float>* array = (const std::vector<float>*)wrenGetSlotForeign( vm, 2 );
    ImGui::PlotLines(
        wrenGetSlotString( vm, 1 ), array->data(),
        int( wrenGetSlotDouble( vm, 3 ) ), int( wrenGetSlotDouble( vm, 4 ) ),
        NULL, FLT_MAX, FLT_MAX, ImVec2(140, 80), sizeof(float)
    );
}

void plotNumberArrayWithOffsetAndSize( WrenVM* vm ) {
    const std::vector<float>* array = (const std::vector<float>*)wrenGetSlotForeign( vm, 2 );
    ImGui::PlotLines(
        wrenGetSlotString( vm, 1 ), array->data(),
        int( wrenGetSlotDouble( vm, 3 ) ), int( wrenGetSlotDouble( vm, 4 ) ),
        NULL, FLT_MAX, FLT_MAX,
        *(const ImVec2*)wrenGetSlotForeign( vm, 5 ),
        sizeof(float)
    );
}

void plotRingBuffer( WrenVM* vm ) {
    RingBuffer<float>* buffer = (RingBuffer<float>*)wrenGetSlotForeign( vm, 2 );
    BasicStorage<float> numbers{ buffer->size() };
    for ( unsigned int i = 0; i < buffer->size(); i++ ) {
        *numbers.at(i) = buffer->at(i);
    }
    ImGui::PlotLines(
        wrenGetSlotString( vm, 1 ), numbers.at(0),
        buffer->size(), 0,
        NULL, FLT_MAX, FLT_MAX,
        ImVec2(140, 80),
        sizeof(float)
    );
}

void plotRingBufferWithSize( WrenVM* vm ) {
    RingBuffer<float>* buffer = (RingBuffer<float>*)wrenGetSlotForeign( vm, 2 );
    BasicStorage<float> numbers{ buffer->size() };
    for ( unsigned int i = 0; i < buffer->size(); i++ ) {
        *numbers.at(i) = buffer->at(i);
    }
    ImGui::PlotLines(
        wrenGetSlotString( vm, 1 ), numbers.at(0),
        buffer->size(), 0,
        NULL, FLT_MAX, FLT_MAX,
        *(const ImVec2*)wrenGetSlotForeign( vm, 3 ),
        sizeof(float)
    );
}

void setTitleBar( WrenVM* vm ) {
    uint32_t* bits = (uint32_t*)wrenGetSlotForeign( vm, 0 );
    *bits &= ~ImGuiWindowFlags_NoTitleBar;
}

void unsetTitleBar( WrenVM* vm ) {
    uint32_t* bits = (uint32_t*)wrenGetSlotForeign( vm, 0 );
    (*bits) |= ImGuiWindowFlags_NoTitleBar;
}

void setResize( WrenVM* vm ) {
    uint32_t* bits = (uint32_t*)wrenGetSlotForeign( vm, 0 );
    *bits &= ~ImGuiWindowFlags_NoResize;
}

void setMove( WrenVM* vm ) {
    uint32_t* bits = (uint32_t*)wrenGetSlotForeign( vm, 0 );
    *bits &= ~ImGuiWindowFlags_NoMove;
}

void unsetMove( WrenVM* vm ) {
    uint32_t* bits = (uint32_t*)wrenGetSlotForeign( vm, 0 );
    *bits |= ImGuiWindowFlags_NoMove;
}

void unsetResize( WrenVM* vm ) {
    uint32_t* bits = (uint32_t*)wrenGetSlotForeign( vm, 0 );
    *bits |= ImGuiWindowFlags_NoResize;
}

void setShowBorders( WrenVM* vm ) {
    uint32_t* bits = (uint32_t*)wrenGetSlotForeign( vm, 0 );
    *bits |= ImGuiWindowFlags_ShowBorders;
}
void unsetShowBorders( WrenVM* vm ) {
    uint32_t* bits = (uint32_t*)wrenGetSlotForeign( vm, 0 );
    *bits &= ~ImGuiWindowFlags_ShowBorders;
}

}
}
