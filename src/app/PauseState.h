#pragma once

#include "app/AppState.h"
#include "app/Context.h"

namespace pg {

// when pushed on the state stack, this will not let any other state below it
// update
class PauseState : public AppState {
public:
    PauseState(Context& context, AppStateStack& stack);
    PauseState() = default;
    virtual ~PauseState() = default;
    void activate() override;
    void render(float dt) override;
    bool update(float dt) override;
    bool handleEvent(const SDL_Event& event) override;
};

}