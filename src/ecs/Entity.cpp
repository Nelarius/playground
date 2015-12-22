#include "ecs/Entity.h"
#include "utils/Assert.h"
#include "utils/Log.h"

namespace pg {
namespace ecs {

//    ______       _   _ _         
//   |  ____|     | | (_) |        
//   | |__   _ __ | |_ _| |_ _   _ 
//   |  __| | '_ \| __| | __| | | |
//   | |____| | | | |_| | |_| |_| |
//   |______|_| |_|\__|_|\__|\__, |
//                            __/ |
//                           |___/ 
Entity::Entity( EntityManager* manager, Id id )
:   manager_( manager ),
    id_( id )
    {}

void Entity::invalidate() {
    id_ = Invalid;
    manager_ = nullptr;
}

bool Entity::isValid() const {
    if ( manager_ ) {
        return manager_->isValid_( id_ );
    }
    // if there is no manager, then this entity
    // is invalid (the static member).
    return false;
}

void Entity::destroy() {
    // to prevent destroying an already invaldiated Entity
    if ( manager_ ) {
        manager_->destroy_( id_ );
        invalidate();
    }
}

Id Entity::id() const {
    return id_;
}

bool Entity::operator==( const Entity& rhs ) const {
    return manager_ == rhs.manager_ && id_ == rhs.id_;
}

bool Entity::operator!=( const Entity& rhs ) const {
    return !( *this == rhs );
}

bool Entity::operator>( const Entity& rhs ) const {
    return id_ > rhs.id_;
}

bool Entity::operator<( const Entity& rhs ) const {
    return id_ < rhs.id_;
}

//    ______       _   _ _         __  __                                   
//   |  ____|     | | (_) |       |  \/  |                                  
//   | |__   _ __ | |_ _| |_ _   _| \  / | __ _ _ __   __ _  __ _  ___ _ __ 
//   |  __| | '_ \| __| | __| | | | |\/| |/ _` | '_ \ / _` |/ _` |/ _ \ '__|
//   | |____| | | | |_| | |_| |_| | |  | | (_| | | | | (_| | (_| |  __/ |   
//   |______|_| |_|\__|_|\__|\__, |_|  |_|\__,_|_| |_|\__,_|\__, |\___|_|   
//                            __/ |                          __/ |          
//                           |___/                          |___/           
EntityManager::EntityManager( EventManager& eventManager, uint32_t arenaSize )
:   ArenaSize_( arenaSize ),
    indexCounter_( 0u ),
    componentPools_(),
    componentMasks_(),
    entityVersions_(),
    freeList_(),
    mailMan_( eventManager )
    {}

EntityManager::~EntityManager() {
    for ( auto entity: join() ) {
        entity.destroy();
    }
}

Entity EntityManager::create() {
    uint32_t index, version;
    version = 0u;
    if ( freeList_.empty() ) {
        index = indexCounter_++;    // should start from zero, hence postfix
        accommodateEntity_( index );    // a new entry to componentMasks_, entityVersions_ made here
    } else {
        index = freeList_.back();
        freeList_.pop_back();
        version = entityVersions_[index];    // versions are incremented in destroy
        componentMasks_[index].set( MaxComponents, false ); // resets the last bit signalling that the entity is now valid
    }
    
    Entity entity( this, Id( index, version ) );
    mailMan_.emit<EntityCreatedEvent>( entity );
    return entity;
}

Entity EntityManager::get( uint32_t index ) {
    // indexCounter_ always points to the position of the next entity to be created
    if ( index < indexCounter_ ) {
        return Entity( this, Id( index, entityVersions_[index]) );
    }
    return Entity();
}

EntityManager::View EntityManager::join() {
    return View{ this };
}

void EntityManager::accommodateEntity_( uint32_t index ) {
    for ( auto& pool : componentPools_ ) {
        pool->reserve( index + 1 );
    }
    componentMasks_.emplace_back(); // default bitset constructor initializes to all zeros
    entityVersions_.push_back( 0u );
}

void EntityManager::destroy_( Id id ) {
    ASSERT( isValid_( id ) ); 
    mailMan_.emit<EntityDestroyedEvent>( Entity( this, id ) );
    uint32_t index = id.index();
    auto& mask = componentMasks_[index];
    // we need to call the destructor of each component
    for ( uint32_t i = 0; i < componentPools_.size(); i++ ) {
        if ( mask.test(i) ) {
            componentPools_[i]->destroy( index );
        }
    }
    mask.reset();
    mask.set( MaxComponents );   // this element tells the iterator to skip this entity
    entityVersions_[index]++;
    freeList_.push_back( index );
}

bool EntityManager::isValid_( Id id ) const {
    if ( id.version() < entityVersions_[id.index()] ) {
        return false;
    }
    return true;
}

void EntityManager::reset() {
    componentPools_.clear();
    componentMasks_.clear();
    entityVersions_.clear();
    freeList_.clear();
}

std::size_t EntityManager::size() const {
    return entityVersions_.size() - freeList_.size();
}

}
}
