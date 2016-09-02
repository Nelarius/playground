
#pragma once

#include <SDL_events.h>
#include "app/Window.h"
#include "ecs/Include.h"
#include "system/ImGuiRenderer.h"
#include "manager/MeshManager.h"
#include "manager/ShaderManager.h"
#include "manager/TextFileManager.h"

namespace pg {

using namespace ecs;

struct Mouse {
    Mouse() = default;

    int x{ 0 };
    int y{ 0 };
    int dx{ 0 };
    int dy{ 0 };
};

/// @brief This struct holds variables that are used by the systems at runtime.
struct Context {

    friend class Application;

    Context() = default;
    ~Context() = default;

    /// @brief Get real-time input for the mouse.
    Mouse mouse()     const { return mouse_; }

    EventManager  eventManager{};
    EntityManager entityManager{ eventManager };
    SystemManager systemManager{ eventManager, entityManager };

    MeshManager     meshManager{};
    ShaderManager   shaderManager{};
    TextFileManager textFileManager{ *this };
    bool            running{ true };
    Window*         window{ nullptr };
    system::ImGuiRenderer* imguiRenderer{ nullptr };

private:
    Mouse  mouse_{};

};

}

