#include "app/Application.h"
#include "app/GameState.h"
#include "app/PauseState.h"
#include "utils/Assert.h"
#include "utils/File.h"
#include "utils/Json.h"
#include "utils/Log.h"
#include "utils/Locator.h"
#include "utils/StringId.h"
#include "system/WrenBindings.h"
#include "system/DebugDrawRenderer.h"
#include "system/RenderSystem.h"
#include "DebugDraw.hpp"
#include "Wren++.h"
#include "mm_json.h"
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
    StringId::Database stringDb{};
    StringId::setDatabase(&stringDb);

    initialize_();

    DebugDrawRenderer debugDrawRenderer(context_);
    dd::initialize(&debugDrawRenderer);

    running_ = true;
    uint32_t tdelta{ targetDeltaTime };

    while (running_) {
        uint32_t start = SDL_GetTicks();
        context_.imguiRenderer->newFrame(SDLTimeToPgTime(tdelta), mouse_.getMouseCoords().x, mouse_.getMouseCoords().y);
        /*
         * Handle events here
         * */
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // there are only two events which are handled here
            // mouse down and mouse up, which are passed to ImGui
            // so isn't the mouse click then passed on to the script system?
            // Because currently the GameState registers its own mouse handler with the script system
            if (!(mouse_.handleEvent(event) && ImGui::IsMouseHoveringAnyWindow())) {
                stateStack_.handleEvent(event);
            }
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

        context_.textFileManager.update();
        stateStack_.update(SDLTimeToPgTime(tdelta));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        stateStack_.render(SDLTimeToPgTime(tdelta));
        dd::flush(std::uint64_t(tdelta * 1000.f));
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

    dd::shutdown();
}

void Application::initialize_() {
    /*
     * Add the resource managers to the locators.
     * This is so that we can access the managers from C-style free functions.
     **/
    Locator<MeshManager>::set(&context_.meshManager);
    Locator<ShaderManager>::set(&context_.shaderManager);
    Locator<TextFileManager>::set(&context_.textFileManager);

    /*
     * Initialize app state here
     * */
    JsonParser json("config.json");
    WindowSettings settings{};

    JsonToken window = json.query("window");
    settings.width = json.query(window, "width").as<int>();
    settings.height = json.query(window, "height").as<int>();
    settings.name = json.query(window, "name").as<const char*>();
    JsonToken opengl = json.query(window, "opengl");
    settings.glMajor = json.query(opengl, "major").as<int>();
    settings.glMinor = json.query(opengl, "minor").as<int>();
    settings.stencilBits = json.query(opengl, "stencilBits").as<int>();
    settings.depthBits = json.query(opengl, "depthBits").as<int>();
    settings.multisampleBuffer = json.query(opengl, "multisampleBuffer").as<int>();
    settings.multisampleSamples = json.query(opengl, "multisampleSamples").as<int>();

    window_.initialize(settings);

    targetDeltaTime = uint32_t(1.f / json.query("frameRate").as<int>());


    std::string shaderPrefix = json.query("shaderPrefix").as<const char*>();
    LOG_DEBUG << "shaderPrefix: " << shaderPrefix;

    /*
     * Create app states here
     * */
    stateStack_.registerState< GameState >(states::Game);
    stateStack_.registerState< PauseState >(states::Pause);
    stateStack_.pushState(states::Game);

    /*
     * Initialize Wren state
     * */
    wrenpp::VM::loadModuleFn = [this](const char* mod) -> char* {
        std::string path(mod);
        path += ".wren";
        const std::string& source = this->context_.textFileManager.get(path);
        char* buffer = (char*)malloc(source.size() + 1);
        PG_ASSERT(buffer != NULL);
        buffer[source.size()] = '\0';
        memcpy(buffer, source.c_str(), source.size());
        return buffer;
    };
    wrenpp::VM::writeFn = [](WrenVM* vm, const char* text) -> void {
        // this hack exists because Wren always prints an extra newline as a separate print statement.
        if (text[0] != '\n') {
            LOG_INFO << text;
        }
    };

    /*
     * Load resources that all app states will depend on
     **/
    //context_.textFileManager.addWatch(shader, false);
    // everything here should eventually go into a loading state
    context_.shaderManager.addShader(shaderPrefix + "/basic.vert.glsl", GL_VERTEX_SHADER);
    context_.shaderManager.addShader(shaderPrefix + "/basic.frag.glsl", GL_FRAGMENT_SHADER);
    context_.shaderManager.compile("basic");

    context_.shaderManager.addShader(shaderPrefix + "/specular.vert.glsl", GL_VERTEX_SHADER);
    context_.shaderManager.addShader(shaderPrefix + "/specular.frag.glsl", GL_FRAGMENT_SHADER);
    context_.shaderManager.compile("specular");

    context_.shaderManager.addShader(shaderPrefix + "/panel.vert.glsl", GL_VERTEX_SHADER);
    context_.shaderManager.addShader(shaderPrefix + "/panel.frag.glsl", GL_FRAGMENT_SHADER);
    context_.shaderManager.compile("panel");

    /*
     * Finally, set the application context
     * */
    context_.window = &window_;
    context_.imguiRenderer = new system::ImGuiRenderer(context_);

    context_.meshManager.initialize();

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

    // bind the scripting API so that Wren can find the methods
    wren::bindVectorModule();
    wren::bindMathModule();
    wren::bindRandomModule();
    wren::bindQuaternionModule();
    wren::bindEntityModule();
    wren::bindComponentModule();
    wren::bindNumberArrayModule();
    wren::bindRingBufferModule();
    wren::bindImguiModule();
    wren::bindUtilsModule();
    wren::bindSystemsModule();
}

}
