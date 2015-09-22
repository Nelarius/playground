# Core application facilities

This module contains
* the main application class which gets executed from `main()`
* the window wrapper, which application uses
* the general application context
  * resource managers from the `manager` module contained in the context
* application state control

## Controlling application state

Use `AppStateStack` to control the the execution flow. Register The various app states using `AppStateStack::registerState`. Push an app state onto the stack using `AppStateStack::pushState`.

During the game loop, there are two methods that need to be called. Event handling is done in `AppStateStack::handleEvent`. During this method call, events are delegated to all app states on the state stack. The states on the stack request modifications on the state stack. For this reason, `AppStateStack ` calls an internal `AppStateStack::applyPendingChanges_` at the end of the `handleEvent` call. The second game loop method call is  `update`. It calls the update method on all current app states, which in turn should call the update method on all the owned systems.
