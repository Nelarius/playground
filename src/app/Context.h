
#pragma once

#include <SDL2/SDL_events.h>
#include "app/Window.h"
#include "manager/MeshManager.h"
#include "manager/ShaderManager.h"
#include "manager/TextFileManager.h"
#include <vector>

namespace pg {

struct Mouse {
    Mouse() = default;

    int x{0};
    int y{0};
    int dx{0};
    int dy{0};
};

/// @brief This struct holds variables that are used by the systems at runtime.
struct Context {

    friend class Application;

    Context() = default;
    ~Context() = default;

    /// @brief Get real-time input for the mouse.
    Mouse mouse()     const { return mouse_; }
    /// @brief Get the elapsed time since the program started!
    float elapsedTime() const;
    
    MeshManager     meshManager{};
    ShaderManager   shaderManager{};
    TextFileManager textFileManager{ *this };
    bool            running{ true };
    Window*         window{ nullptr };
    
    private:
        Mouse  mouse_{};
        float  time_{0.0f};

};

}

