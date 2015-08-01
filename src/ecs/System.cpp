
#include "ecs/System.h"
#include "ecs/Entity.h"
#include "ecs/Event.h"

uint32_t ce::ecs::BaseSystem::familyCounter_ = 0u;

using ce::ecs::SystemManager;

SystemManager::SystemManager( ce::ecs::EventManager& events, ce::ecs::EntityManager& entities )
:   events_( events ),
    entities_( entities )
    {}
