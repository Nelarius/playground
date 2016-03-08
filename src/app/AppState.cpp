#include "app/AppState.h"
#include "app/AppStateStack.h"

using pg::Context;
using pg::AppState;
using pg::AppStateStack;

AppState::AppState(Context& context, AppStateStack& stack)
    : context_(context),
    keyboard_(),
    mouse_(context),
    stack_(stack)
{}

void AppState::requestStackClear_() {
    stack_.clearStates();
}

void AppState::requestStackPop_() {
    stack_.popState();
}

void AppState::requestStackPush_(pg::states::Id id) {
    stack_.pushState(id);
}
