#include "app/Window.h"
#include "utils/Assert.h"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include <cstdlib>

using pg::Window;
using pg::WindowSettings;

Window::Window()
    :   window_( nullptr ),
        glContext_(),
        name_( "Playground engine" ),
        width_( 800 ),
        height_( 600 ),
        glMajor_( 4 ),
        glMinor_( 3 ),
        stencilBits_( 8 ),
        depthBits_( 24 ),
        msBuffer_( 1 ),
        msSamples_( 4 )
        {}

Window::~Window() {
    SDL_DestroyWindow( window_ );
    SDL_Quit();
}

void Window::initialize_() {
    initializeSDL_();
    initializeOpenGL_();
}

void Window::initializeSDL_() {
    SDL_Init( SDL_INIT_EVERYTHING );

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, glMajor_ );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, glMinor_ );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, depthBits_ );
    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, stencilBits_ );
    //anti-aliasing
    SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 );
    SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 4 );

    window_ =   SDL_CreateWindow(
                    name_.c_str(),
                    SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED,
                    width_, height_,
                    SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
                );
    glContext_ = SDL_GL_CreateContext( window_ );
}

void Window::initializeOpenGL_() {
    //initialize GLEW
    glewExperimental = GL_TRUE;

    if ( glewInit() ) {
        std::cerr << "glewInit failed." << std::endl;
        std::exit( EXIT_FAILURE );
    }

    glEnable( GL_DEPTH_TEST );
    //enable culling
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0 );

    glDepthFunc( GL_LEQUAL );
    glEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS );

    std::cout << "OpenGL version: " << glGetString( GL_VERSION ) << std::endl;
    std::cout << "GLSL version: " << glGetString( GL_SHADING_LANGUAGE_VERSION ) << std::endl;
    std::cout << "Vendor: " << glGetString( GL_VENDOR ) << std::endl;
    std::cout << "Renderer: " << glGetString( GL_RENDERER ) << std::endl << std::endl;
    
    glClearColor(
        clearColorR_,
        clearColorG_,
        clearColorB_,
        clearColorA_
    );
}

void Window::display() {
    SDL_GL_SwapWindow( window_ );
}

void Window::initialize( const WindowSettings& settings ) {
    width_ = settings.width;
    height_ = settings.height;
    name_ = settings.name;
    glMajor_ = settings.glMajor;
    glMinor_ = settings.glMinor;
    stencilBits_ = settings.stencilBits;
    depthBits_ = settings.depthBits;
    msBuffer_ = settings.multisampleBuffer;
    msSamples_ = settings.multisampleSamples;
    clearColorR_ = settings.clearColor.r;
    clearColorG_ = settings.clearColor.g;
    clearColorB_ = settings.clearColor.b;
    clearColorA_ = settings.clearColor.a;
    initialize_();
}

unsigned Window::width() const {
    return width_;
}

unsigned Window::height() const {
    return height_;
}

SDL_Window* Window::SDLwindow() {
    return window_;

}