#include "app/Application.h"
#include "app/GameState.h"
#include "app/PauseState.h"
//#include "utils/Assert.h"
#include "utils/File.h"
#include "utils/Log.h"
#include "utils/Locator.h"
#include "Wrenly.h"
#include "json11/json11.hpp"
#include <SDL_timer.h>
#include <string>
#include <cstdint>

namespace {
uint32_t targetDeltaTime{ 16u };
inline float SDLTimeToPgTime(uint32_t dt) {
    return float(dt) / 1000.f;
}
}

namespace pg {

void Application::run() {
    initialize_();

    running_ = true;
    uint32_t tdelta{ targetDeltaTime };

    while (running_) {
        uint32_t start = SDL_GetTicks();
        /*
         * Handle events here
         * */
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            mouse_.handleEvent(event);
            stateStack_.handleEvent(event);
        }
        /*
         * A state might have called quits
         * */
        if (!context_.running) {
            running_ = false;
        }

        /*
        * Update mouse current mouse coordinates here
        * */
        mouse_.handleMousePressedCallbacks();

        context_.imguiRenderer->newFrame(SDLTimeToPgTime(tdelta), mouse_.getMouseCoords().x, mouse_.getMouseCoords().y);
        context_.textFileManager.update();
        stateStack_.update(SDLTimeToPgTime(tdelta));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        stateStack_.render(SDLTimeToPgTime(tdelta));
        context_.imguiRenderer->render();

        window_.display();

        /*
         * Sleep for the remainder of the frame, if we have time for it
         * */
        uint32_t end = SDL_GetTicks();
        tdelta = end - start;
        if (tdelta < targetDeltaTime) {
            tdelta = targetDeltaTime;
            SDL_Delay(targetDeltaTime - tdelta);
        }
    }
}

void Application::initialize_() {
    /*
     * Add managers to the locators
     **/
    Locator<MeshManager>::set(&context_.meshManager);
    Locator<TextFileManager>::set(&context_.textFileManager);

    /*
     * Initialize app state here
     * */
    auto json = pg::fileToString("config.json");
    std::string error{ "" };
    auto obj = json11::Json::parse(json, error).object_items();
    auto window = obj["window"].object_items();
    auto opengl = window["opengl"].object_items();

    targetDeltaTime = uint32_t(1.0f / obj["frameRate"].number_value());

    WindowSettings settings{};
    settings.width = window["width"].int_value();
    settings.height = window["height"].int_value();
    settings.name = window["name"].string_value();
    settings.glMajor = opengl["major"].int_value();
    settings.glMinor = opengl["minor"].int_value();
    settings.stencilBits = opengl["stencilBits"].int_value();
    settings.depthBits = opengl["depthBits"].int_value();
    settings.multisampleBuffer = opengl["multisampleBuffers"].int_value();
    settings.multisampleSamples = opengl["multisampleSamples"].int_value();

    window_.initialize(settings);

    std::string data = obj["dataPrefix"].string_value();
    std::string shaders = obj["builtinPrefix"].string_value();

    /*
     * Create app states here
     * */
    stateStack_.registerState< GameState >(states::Game);
    stateStack_.registerState< PauseState >(states::Pause);
    stateStack_.pushState(states::Game);

    /*
     * Initialize Wren state
     * */
    wrenly::Wren::loadModuleFn = [this](const char* mod) -> char* {
        std::string path(mod);
        path += ".wren";
        const std::string& source = this->context_.textFileManager.get(path);
        char* buffer = (char*)malloc(source.size() + 1);
        //PG_ASSERT(buffer != NULL);
        buffer[source.size()] = '\0';
        memcpy(buffer, source.c_str(), source.size());
        return buffer;
    };
    wrenly::Wren::writeFn = [](WrenVM* vm, const char* text) -> void {
        LOG_INFO << text;
    };

    /*
     * Load resources that all app states will depend on
     **/
    context_.textFileManager.addWatch(data, false);
    // everything here should eventually go into a loading state
    context_.shaderManager.addShader(shaders + "/basic.vert.glsl", GL_VERTEX_SHADER);
    context_.shaderManager.addShader(shaders + "/basic.frag.glsl", GL_FRAGMENT_SHADER);
    context_.shaderManager.compile("basic");

    context_.shaderManager.addShader(shaders + "/specular.vert.glsl", GL_VERTEX_SHADER);
    context_.shaderManager.addShader(shaders + "/specular.frag.glsl", GL_FRAGMENT_SHADER);
    context_.shaderManager.compile("specular");

    context_.shaderManager.addShader(shaders + "/panel.vert.glsl", GL_VERTEX_SHADER);
    context_.shaderManager.addShader(shaders + "/panel.frag.glsl", GL_FRAGMENT_SHADER);
    context_.shaderManager.compile("panel");

    /*
     * Finally, set the application context
     * */
    context_.window = &window_;
    context_.imguiRenderer = new system::ImGuiRenderer(context_);

    mouse_.registerMouseDownCallback(
        MouseButton::Left,
        [this]() -> void {
        this->context_.imguiRenderer->mouseButtonPressed(SDL_BUTTON_LEFT);
    }
    );
    mouse_.registerMouseUpCallback(
        MouseButton::Left,
        [this]() -> void {
        this->context_.imguiRenderer->mouseButtonReleased(SDL_BUTTON_LEFT);
    }
    );
}

}
