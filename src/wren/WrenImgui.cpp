#include "imgui/imgui.h"
#include "wren/WrenImgui.h"
#include "math/Vector.h"
#include "utils/Log.h"
#include <vector>

namespace pg {
namespace wren {

void begin( WrenVM* vm ) {
    ImGui::Begin( (const char*)wrenGetArgumentString( vm, 1 ), NULL, 0 );
}

void beginWithFlags( WrenVM* vm ) {
    ImGui::Begin(
        (const char*)wrenGetArgumentString( vm, 1 ),
        NULL,
        *(uint32_t*)wrenGetArgumentForeign( vm, 2 )
    );
}

void setWindowPos( WrenVM* vm ) {
    ImGui::SetNextWindowPos( *(const ImVec2*)wrenGetArgumentForeign( vm, 1 ) );
}

void setWindowSize( WrenVM* vm ) {
    ImGui::SetNextWindowSize( *(const ImVec2*)wrenGetArgumentForeign( vm, 1 ) );
}

void text( WrenVM* vm ) {
    const char* str = wrenGetArgumentString( vm, 1 );
    ImGui::Text( str );
}

void dummy( WrenVM* vm ) {
    const math::Vector2f* v = (const math::Vector2f*)wrenGetArgumentForeign( vm, 1 );
    ImGui::Dummy( (const ImVec2&)(*v) );
}

void textColored( WrenVM* vm ) {
    // do nothing for now
    //const math::Vector2f* v = (const math::Vector2f*)wrenGetArgumentForeign( vm, 1 );
    //const char* t = wrenGetArgumentString( vm, 2 );
    //ImGui::TextColored();
}

void bulletText( WrenVM* vm ) {
    ImGui::BulletText( (const char*)wrenGetArgumentString( vm, 1 ) );
}

void button( WrenVM* vm ) {
    ImGui::Button( (const char*)wrenGetArgumentString( vm, 1 ) );
}

void buttonSized( WrenVM* vm ) {
    ImGui::Button(
        (const char*)wrenGetArgumentString( vm, 1 ),
        *(const ImVec2*)wrenGetArgumentForeign( vm, 2 )
    );
}

void plotNumberArray( WrenVM* vm ) {
    const std::vector<float>* array = (const std::vector<float>*)wrenGetArgumentForeign( vm, 2 );
    ImGui::PlotLines(
        wrenGetArgumentString( vm, 1 ),
        array->data(),
        int( wrenGetArgumentDouble( vm, 3 ) ),
        0, NULL, FLT_MAX, FLT_MAX, ImVec2(140, 80), sizeof(float)
    );
}

void plotNumberArrayWithOffset( WrenVM* vm ) {
    const std::vector<float>* array = (const std::vector<float>*)wrenGetArgumentForeign( vm, 2 );
    ImGui::PlotLines(
        wrenGetArgumentString( vm, 1 ), array->data(),
        int( wrenGetArgumentDouble( vm, 3 ) ), int( wrenGetArgumentDouble( vm, 4 ) ),
        NULL, FLT_MAX, FLT_MAX, ImVec2(140, 80), sizeof(float)
    );
}

void plotNumberArrayWithOffsetAndSize( WrenVM* vm ) {
    const std::vector<float>* array = (const std::vector<float>*)wrenGetArgumentForeign( vm, 2 );
    ImGui::PlotLines(
        wrenGetArgumentString( vm, 1 ), array->data(),
        int( wrenGetArgumentDouble( vm, 3 ) ), int( wrenGetArgumentDouble( vm, 4 ) ),
        NULL, FLT_MAX, FLT_MAX,
        *(const ImVec2*)wrenGetArgumentForeign( vm, 5 ),
        sizeof(float)
    );
}

void setTitleBar( WrenVM* vm ) {
    uint32_t* bits = (uint32_t*)wrenGetArgumentForeign( vm, 0 );
    *bits &= ~ImGuiWindowFlags_NoTitleBar;
}

void unsetTitleBar( WrenVM* vm ) {
    uint32_t* bits = (uint32_t*)wrenGetArgumentForeign( vm, 0 );
    (*bits) |= ImGuiWindowFlags_NoTitleBar;
}

void setResize( WrenVM* vm ) {
    uint32_t* bits = (uint32_t*)wrenGetArgumentForeign( vm, 0 );
    *bits &= ~ImGuiWindowFlags_NoResize;
}

void setMove( WrenVM* vm ) {
    uint32_t* bits = (uint32_t*)wrenGetArgumentForeign( vm, 0 );
    *bits &= ~ImGuiWindowFlags_NoMove;
}

void unsetMove( WrenVM* vm ) {
    uint32_t* bits = (uint32_t*)wrenGetArgumentForeign( vm, 0 );
    *bits |= ImGuiWindowFlags_NoMove;
}

void unsetResize( WrenVM* vm ) {
    uint32_t* bits = (uint32_t*)wrenGetArgumentForeign( vm, 0 );
    *bits |= ImGuiWindowFlags_NoResize;
}

void setShowBorders( WrenVM* vm ) {
    uint32_t* bits = (uint32_t*)wrenGetArgumentForeign( vm, 0 );
    *bits |= ImGuiWindowFlags_ShowBorders;
}
void unsetShowBorders( WrenVM* vm ) {
    uint32_t* bits = (uint32_t*)wrenGetArgumentForeign( vm, 0 );
    *bits &= ~ImGuiWindowFlags_ShowBorders;
}

}
}