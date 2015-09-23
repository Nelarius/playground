# The ECS module

This module contains the engine's entity-component system (ECS). Here you will find
* Entity
* EntityManager
* SystemManager
* SystemManager
* as well as Events and Systems

Here are some important things to know. Currently, the chunk size of each component memory pool is controlled by the `EntityManager::PoolSize_` field. Ideally, all of the components used in the game would fit within one chunk. Additionally, `EntityManager` should expose the variable in the public API.

## Entity creation

Entities are created and dispensed by an instance of the `EntityManager` class:

```cpp
auto entity = entityManager.create();
```

This entity handle can be invalidated or destroyed, after which you cannot use the handle anymore. The entity can be copied in the usual way.

```cpp
entity2 = entity;

entity.invalidate();
entity.isValid();   // false
entity2.isValid();  // also false

entity.destroy();   // this will now fail
```

If you want to optionally store an entity, the default constructor of `Entity` will construct an invalid entity.

> The underlying handle within the entity class can be obtained by calling entity.id(), which returns the type Id. Id actually stores two uint32_t values. The first is the unique id of the entity, and the second is the version of the entity. The EntityManager instance which created the entity also stores a version for that entity. When you destroy the entity, the entity manager increments its version value. Thus the validity of the entity object can be easily checked by comparing the entity's version to the entity manager's version. If they differ, then the entity was destroyed.

## Using components

Data is contained in components, which are completely unadorned structs:

```cpp
struct Position {
    Vector3f position;
};
```

Components are assigned to entities by using `Entity::assign<C>( Args&&... args )`:

```cpp
entity.assign<Position>( std::initializer_list<float>{ 0.0f, 0.0f, 0.0f } );
```

Note that the component is created in memory using brace-syntax; the same parameters you would use in the aggregate initialization of the component struct must be passed to `Entity::assign<C>`.

Here are a few other template methods provided by Entity:

```cpp
// has the Position-component been assigned?
entity.has<Position>();
// remove the position component
entity.remove<Position>();
/*
 * The component method returns a handle.
 * The handle acts like a pointer to the desired component.
 * The actual type of 'pos' is ComponentHandle<Position>.
 * */
auto pos = entity.component<Position>();
pos->position = Vector3f( 1.0f, 1.0f, 1.0f );
```

By the way, when a component is destroyed in entity manager, the destructor for that component is called. You can safely place objects which require a destructor call in components.

> Using `Entity::component<C>` has O(1) time complexity. The pointer is obtained from a block of memory using the entity's unique id. See the next block quote for more details.

`EntityManager` provides an iterator interface for accessing entities. The iterators can be obtained by using the `join, join<C...>` methods.

```cpp
// iterate over all valid entities
for ( Entity entity: entityManager.join() ) {
    // do work
}

// iterate over all valid entities containing the Position component
for ( Entity entity: entityManager.join<Position>() ) {
    // do work
}

// iterate over all valid entities containing the Position and Velocity component
for ( Entity entity: entityManager.join<Position, Velocity>() ) {
    // do work
}
```

Iterating over entities in `EntityManager` has been implemented using two auxiliary classes: `Iterator` and `View`. `View` is used to select which entities to iterator over, and to provide the iterators:

```cpp
// add entities, components here ...
EntityManager::View v{ &entityManager };
view.v<Position, Velocity>();   // select only entities containing Position and velocity
for ( auto it = view.begin(); it != view.end(); ++it ) {
    // 
}
```

But the `join` method returns a `View` object, so that you don't have to write all that yourself.

> Components are also managed by the entity manager. The entity manager allocates a block of memory for each component type. The unique entity id is used as the offset for the component within the memory block. Thus, fetching the component will always have O(1) time complexity, with the trade off that more memory is allocated than strictly needed.

## Implement functionality using systems

Program logic is implemented using systems. Systems must inherit the class `System<S>`, where `S` is the deriving class itself. Two pure virtual methods can be overridden: `configure( EventManager& )` and `update( EventManager&, EntityManager&, float )`.

Logic is implemented in the `update`-method using the entity manager to gain access to components. For instance, a very simple movement system might be implemented as:

