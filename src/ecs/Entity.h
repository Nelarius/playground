
#pragma once

#include "ecs/Id.h"
#include "utils/Pool.h"
#include "ecs/Component.h"
#include "ecs/Event.h"
#include "utils/Log.h"
#include <vector>
#include <memory>
#include <iterator>
#include <cstdint>
#include <bitset>

namespace ce {
namespace ecs {

class EntityManager;

/**
 * @class ComponentHandle
 * @author Muszynski Johann M
 * @date 23/07/15
 * @file Component.h
 * @brief A wrapper around a component instance.
 * This handle will be invalidated if:
 * - the component is removed from the host entity
 * - the host entity is destroyed
 */
template<typename C>
class ComponentHandle {
    public:
        ComponentHandle() = default;
        ComponentHandle( EntityManager* manager, Id id )
        :   manager_( manager ),
            id_( id )
            {}
        ComponentHandle( const ComponentHandle& )               = default;
        ComponentHandle( ComponentHandle&& )                    = default;
        ComponentHandle& operator=( const ComponentHandle& )    = default;
        ComponentHandle& operator=( ComponentHandle&& )         = default;
        
        C* operator->();
        const C* operator->() const;
        bool valid() const;
        bool operator==( const ComponentHandle<C>& rhs ) const;
        bool operator!=( const ComponentHandle<C>& rhs ) const;

    private:
        EntityManager*  manager_{ nullptr };
        Id              id_{ Invalid };
};

class Entity {
    public:
        Entity( EntityManager* manager, Id id );
        Entity()                            = delete;
        Entity( const Entity& )	            = default;
        Entity( Entity&& )                  = default;
        Entity& operator=( const Entity& )  = default;
        Entity& operator=( Entity&& )       = default;
        ~Entity()                           = default;
        
        bool operator==( const Entity& ) const;
        bool operator!=( const Entity& ) const;
        bool operator>( const Entity& ) const;
        bool operator<( const Entity& ) const;
        
        /**
         * @brief Invalidate this handle, does nothing to the underlying entity.
         */
        void invalidate();
        /**
         * @brief Destroy the entity this handle points to, removing all associated components.
         */
        void destroy();
        /**
         * @brief Check if this is a valid handle.
         * @return True if this handle is still valid, false otherwise.
         */
        bool isValid() const;
        /**
         * @brief Get this handle's id.
         * @return 
         */
        Id id() const;
        /**
         * @brief Assign a component of type C to this entity.
         * @return A component handle, which acts like a pointer to the assigned component.
         */
        template<typename C, typename... Args>
        ComponentHandle<C> assign( Args&& ... args );
        /**
         * @brief Remove a component of type C from this entity.
         */
        template<typename C>
        void remove();
        /**
         * @brief Get a component of type C assigned to this entity.
         * @return A component handle, which acts like a pointer to the assigned component.
         */
        template<typename C>
        ComponentHandle<C> component() const;
        /**
         * @brief Check if this entity has an assigned component of type C.
         * @return True, if component of type C has been assigned, false otherwise.
         */
        template<typename C>
        bool has() const;
        
