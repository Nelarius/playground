
#include "ecs/System.h"
#include "ecs/Entity.h"
#include "ecs/Event.h"

namespace pg {
namespace ecs {

uint32_t BaseSystem::familyCounter_ = 0u;

SystemManager::SystemManager(EventManager& events, EntityManager& entities)
    : events_(events),
    entities_(entities)
{}

}
}
