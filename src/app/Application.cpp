#include "app/Application.h"
#include "app/GameState.h"
#include "app/PauseState.h"
#include "utils/Assert.h"
#include "utils/File.h"
#include "utils/Log.h"
#include "Wrenly.h"
#include "wren/Include.h"
#include "json11/json11.hpp"
#include <string>
#include <chrono>
#include <thread>

namespace {
    std::chrono::duration<float, std::ratio<1,1>> TargetDeltaTime{ 0.016667f };
}

namespace pg {

void Application::run() { 
    initialize_();

    running_ = true;
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float, std::ratio<1,1>> time( 0.0f );

    while( running_ ) {
        auto newTime = std::chrono::steady_clock::now();
        std::chrono::duration<float, std::ratio<1,1>> dt = newTime - currentTime;
        currentTime = std::chrono::steady_clock::now();
        /*
         * Update mouse current mouse coordinates here
         * */
        mouse_.update();
        /*
         * Handle events here
         * */
        SDL_Event event;
        while( SDL_PollEvent( &event ) ) {
            stateStack_.handleEvent( event );
        }
        /*
         * A state might have called quits
         * */
        if ( !context_.running ) {
            running_ = false;
        }

        context_.imguiRenderer->newFrame( dt.count(), mouse_.getMouseCoords().x, mouse_.getMouseCoords().y );
        context_.textFileManager.update();
        stateStack_.update( dt.count() );

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        stateStack_.render( dt.count() );
        context_.imguiRenderer->render();

        window_.display();
        time += dt;

        /*
         * Sleep for the remainder of the frame, if we have time for it
         * */
        auto remainder = TargetDeltaTime - (std::chrono::steady_clock::now() - newTime);
        if ( remainder.count() > 0.0f ) {
            std::this_thread::sleep_for( remainder );
        }
    }
}

void Application::initialize_() {
    /*
     * Initialize app state here
     * */
    auto json = pg::FileToString( "config.json" );
    std::string error{""};
    auto obj = json11::Json::parse( json, error ).object_items();
    auto window = obj["window"].object_items();
    auto opengl = window["opengl"].object_items();

    TargetDeltaTime = std::chrono::duration<float, std::ratio<1,1>>( 1.0f /  obj["frameRate"].number_value() );

    WindowSettings settings{};
    settings.width = window["width"].int_value();
    settings.height = window["height"].int_value();
    settings.name = window["name"].string_value();
    settings.glMajor = opengl["major"].int_value();
    settings.glMinor = opengl["minor"].int_value();
    settings.stencilBits = opengl["stencil_bits"].int_value();
    settings.depthBits = opengl["depth_bits"].int_value();
    settings.multisampleBuffer = opengl["multisample_buffers"].int_value();
    settings.multisampleSamples = opengl["multisample_samples"].int_value();

    window_.initialize( settings );

    /*
     * Create app states here
     * */
    stateStack_.registerState< GameState >( states::Game );
    stateStack_.registerState< PauseState >( states::Pause );
    stateStack_.pushState( states::Game );

    /*
     * Initialize Wren state
     * */
     wrenly::Wren::loadModuleFn = [this]( const char* mod ) -> char* {
        std::string path( mod );
        path += ".wren";
        const std::string& source = this->context_.textFileManager.get( path );
        char* buffer = (char*) malloc( source.size() + 1 );
        buffer[source.size()] = '\0';
        memcpy( buffer, source.c_str(), source.size() );
        return buffer;
    };
    wrenly::Wren::writeFn = []( WrenVM* vm, const char* text ) -> void {
        LOG_INFO << text;
    };

    /*
     * Load resources that all app states will depend on
     * */
    context_.textFileManager.addWatch( "data", false );
    // everything here should eventually go into a loading state
    context_.shaderManager.addShader( "data/basic.vert.glsl", GL_VERTEX_SHADER );
    context_.shaderManager.addShader( "data/basic.frag.glsl", GL_FRAGMENT_SHADER );
    context_.shaderManager.compile( "basic" );

    context_.shaderManager.addShader( "data/specular.vert.glsl", GL_VERTEX_SHADER );
    context_.shaderManager.addShader( "data/specular.frag.glsl", GL_FRAGMENT_SHADER );
    context_.shaderManager.compile( "specular" );

    context_.shaderManager.addShader( "data/panel.vert.glsl", GL_VERTEX_SHADER );
    context_.shaderManager.addShader( "data/panel.frag.glsl", GL_FRAGMENT_SHADER );
    context_.shaderManager.compile( "panel" );

    /*
     * Finally, set the application context
     * */
    context_.window = &window_;
    context_.imguiRenderer = new system::ImGuiRenderer( context_ );

    mouse_.setPressCallback(
        SDL_BUTTON_LEFT,
        Command(
            [ this ] () -> void {
                this->context_.imguiRenderer->mouseButtonPressed( SDL_BUTTON_LEFT );
            },
            std::function<void()>()
        )
    );
    mouse_.setReleaseCallback(
        SDL_BUTTON_LEFT,
        Command(
            [ this ] () -> void {
                this->context_.imguiRenderer->mouseButtonReleased( SDL_BUTTON_LEFT );
            },
            std::function<void()>()
        )
    );
}

}