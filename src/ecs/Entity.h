
#pragma once

#include "ecs/Id.h"
#include "utils/MemoryArena.h"
#include "ecs/Component.h"
#include "ecs/Event.h"
#include "utils/Log.h"
#include <vector>
#include <memory>
#include <iterator>
#include <cstdint>
#include <bitset>

namespace pg {
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
    ComponentHandle(EntityManager* manager, Id id)
        : manager_(manager),
        id_(id)
    {}
    ComponentHandle(const ComponentHandle&) = default;
    ComponentHandle(ComponentHandle&&) = default;
    ComponentHandle& operator=(const ComponentHandle&) = default;
    ComponentHandle& operator=(ComponentHandle&&) = default;

    C* operator->();
    const C* operator->() const;
    C& operator*();
    const C& operator*() const;
    bool isValid() const;
    bool operator==(const ComponentHandle<C>& rhs) const;
    bool operator!=(const ComponentHandle<C>& rhs) const;

private:
    EntityManager*  manager_{ nullptr };
    Id              id_{ Invalid };
};

class Entity {
public:
    Entity(EntityManager* manager, Id id);
    Entity() = default;
    Entity(const Entity&) = default;
    Entity(Entity&&) = default;
    Entity& operator=(const Entity&) = default;
    Entity& operator=(Entity&&) = default;
    virtual ~Entity() = default;

    bool operator==(const Entity&) const;
    bool operator!=(const Entity&) const;
    bool operator>(const Entity&) const;
    bool operator<(const Entity&) const;

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
    ComponentHandle<C> assign(Args&& ... args);
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

    template<typename C>
    C* rawPointer() const;
    /**
     * @brief Check if this entity has an assigned component of type C.
     * @return True, if component of type C has been assigned, false otherwise.
     */
    template<typename C>
    bool has() const;

protected:
    // friend of EntityManager 
    EntityManager*  manager_{ nullptr };
    Id              id_{ Invalid };
};

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

// this should be a run-time adjustable
// to do so, EntityManager needs to use the Cheshire Cat pattern
const uint32_t MaxComponents = 16u;
const uint32_t MaskElements = MaxComponents + 1u;

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
    EntityManager(EventManager&, uint32_t arenaSize = 64);
    ~EntityManager();

    EntityManager(const EntityManager&) = delete;
    EntityManager(EntityManager&&) = delete;
    EntityManager& operator=(const EntityManager&) = delete;
    EntityManager& operator=(EntityManager&&) = delete;
    /**
     * @brief Create a new entity.
     * @return A handle to the new Entity.
     * Calling this may result in an occasional memory allocation for new components.
     */
    Entity create();
    /**
     * @brief Get an valid entity for the given index.
     */
    Entity get(uint32_t index);
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
        Iterator(EntityManager* owner, std::uint32_t index)
            : owner_(owner),
            index_(index),
            componentIndices_()
        {}
        Iterator(EntityManager* owner, std::uint32_t index, std::vector<uint32_t>&& components)
            : owner_(owner),
            index_(index),
            componentIndices_(std::move(components))
        {}
        Iterator() = delete;
        Iterator(const Iterator&) = default;
        Iterator(Iterator&&) = default;
        Iterator& operator=(const Iterator&) = default;
        Iterator& operator=(Iterator&&) = default;
        ~Iterator() = default;

        bool operator==(const Iterator& rhs) { return index_ == rhs.index_; }
        bool operator!=(const Iterator& rhs) { return index_ != rhs.index_; }
        Iterator& operator++() { // prefix operator
            index_++;
            skip();
            return *this;
        }
        Entity operator*() { return Entity(owner_, Id(index_, owner_->entityVersions_[index_])); }
        /**
         * @brief If the current index is invalid, or the components aren't present, skip forward to the next valid entity.
         */
        void skip() {
            const uint32_t stop = owner_->componentMasks_.size();
            while (index_ < stop && skipIndex_(index_)) {
                index_++;
            }
        }

    private:
        inline bool indexContainsComponents_(uint32_t index) const {
            if (componentIndices_.size() == 0u) {
                return true;
            }
            for (uint32_t i : componentIndices_) {
                if (!owner_->componentMasks_[index].test(i)) {
                    return false;
                }
            }
            return true;
        }
        inline bool indexIsValid_(uint32_t index) const {
            if (owner_->componentMasks_[index].test(MaxComponents)) {
                return false;
            }
            return true;
        }
        inline bool skipIndex_(uint32_t index) const {
            if (index >= owner_->componentMasks_.size()) {
                return false;
            }
            return !(indexIsValid_(index) && indexContainsComponents_(index));
        }

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
        View(EntityManager* owner)
            : owner_(owner),
            componentIndices_()
        {}
        View() = delete;
        ~View() = default;
        View(const View&) = default;
        View(View&&) = default;
        View& operator=(const View&) = default;
        View& operator=(View&&) = default;

        template<typename C>
        void view() {
            componentIndices_.push_back(detail::getComponentId<C>());
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
            Iterator it{ owner_, 0u, std::move(componentIndices_) };
            it.skip();
            return it;
        }
        Iterator end() { return Iterator(owner_, owner_->componentMasks_.size()); }

    private:
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

    void destroy_(Id id);
    bool isValid_(Id id) const;
    void accommodateEntity_(uint32_t index);

    // move template code out so that this file is still human-readable
    template<typename C>
    void accommodateComponent_();

    template<typename C, typename... Args>
    ComponentHandle<C> assign_(Id id, Args&&... args);

    template<typename C>
    void remove_(Id id);  // remove a component
    template<typename C>
    C* component_(Id id);    // the component getter
    template<typename C>
    bool hasComponent_(Id id) const;

    const uint32_t                              ArenaSize_{ 64 };
    uint32_t                                    indexCounter_{ 0u };
    std::vector<std::unique_ptr<BaseArena>>     componentPools_{};
    std::vector<std::bitset<MaskElements>>      componentMasks_{};
    std::vector<uint32_t>                       entityVersions_{};
    std::vector<uint32_t>                       freeList_{};
    EventManager&                               eventDispatcher_;
};