    private:
        // friend of EntityManager 
        EntityManager*  manager_{ nullptr };
        Id              id_{ Invalid };
};

// this should be a run-time adjustable
// to do so, EntityManager needs to use the Cheshire Cat pattern
const uint32_t MaxComponents = 16u;

/**
 * @class EntityCreatedEvent
 * @author Muszynski Johann M
 * @date 28/07/15
 * @file Entity.h
 * @brief Emitted when an entity is created.
 */
struct EntityCreatedEvent {
    Entity entity;
};

/**
 * @class EntityDestroyedEvent
 * @author Muszynski Johann M
 * @date 28/07/15
 * @file Entity.h
 * @brief Emitted just before the entity is destroyed.
 */
struct EntityDestroyedEvent {
    Entity entity;
};

/**
 * @class ComponentAssignedEvent
 * @author Muszynski Johann M
 * @date 28/07/15
 * @file Entity.h
 * @brief Emitted when an entity is assigned to a component.
 */
template<typename C>
struct ComponentAssignedEvent {
    Entity entity;
    ComponentHandle<C> component;
};

/**
 * @class ComponentRemovedEvent
 * @author Muszynski Johann M
 * @date 28/07/15
 * @file Entity.h
 * @brief Emitted just before the component is removed.
 */
template<typename C>
struct ComponentRemovedEvent {
    Entity entity;
    ComponentHandle<C> component;
};

/**
 * @class EntityManager
 * @author Nelarius
 * @date 06/14/15
 * @file EntityManager.h
 * @brief 
 * 
 * This class is uncopyable because it should remain the sole owner of all the contained entities and components.
 */
class EntityManager {
    public:
        /**
         * @brief Initialize with the number of elements to store in one chunk in the pool.
         * @param poolSize The number of elements reserved in one chunk of memory. 64 by default.
         */
        EntityManager( EventManager&, uint32_t poolSize = 64 );
        ~EntityManager();
        
        EntityManager( const EntityManager& )               = delete;
        EntityManager( EntityManager&& )                    = delete;
        EntityManager& operator=( const EntityManager& )    = delete;
        EntityManager& operator=( EntityManager&& )         = delete;
        /**
         * @brief Create a new entity.
         * @return A handle to the new Entity.
         * Calling this may result in an occasional memory allocation for new components.
         */
        Entity create();
        /**
         * @brief Destroy all entities and components associated with this manager.
         */
        void reset();
        /**
         * @brief Get the number of valid entities contained.
         */
        std::size_t size() const;
        
        /**
         * @class Iterator
         * @author Muszynski Johann M
         * @date 24/07/15
         * @file Entity.h
         * @brief An iterator over entities with specific components.
         * Performance note: this should only be used in range-based loops.
         * This way, only two iterators are created, and we minimize the overhead
         * of allocating memory for the contained vector.
         */
        class Iterator {
            public:
                Iterator( EntityManager* owner, std::uint32_t index ) 
                :   owner_( owner ), 
                    index_( index ),
                    componentIndices_()
                    {}
                Iterator( EntityManager* owner, std::uint32_t index, std::vector<uint32_t>&& components )
                :   owner_( owner ),
                    index_( index ),
                    componentIndices_( std::move( components ) )
                    {}
                Iterator()                              = delete;
                Iterator( const Iterator& )             = default;
                Iterator( Iterator&& )                  = default;
                Iterator& operator=( const Iterator& )  = default;
                Iterator& operator=( Iterator&& )       = default;
                ~Iterator()                             = default;
                
                bool operator==( const Iterator& rhs ) { return index_ == rhs.index_; }
                bool operator!=( const Iterator& rhs ) { return index_ != rhs.index_; }
                Iterator& operator++() { // prefix operator
                    uint32_t stop = owner_->componentMasks_.size();
                    bool incorrectComponents = false;
                    do {
                        index_++;
                        for ( uint32_t family: componentIndices_ ) {
                            if ( !owner_->componentMasks_[index_].test( family ) ) {
                                incorrectComponents = true;
                                break;
                            }
                        }
                    } while ( incorrectComponents && index_ < stop );
                    return *this;
                }
                Entity operator*() { return Entity( owner_, Id( index_, owner_->entityVersions_[index_] ) ); }
                
            private:                
                EntityManager*              owner_;
                std::uint32_t               index_;
                std::vector<std::uint32_t>  componentIndices_;
        };
        
        /**
         * @class View
         * @author Muszynski Johann M
         * @date 24/07/15
         * @file Entity.h
         * @brief A view of entities containing none or more components.
         * Use this to get the iterators that you want.
         */
        class View {
            public:
                View( EntityManager* owner )
                :   owner_( owner ),
                    componentIndices_()
                    {}
                View()                          = delete;
                ~View()                         = default;
                View( const View& )             = default;
                View( View&& )                  = default;
                View& operator=( const View& )  = default;
                View& operator=( View&& )       = default;
                
