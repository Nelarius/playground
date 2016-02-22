
#pragma once

#include "ecs/Component.h"
#include "ecs/Event.h"
#include "ecs/Entity.h"
#include "utils/Assert.h"
#include <unordered_map>
#include <type_traits>
#include <memory>
#include <cstddef>  // for std::size_t
#include <cstdint>

namespace pg {
namespace ecs {
namespace detail {

inline uint32_t& systemId() {
    static uint32_t id{ 0u };
    return id;
}

template<typename T>
uint32_t getSystemIdImpl() {
    static uint32_t id = ++systemId();
    return id;
}

}   // detail

template<typename S>
uint32_t getSystemId() {
    return detail::getSystemIdImpl<std::decay<S>>();
}

class EntityManager;
class EventManager;

class System {
public:
    System() = default;
    virtual ~System() = default;

    virtual void configure(EventManager&) {}
    virtual void update(EntityManager&, EventManager&, float dt) {}
};

class SystemManager {
public:
    SystemManager(EventManager& events, EntityManager& entities)
        : events_{ events},
        entities_{ entities } {}
    ~SystemManager() = default;

    template<typename S, typename... Args>
    std::shared_ptr<S> add(Args&&... args);

    template<typename S>
    std::shared_ptr<S> system();

    template<typename S>
    void configure();
    template<typename S>
    void update(float dt);

private:
    EventManager&   events_;
    EntityManager&  entities_;
    std::unordered_map<uint32_t, std::shared_ptr<System>>   systems_;
};

/////////////////////////////////////////////////////////////////////////////
// SystemManager implementation
/////////////////////////////////////////////////////////////////////////////
template<typename S, typename... Args>
std::shared_ptr<S> SystemManager::add(Args&&... args) {
    std::shared_ptr<S> s(new S(std::forward<Args>(args)...));
    systems_.insert(std::make_pair(getSystemId<S>(), s));
    return s;
}

template<typename S>
std::shared_ptr<S> SystemManager::system() {
    auto it = systems_.find(getSystemId<S>());
    PG_ASSERT(it != systems_.end());
    return std::shared_ptr<S>(std::static_pointer_cast<S>(it->second));
}

template<typename S>
void SystemManager::configure() {
    auto it = systems_.find(getSystemId<S>());
    PG_ASSERT(it != systems_.end());
    it->second->configure(events_);
}

template<typename S>
void SystemManager::update(float dt) {
    auto it = systems_.find(getSystemId<S>());
    PG_ASSERT(it != systems_.end());
    it->second->update(entities_, events_, dt);
}

}   // namespace ecs
}   // namespace ce 

