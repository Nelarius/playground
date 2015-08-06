#include "app/Application.h"
#include "GameState.h"
#include "lua/LuaState.h"
#include "utils/Assert.h"
#include "utils/File.h"
#include "utils/Log.h"
#include <SDL2/SDL_mouse.h>
#include <GL/glew.h>
extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}
#include <LuaBridge/LuaBridge.h>
#include <string>
#include <chrono>
#include <thread>
#include <cmath>

#include <iostream>

namespace pg {

namespace {
    std::chrono::duration<float, std::ratio<1,1>> TargetDeltaTime{ 0.016667f };
}

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
        updateContext_();
        context_.time_ = time.count();
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

        /*if ( SDL_GetMouseState( NULL, NULL ) & SDL_BUTTON(SDL_BUTTON_LEFT) ) {
            camera_.orbit( context_.mouse().dx, context_.mouse().dy );
        } else if ( SDL_GetMouseState( NULL, NULL ) & SDL_BUTTON(SDL_BUTTON_RIGHT) ) {
            camera_.translate( context_.mouse().dx, context_.mouse().dy );
        }*/

        /*for (auto& event: context_.events_ ) {
            if ( event.type == SDL_MOUSEWHEEL ) {
                camera_.dolly( 0.5f*event.wheel.y );
            }
        }*/
        
        stateStack_.update( dt.count() );
        
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        stateStack_.render( dt.count() );
        
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
    pg::LuaState lua{ false };
    lua.execute( "config.lua" );
    
    luabridge::LuaRef targetFrameRate = luabridge::getGlobal( lua.get(), "targetFrameRate" );
    luabridge::LuaRef window = luabridge::getGlobal( lua.get(), "window" );
    luabridge::LuaRef opengl = window["opengl"];
    
    TargetDeltaTime = std::chrono::duration<float, std::ratio<1,1>>( 1.0f /  targetFrameRate.cast<float>() );
    
    WindowSettings settings{};
    settings.width = window["width"].cast<int>();
    settings.height = window["height"];
    settings.name = window["name"].cast<std::string>();
    settings.glMajor = opengl["major"];
    settings.glMinor = opengl["minor"];
    settings.stencilBits = opengl["stencil_bits"];
    settings.depthBits = opengl["depth_bits"];
    settings.multisampleBuffer = opengl["multisample_buffers"];
    settings.multisampleSamples = opengl["multisample_samples"];
    
    window_.initialize( settings );
    
    context_.window = &window_;
    
    stateStack_.registerState<GameState>( states::Game );
    stateStack_.pushState( states::Game );
}

void Application::updateContext_() {
    //context_.events_.clear();

    // update real-time input
    int oldx = context_.mouse_.x;
    int oldy = context_.mouse_.y;
    int newx, newy;
    SDL_GetMouseState( &newx, &newy );
    context_.mouse_.dx = newx - oldx;
    context_.mouse_.dy = newy - oldy;
    context_.mouse_.x = newx;
    context_.mouse_.y = newy;

    // update events
    /*SDL_Event event;
    while( SDL_PollEvent( &event ) ) {
        // close is the only event that the application shell captures & handles
        if ( event.type == SDL_QUIT ) {
            running_ = false;
        } else {
            context_.events_.push_back( event );
        }
    }*/
}



}