```cpp
struct Position {
    Vector3f position;
};

struct Velocity {
    Vector3f velocity;
};

class Body: public System<Body> {
    public:
        void configure( EventManager& events ) override {}
        void update( EventManager& events, EntityManager& entities, float dt ) override {
            for ( Entity entity: entities.join<Position, Velocity>() ) {
                entity.component<Position>()->position += 
					entity.component<Velocity>()->velocity * dt;
            }
        }
};
``` 

Different systems can be tied together using `SystemManager`:

```cpp
systemManager.add<Body>();
systemManager.configure<Body>();
systemManager.update<Body>( dt );
std::shared_ptr<System> body = systemManager.system<Body>();
```

### Communicating using events

Systems communicate between each other using events. In order for a system to receive events, it must inherit `Receiver` and implement the following method: `void receive( const E& event )`, where `E` is the type of event. Subscribe to the event using `EventManager::subscribe<E, S>( const S& )`, where `S` is your system. Events are emitted using `EventManager::emit<E, Args...>( Args&&... )`.

As an example, here's how you could implement a simple trigger system using systems, components, and events. We want a trigger event to fire when an entity wanders into a trigger zone (a rectangle). We define the following components and events:

```cpp
struct TriggerComponent {
    Rectangle area;
};

struct PositionComponent {
    Vector2f position;
};

struct VelocityComponent {
    Vector2f velocity;
    float factor;
};

struct TriggerEvent {
    Entity entity;
};
```

We want the trigger system to fire the event. It can do so in the `update` method.

```cpp
class Trigger: public System<Trigger> {
public:
    void update( EventManager& events, EntityManager& entities, float dt ) {
        // yucky O(N^2) implementation
        for ( auto posEnt: entities.join<PositionComponent>() ) {
            auto pos = entity.component<PositionComponent>();
            for ( auto triggerEnt: entities.join<TriggerComponent>() ) {
                auto rect = entity.component<TriggerComponent>();
                // if the entity's position is in the rectangle, then fire the event!
                if ( InRectangle( rect, pos  ) ) {
                    // pass the entity to the event's default constructor
                    events.emit<TriggerEvent>( posEnt );    
                }
            }
        }
    }
};
```

Now let's assume that when a trigger fires, the entity who triggered it will speed up by a factor of two. We want the body system to receive the event. We need the `configure`, `update` and `receive` methods to be defined. The body system must additionally inherit from `Receiver`:

```cpp
class Body: public System<Body>, Receiver {
public:
    void configure( EventManager& events ) override {
        // pass a reference to yourself so that the event manager can contact you :)
        events.subscribe<TriggerEvent>( *this );
    }
    
    void receive( const TriggerEvent& event ) {
        // speed the entity up!
        event.entity.component<VelocityComponent>()->factor *= 2.0f;
    }
    
    void update( EventManager& events, EntityManager& entities, float dt ) override {
        for ( Entity entity: entities.join<PositionComponent, VelocityComponent>() ) {
            auto pos = entity.component<PositionComponent>();
            auto vel = entity.component<VelocityComponent>();
            pos->position += vel->velocity * vel->factor * dt;
        }
    }
};
```

`EntityManager` emits the following events, regardless of whether any system is subscribed to them.

```cpp
/**
 * @class EntityCreatedEvent
 * @brief Emitted when an entity is created.
 */
struct EntityCreatedEvent {
    Entity entity;
};

/**
 * @class EntityDestroyedEvent
 * @brief Emitted just before the entity is destroyed.
 */
struct EntityDestroyedEvent {
    Entity entity;
};

/**
 * @class ComponentAssignedEvent
 * @brief Emitted when an entity is assigned to a component.
 */
template<typename C>
struct ComponentAssignedEvent {
    Entity entity;
    ComponentHandle<C> component;
};

/**
 * @class ComponentRemovedEvent
 * @brief Emitted just before the component is removed.
 */
template<typename C>
struct ComponentRemovedEvent {
    Entity entity;
    ComponentHandle<C> component;
};
```

## The three manager classes

Here's the order in which you have to instantiate the three manager classes.

```cpp
pg::ecs::EventManager eventManager{};
pg::ecs::EntityManager entityManager { eventManager };
pg::ecs::SystemManager systemManager { eventManager, systemManager };
```
