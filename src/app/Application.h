
#pragma once

#include "app/Window.h"
#include "app/Context.h"
#include "app/AppStateStack.h"
#include "app/MouseEvents.h"
#include <memory>

namespace pg {

/**
 * @class Application
 * @file Application.h
 * @brief Wraps all systems and managers, contains the main game loop.
 */
class Application {
    public:
        Application() = default;
        ~Application() = default;

        /**
         * @brief Execute the main game loop.
         */
        void run();

    private:
        void initialize_();

        bool            running_{ false };
        Window          window_{};
        Context         context_{};
        MouseEvents     mouse_{};

        AppStateStack   stateStack_{ context_ };
};

}
