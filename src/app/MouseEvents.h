#pragma once

#include "app/Command.h"
#include "math/Vector.h"
#include <SDL_events.h>
#include <vector>
#include <utility>
#include <unordered_map>
#include <map>

namespace pg {

namespace ecs {
class Entity;
}

namespace system {
class ScriptHandler;
}

/*
 * These enumerations correspond to SDL_BUTTON_* values,
 * defined in http://hg.libsdl.org/SDL/file/default/include/SDL_mouse.h
 */
enum MouseButton {
    Left = 1,
    Middle,
    Right
};

/**
 * @class MouseEvents
 * @file MouseEvents.h
 * @brief Bind callbacks to mouse events.
 */
class MouseEvents{
    public:
        MouseEvents()   = default;
        ~MouseEvents()  = default;

        math::Vec2i getMouseCoords() const;
        math::Vec2i getMouseDelta() const;

        void registerMouseDownScriptCallback(std::string, ecs::Entity*);
        void registerMousePressedScriptCallback(std::string, ecs::Entity*);
        void registerMouseUpScriptCallback(std::string, ecs::Entity*);
        void registerMouseDownCallback(MouseButton button, std::function<void()>);
        void registerMousePressedCallback(MouseButton button, std::function<void()>);
        void registerMouseUpCallback(MouseButton button, std::function<void()>);
        void handleEvent(const SDL_Event&);
        void handleMousePressedCallbacks();

        void setScriptHandler(system::ScriptHandler&);

    private:

        struct CallbackData {
            explicit CallbackData(ecs::Entity* entity)
                : entities{},
                callback{ []() -> void {} } {
                entities.push_back(entity);
            }
            explicit CallbackData(std::function<void()> callable)
                : entities{},
                callback{ callable } {}
            std::vector<ecs::Entity*>   entities;
            std::function<void()>       callback;
        };

        uint32_t    previousState_; // the mouse state mask
        math::Vec2i previousCoords_;
        math::Vec2i currentCoords_;

        void addToMap_(std::map<int, CallbackData>&, MouseButton, std::function<void()>);
        void addToMap_(std::map<int, CallbackData>&, MouseButton, ecs::Entity*);

        std::map<int, CallbackData> mouseDownCallbacks_{};
        std::map<int, CallbackData> mousePressedCallbacks_{};
        std::map<int, CallbackData> mouseUpCallbacks_{};
        system::ScriptHandler*      scriptSystem_{ nullptr };
};

}
