# Core application facilities

This module contains
* the main application class which gets executed from `main()`
* the window wrapper, which application uses
* the general application context
  * Why do we need a context? The idea is to store instances of managers which should only exist once within the context. You, as a user, never create the manager, but you access the context and use the services that it provides.
  * Note that the context is shared across the different application states.
  * Resource managers from the `manager` module contained in the context
* keyboard and mouse event managers 
* application state control
* input and output logic for the world state

## Reading and writing the world

Currently, the world state can only be read in. The world state is stored as a single JSON file. Here's how you use it:

```cpp
// context is owned by the application class
WorldIO world{ context };
// entities, events are declared elsewhere
// they are ecs::EntityManager and ecs::EventManager, respectively
world.read( "data/scene.json", entities, events );
```

## Controlling application state

Use `AppStateStack` to control the the execution flow. Register The various app states using `AppStateStack::registerState`. Push an app state onto the stack using `AppStateStack::pushState`.

During the game loop, there are two methods that need to be called. Event handling is done in `AppStateStack::handleEvent`. During this method call, events are delegated to all app states on the state stack. The states on the stack request modifications on the state stack. For this reason, `AppStateStack ` calls an internal `AppStateStack::applyPendingChanges_` at the end of the `handleEvent` call. The second game loop method call is  `update`. It calls the update method on all current app states, which in turn should call the update method on all the owned systems.

## Commands

A Command is a simple class containing two callables. The first callable executes the command, the second one undoes it.

## Handling mouse events

You can use `MouseEvents` to trigger specific commands. Set a command for a specific mouse button and event by calling the `*set` methods.

Call `MouseEvents::update` in the update-handling portion of your loop. This is when `MouseEvents` will check if any state has changed and fire any commands.

## Handling keyboard events

`KeyboardManager` is a class which can conveniently handle keyboard events for you. You register callbacks with the manager, and then, given an SDL event, it will call the appropriate callback.

#### Additional state

The `app/KeyboardManager.cpp` has a static map.