                template<typename C>
                void view() {
                    componentIndices_.push_back( Component<C>::family() );
                }
                /**
                 * @brief Filter the viewed entities based on component type.
                 * By not calling this method, we have a view of all currently valid entities.
                 */
                template<typename C1, typename C2, typename... Components>
                void view() {
                    view<C1>();
                    view<C2, Components...>();  // recursive!
                }
                /**
                 * @brief Return an iterator to the beginning of the view.
                 * @return 
                 * Note that by calling this method, the view is returned to the unfiltered state.
                 */
                Iterator begin() {
                    return Iterator( owner_, windForward_(), std::move( componentIndices_ ));
                }
                Iterator end() { return Iterator( owner_, owner_->componentMasks_.size() ); }
            
            private:
                bool indexContainsComponents_( uint32_t index ) const {
                    for ( uint32_t i: componentIndices_ ) {
                        if ( !owner_->componentMasks_[index].test(i) ) {
                            return false;
                        }
                    }
                    return true;
                }
                uint32_t windForward_() const {
                    if ( componentIndices_.size() == 0u ) {
                        return 0u;
                    }
                    uint32_t index = 0u;
                    while ( !indexContainsComponents_( index ) && index < owner_->componentMasks_.size() ) {
                        index++;
                    }
                    return index;
                }
                
                EntityManager*          owner_;
                // these elements index into EntityManager::componentPools_ and 
                // the component masks in EntityManager::componentMasks_
                std::vector<uint32_t>   componentIndices_;
        };
        
        /**
         * @brief Join all valid entities.
         * @return A view, which will iterate over all valid entities in a range-based loop. 
         */
        View join();
        template<typename C>
        View join();
        /**
         * @brief Join all entities with a specific component type.
         * @return A view, which will iterate over all valid entities composed of the given types.
         */
        template<typename C1, typename C2, typename... Components>
        View join();
        
    private:
        friend class Entity;
        template<typename C> friend class ComponentHandle;
        
        void destroy_( Id id );
        bool isValid_( Id id ) const;
        void accommodateEntity_( uint32_t index );
        
        // move template code out so that this file is still human-readable
        template<typename C> 
        void accommodateComponent_();
        
        template<typename C, typename... Args> 
        ComponentHandle<C> assign_( Id id, Args&&... args ); 
        
        template<typename C>
        void remove_( Id id );  // remove a component
        template<typename C>
        C* component_( Id id );    // the component getter
        template<typename C>
        bool hasComponent_( Id id ) const;
        
