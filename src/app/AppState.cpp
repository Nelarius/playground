#include "app/AppState.h"
#include "app/AppStateStack.h"

using ce::Context;
using ce::AppState;
using ce::AppStateStack;

AppState::AppState( Context& context, AppStateStack& stack  )
:	context_( context ),
	stack_( stack )
	{}

void AppState::requestStackClear_() {
    stack_.clearStates();
}

void AppState::requestStackPop_() {
	stack_.popState();
}

void AppState::requestStackPush_( ce::states::Id id ) {
    stack_.pushState( id );
}
