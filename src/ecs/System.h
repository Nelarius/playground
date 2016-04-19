
#pragma once

#include "ecs/Component.h"
#include "ecs/Event.h"
#include "ecs/Entity.h"
#include "utils/Assert.h"
#include <vector>
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
    static uint32_t id = systemId()++;
    return id;
}

template<typename S>
uint32_t getSystemId() {
    return detail::getSystemIdImpl<std::decay<S>>();
}

}   // detail


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
        entities_{ entities },
        systems_{} {}
    ~SystemManager() = default;

    template<typename S, typename... Args>
    System* add(Args&&... args);

    template<typename S>
    S& system();

    template<typename S>
    void configure();
    template<typename S>
    void update(float dt);

private:
    EventManager&   events_;
    EntityManager&  entities_;
    std::vector<std::unique_ptr<System>> systems_;
};

/////////////////////////////////////////////////////////////////////////////
// SystemManager implementation
/////////////////////////////////////////////////////////////////////////////
template<typename S, typename... Args>
System* SystemManager::add(Args&&... args) {
    PG_ASSERT(detail::getSystemId<S>() == systems_.size());
    systems_.emplace_back(new S(std::forward<Args>(args)...));
    return systems_[detail::getSystemId<S>()].get();
}

template<typename S>
S& SystemManager::system() {
    PG_ASSERT(detail::getSystemId<S>() < systems_.size());
    return (S&)(*systems_[detail::getSystemId<S>()].get());
}

template<typename S>
void SystemManager::configure() {
    PG_ASSERT(detail::getSystemId<S>() < systems_.size());
    systems_[detail::getSystemId<S>()]->configure(events_);
}

template<typename S>
void SystemManager::update(float dt) {
    PG_ASSERT(detail::getSystemId<S>() < systems_.size());
    systems_[detail::getSystemId<S>()]->update(entities_, events_, dt);
}

}   // namespace ecs
}   // namespace ce 