/***
*      _____                                   __  __ __             ____
*     / ___/__  __ _  ___  ___  ___  ___ ___  / /_/ // /__ ____  ___/ / /__
*    / /__/ _ \/  ' \/ _ \/ _ \/ _ \/ -_) _ \/ __/ _  / _ `/ _ \/ _  / / -_)
*    \___/\___/_/_/_/ .__/\___/_//_/\__/_//_/\__/_//_/\_,_/_//_/\_,_/_/\__/
*                  /_/
*/
template<typename C>
C* ComponentHandle<C>::operator->() {
    PG_ASSERT(isValid());
    return manager_->component_<C>(id_);
}

template<typename C>
const C* ComponentHandle<C>::operator->() const {
    PG_ASSERT(isValid());
    return manager_->component_<C>(id_);
}

template<typename C>
C& ComponentHandle<C>::operator*() {
    PG_ASSERT(isValid());
    return *manager_->component_<C>(id_);
}

template<typename C>
const C& ComponentHandle<C>::operator*() const {
    PG_ASSERT(isValid());
    return *manager_->component_<C>(id_);
}

template<typename C>
bool ComponentHandle<C>::isValid() const {
    return manager_->isValid_(id_);
}

template<typename C>
bool ComponentHandle<C>::operator==(const ComponentHandle<C>& rhs) const {
    return manager_ == rhs.manager_ && id_ == rhs.id_;
}

template<typename C>
bool ComponentHandle<C>::operator!=(const ComponentHandle<C>& rhs) const {
    return manager_ != rhs.manager_ && id_ != rhs.id_;
}

/***
*       ____     __  _ __
*      / __/__  / /_(_) /___ __
*     / _// _ \/ __/ / __/ // /
*    /___/_//_/\__/_/\__/\_, /
*                       /___/
*/
template<typename C>
ComponentHandle<C> Entity::component() const {
    PG_ASSERT(isValid());
    return ComponentHandle<C>{ manager_, id_ };
}

template<typename C, typename... Args>
ComponentHandle<C> Entity::assign(Args&& ... args) {
    PG_ASSERT(isValid());
    return manager_->assign_<C>(id_, std::forward<Args>(args)...);
}

template<typename C>
void Entity::remove() {
    PG_ASSERT(isValid());
    manager_->remove_<C>(id_);
}

template<typename C>
bool Entity::has() const {
    PG_ASSERT(isValid());
    return manager_->hasComponent_<C>(id_);
}

template<typename C>
C* Entity::rawPointer() const {
    PG_ASSERT(isValid());
    return manager_->component_<C>(id_);
}


/***
*       ____     __  _ __       __  ___
*      / __/__  / /_(_) /___ __/  |/  /__ ____  ___ ____ ____ ____
*     / _// _ \/ __/ / __/ // / /|_/ / _ `/ _ \/ _ `/ _ `/ -_) __/
*    /___/_//_/\__/_/\__/\_, /_/  /_/\_,_/_//_/\_,_/\_, /\__/_/
*                       /___/                      /___/
*/
template<typename C>
EntityManager::View EntityManager::join() {
    View view{ this };
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
    const unsigned family = detail::getComponentId<C>();
    while (family >= componentPools_.size()) {
        componentPools_.emplace_back(new MemoryArena<C>(ArenaSize_));
    }
}

template<typename C, typename... Args>
ComponentHandle<C> EntityManager::assign_(Id id, Args&&... args) {
    PG_ASSERT(isValid_(id));
    const int family = detail::getComponentId<C>();
    accommodateComponent_<C>(); // create a new component pool, if not already done
    new (componentPools_[family]->newCapacity(id.index())) C{ std::forward<Args>(args)... };

    if (componentMasks_[id.index()].test(family)) {
        // if there already is a component, then log it and return the existing component
        LOG_ERROR << "Tried to assign a component on top of an already existing one!";
        return ComponentHandle<C>{ this, id };
    }

    eventDispatcher_.emit<ComponentAssignedEvent<C>>(Entity{ this, id }, ComponentHandle<C>{ this, id });

    componentMasks_[id.index()].set(family);
    return ComponentHandle<C>{ this, id };
}

template<typename C>
void EntityManager::remove_(Id id) {
    PG_ASSERT(isValid_(id));
    eventDispatcher_.emit<ComponentRemovedEvent<C>>(Entity{ this, id }, ComponentHandle<C>{ this, id });
    const int family = detail::getComponentId<C>();
    const uint32_t index = id.index();
    componentPools_[family]->destroy(index);
    componentMasks_[index].reset(family);
}

template<typename C>
C* EntityManager::component_(Id id) {
    PG_ASSERT(isValid_(id));
    const unsigned family = detail::getComponentId<C>();
    return static_cast<C*>(componentPools_[family]->at(id.index()));
}

template<typename C>
bool EntityManager::hasComponent_(Id id) const {
    PG_ASSERT(isValid_(id));
    const unsigned family = detail::getComponentId<C>();
    return componentMasks_[id.index()].test(family);
}

}   // namespace ecs
}   // namespace pg