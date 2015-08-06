
#pragma once

#include "app/Window.h"
#include "app/Context.h"
#include "app/AppStateStack.h"
#include <memory>

namespace pg {

/**
 * @class Application
 * @author Muszynski Johann M
 * @date 05/12/15
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
        void updateContext_();

        bool            running_{ false };
        Window          window_{};
        Context         context_{};
        
        AppStateStack   stateStack_{ context_ };
};

}

