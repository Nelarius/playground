
#pragma once

#include <SDL2/SDL_video.h>
#include <string>

namespace pg {

struct WindowSettings {
    WindowSettings() = default;
    ~WindowSettings() = default;
    
    int width{ 800 };
    int height{ 600 };
    std::string name{ "Crowd engine" };
    int glMajor{ 3 };
    int glMinor{ 1 };
    int stencilBits{ 8 };
    int depthBits{ 24 };
    int multisampleBuffer{ 1 };
    int multisampleSamples{ 4 };
    typedef struct {
        float r, g, b, a;
    } Color;
    Color clearColor{ 0.0f, 0.0f, 0.0f, 1.0f };
};

/// @brief A platform-independent window.
/// This window contains a valid OpenGL 4.3 context.
class Window {
    public:
        /// @brief Construct the window.
        /// The OpenGL context and window are ready for use after construction.
        Window();
        ~Window();

        Window( const Window& )             = delete;
        Window& operator=( const Window& )  = delete;
        Window( Window&& )                  = delete;
        Window& operator=( Window&& )       = delete;

        /// @brief Display the window and any changes made to the back buffer.
        void display();
        
        /// @brief Initialize the window with window settings
        void initialize( const WindowSettings& settings );

        unsigned width() const;
        unsigned height() const;

    private:
        void initialize_();
        void initializeSDL_();
        void initializeOpenGL_();

        SDL_Window*     window_;
        SDL_GLContext   glContext_;
        std::string     name_;
        int             width_;
        int             height_;
        int             glMajor_;
        int             glMinor_;
        int             stencilBits_;
        int             depthBits_;
        int             msBuffer_;  // multisample buffer
        int             msSamples_; // number of multisamples
        float           clearColorR_;
        float           clearColorB_;
        float           clearColorG_;
        float           clearColorA_;

};

}   //namespace ce