        const uint32_t                              PoolSize_{ 64 };
        uint32_t                                    indexCounter_{ 0u };
        std::vector<std::unique_ptr<BasePool>>      componentPools_{};
        std::vector<std::bitset<MaxComponents>>     componentMasks_{};
        std::vector<uint32_t>                       entityVersions_{};
        std::vector<uint32_t>                       freeList_{};
        EventManager&                               mailMan_;
};

/////////////////////////////////////////////////////////////////////////////
// ComponentHandle implementation
/////////////////////////////////////////////////////////////////////////////
template<typename C>
C* ComponentHandle<C>::operator->() {
    return manager_->component_<C>( id_ );
}

template<typename C>
const C* ComponentHandle<C>::operator->() const {
    return manager_->component_<C>( id_ );
}

template<typename C>
bool ComponentHandle<C>::valid() const {
    return manager_->isValid_( id_ );
}

template<typename C>
bool ComponentHandle<C>::operator==( const ComponentHandle<C>& rhs ) const {
    return manager_ == rhs.manager_ && id_ == rhs.id_;
}

template<typename C>
bool ComponentHandle<C>::operator!=( const ComponentHandle<C>& rhs ) const {
    return manager_ != rhs.manager_ && id_ != rhs.id_;
}

/////////////////////////////////////////////////////////////////////////////
// Entity implementation
/////////////////////////////////////////////////////////////////////////////
template<typename C>
ComponentHandle<C> Entity::component() const {
    ASSERT( isValid(), "Entity::component> Try to access component of an invalid entity." );
    return ComponentHandle<C>{ manager_, id_ };
}

template<typename C, typename... Args>
ComponentHandle<C> Entity::assign( Args&& ... args) {
    ASSERT( isValid(), "Entity::assign> Try to assign component to an invalid entity." );
    return manager_->assign_<C>( id_, std::forward<Args>( args )... );
}

template<typename C>
void Entity::remove() {
    if ( !isValid() ) {
        LOG(ce::LogLevel::Error) << "Tried to remove component from invalid entity.";
        return;
    }
    manager_->remove_<C>( id_ );
}

template<typename C>
bool Entity::has() const {
    ASSERT( isValid(), "Entity::has> Checking components on an invalid entity." );
    return manager_->hasComponent_<C>( id_ );
}

/////////////////////////////////////////////////////////////////////////////
// EntityManager implementation
/////////////////////////////////////////////////////////////////////////////
template<typename C>
EntityManager::View EntityManager::join() {
    View view { this };
    view.view<C>();
    return view;
}

template<typename C1, typename C2, typename... Components>
EntityManager::View EntityManager::join() {
    View v{ this };
    v.view<C1>();
    v.view<C2, Components...>();
    return v;
}

template<typename C>
void EntityManager::accommodateComponent_() {
    const unsigned family = Component<C>::family();
    while( family >= componentPools_.size() ) {
        componentPools_.emplace_back( new Pool<C>( PoolSize_ ) );
    }
}

template<typename C, typename... Args>
ComponentHandle<C> EntityManager::assign_( Id id, Args&&... args ) {
    ASSERT( isValid_(id), "EntityManager::assign_> tried to assign component to invalid entity." );
    const int family = Component<C>::family();
    accommodateComponent_<C>(); // create a new component pool, if not already done
    new ( componentPools_[family]->at( id.index() ) ) C{ std::forward<Args>( args )... };
    
    if ( componentMasks_[id.index()].test( family ) ) {
        // if there already is a component, then log it and return the existing component
        LOG(ce::LogLevel::Error) << "Tried to assign a component on top of an already existing one!";
        return ComponentHandle<C>{ this, id };
    }
    
    mailMan_.emit<ComponentAssignedEvent<C>>( Entity{ this, id }, ComponentHandle<C>{ this, id } );
    
    componentMasks_[id.index()].set( family );
    return ComponentHandle<C>{ this, id };
}

template<typename C>
void EntityManager::remove_( Id id ) {
    if ( !isValid_( id ) ) {
        LOG(ce::LogLevel::Error) << "Tried to remove component from invalid entity.";
        return;
    }
    mailMan_.emit<ComponentRemovedEvent<C>>( Entity { this, id }, ComponentHandle<C>{ this, id } );
    const int family = Component<C>::family();
    const uint32_t index = id.index();
    componentPools_[family]->destroy( index );
    componentMasks_[ index ].reset( family );
}

template<typename C>
C* EntityManager::component_( Id id ) {
    ASSERT( isValid_( id ), "EntityManager::component_> tried to get component of an invalid entity" );
    const unsigned family = Component<C>::family();
    return static_cast<C*>( componentPools_[family]->at( id.index() ) );
}

template<typename C>
bool EntityManager::hasComponent_( Id id ) const {
    ASSERT( isValid_( id ), "EntityManager::hasComponent_> tried to check component of an invalid entity" );
    const unsigned family = Component<C>::family();
    return componentMasks_[id.index()].test( family );
}

}   // namespace ecs
}   // namespace ce

