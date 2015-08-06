#include "app/AppStateStack.h"
#include "utils/Assert.h"

namespace pg {

AppStateStack::AppStateStack( Context& context )
:	context_( context ),
	stack_(),
	factories_()
	{}

void AppStateStack::render( float dt ) {
    for ( auto& ptr: stack_ ) {
        ptr->render( dt );
    }
}

void AppStateStack::update( float dt ) {
    for ( auto it = stack_.rbegin(); it != stack_.rend(); ++it ) {
        if ( !(*it)->update( dt ) ) {
            break;
        }
    }
}

void AppStateStack::handleEvent( const SDL_Event& event ) {
    for ( auto it = stack_.rbegin(); it != stack_.rend(); ++it ) {
        if ( !(*it)->handleEvent( event ) ) {
            break;
        }
    }
    applyPendingChanges_();
}

void AppStateStack::pushState( states::Id id ) {
    pendingList_.emplace_back( states::Push, id );
} 

void AppStateStack::popState() {
    pendingList_.emplace_back( states::Pop );
}

void AppStateStack::clearStates() {
    pendingList_.emplace_back( states::Clear );
}

bool AppStateStack::isEmpty() const {
    return stack_.empty();
}

std::unique_ptr<AppState> AppStateStack::createState_( states::Id id ) {
    auto it = factories_.find( id );
    ASSERT( it != factories_.end(), "AppStateStack::createState_> state not found" );
    //executes the function, returns the constructed managed pointer
    return it->second();
}

void AppStateStack::applyPendingChanges_() {
    for ( PendingChange& change: pendingList_ ) {
        switch( change.action ) {
            case states::Push:
            {
                stack_.push_back( createState_( change.id ) );
                stack_.at( stack_.size() - 1u )->activate();
                break;
            }
            case states::Pop:
                stack_.pop_back();
                break;
            case states::Clear:
                stack_.clear();
                break;
        }
    }
    pendingList_.clear();
}

AppStateStack::PendingChange::PendingChange( states::Action action, states::Id id )
:   action( action ),
    id( id )
    {}


}