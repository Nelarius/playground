#include "app/MouseEvents.h"
#include "app/Context.h"
#include "system/ScriptSystem.h"
#include "utils/Log.h"
#include "utils/StringBimap.h"
#include <SDL_mouse.h>
#include <cstdint>
#include <utility>

namespace {

pg::StringBimap<pg::MouseButton> initMap() {
    pg::StringBimap<pg::MouseButton> bimap{};
    bimap.insert(pg::MouseButton::Left, "Left");
    bimap.insert(pg::MouseButton::Middle, "Middle");
    bimap.insert(pg::MouseButton::Right, "Right");
    return bimap;
}

const pg::StringBimap<pg::MouseButton>& mousemapInstance() {
    static pg::StringBimap<pg::MouseButton> instance = initMap();
    return instance;
}

const char* toString(pg::MouseButton button) {
    return mousemapInstance().at(button).c_str();
}

pg::MouseButton toEnum(std::string str) {
    return mousemapInstance().at(str);
}

}

namespace pg {

MouseEvents::MouseEvents(Context& context)
    : mouseDownCallbacks_{},
    mousePressedCallbacks_{},
    mouseUpCallbacks_{},
    context_{ context },
    scriptSystem_{ nullptr } {}

math::Vec2i MouseEvents::getMouseCoords() const {
    return currentCoords_;
}

math::Vec2f MouseEvents::getNormalizedMouseCoords() const {
    return math::Vec2f{
        2.f * currentCoords_.x / context_.window->width() - 1.f,
        1.f - (2.f * currentCoords_.y / context_.window->height())
    };
}

math::Vec2f MouseEvents::getMouseDelta() const {
    return math::Vec2f{
        float(currentCoords_.x - previousCoords_.x) / context_.window->width(),
        - float(currentCoords_.y - previousCoords_.y) / context_.window->height()
    };
}

bool MouseEvents::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        auto it = mouseDownCallbacks_.find(event.button.button);
        if (it != mouseDownCallbacks_.end()) {
            it->second.callback();
            for (ecs::Entity* entity : it->second.entities) {
                scriptSystem_->onMouseDown(toString(MouseButton(it->first)), entity);
            }
            return true;
        }
    }
    else if (event.type == SDL_MOUSEBUTTONUP) {
        auto it = mouseUpCallbacks_.find(event.button.button);
        if (it != mouseUpCallbacks_.end()) {
            it->second.callback();
            for (ecs::Entity* entity : it->second.entities) {
                scriptSystem_->onMouseUp(toString(MouseButton(it->first)), entity);
            }
            return true;
        }
    }
    return false;
}

void MouseEvents::handleMousePressedCallbacks() {
    previousCoords_ = currentCoords_;
    const uint32_t state = SDL_GetMouseState(&currentCoords_.x, &currentCoords_.y);
    for (auto& pair : mousePressedCallbacks_) {
        if (state & SDL_BUTTON(pair.first)) {
            pair.second.callback();
            for (ecs::Entity* entity : pair.second.entities) {
                scriptSystem_->onMousePressed(toString(MouseButton(pair.first)), entity);
            }
        }
    }
}

void MouseEvents::addToMap_(std::map<int, CallbackData>& m, MouseButton button, ecs::Entity* entity) {
    auto it = m.find(int(button));
    if (it != m.end()) {
        it->second.entities.push_back(entity);
    }
    else {
        m.insert(std::make_pair(int(button), CallbackData(entity)));
    }
}

void MouseEvents::addToMap_(std::map<int, CallbackData>& m, MouseButton button, std::function<void()> callback) {
    auto it = m.find(int(button));
    if (it != m.end()) {
        it->second.callback = callback;
    }
    else {
        m.insert(std::make_pair(int(button), CallbackData(callback)));
    }
}

void MouseEvents::registerMouseDownScriptCallback(std::string button, ecs::Entity* entity) {
    addToMap_(mouseDownCallbacks_, toEnum(button), entity);
}

void MouseEvents::registerMousePressedScriptCallback(std::string button, ecs::Entity* entity) {
    addToMap_(mousePressedCallbacks_, toEnum(button), entity);
}

void MouseEvents::registerMouseUpScriptCallback(std::string button, ecs::Entity* entity) {
    addToMap_(mouseUpCallbacks_, toEnum(button), entity);
}

void MouseEvents::registerMouseDownCallback(MouseButton button, std::function<void()> callback) {
    addToMap_(mouseDownCallbacks_, button, callback);
}

void MouseEvents::registerMousePressedCallback(MouseButton button, std::function<void()> callback) {
    addToMap_(mousePressedCallbacks_, button, callback);
}

void MouseEvents::registerMouseUpCallback(MouseButton button, std::function<void()> callback) {
    addToMap_(mouseUpCallbacks_, button, callback);
}

void MouseEvents::setScriptHandler(system::ScriptSystem& system) {
    scriptSystem_ = &system;
}

}
