#include "ecs/Entity.h"
#include <UnitTest++/UnitTest++.h>

using pg::ecs::EntityManager;
using pg::ecs::Entity;

struct TestStruct {
    int x;
};

SUITE(EntityManagerTest) {
    
    class EntityManagerFixture {
        public:
            EntityManagerFixture() : events(), entities( events ) {}
            pg::ecs::EventManager events;
            pg::ecs::EntityManager entities;
    };
    
    class EntityFixture {
        public:
            EntityFixture() : events(), entities( events ), entity( entities.create() ) {}
            pg::ecs::EventManager events;
            EntityManager entities;
            Entity entity;
    };
    
    TEST( AfterCreateSizeIsOne ) {
        pg::ecs::EventManager events{};
        EntityManager entities{ events };
        Entity entity = entities.create();
        CHECK_EQUAL( 1u, entities.size() );
    }
    
    TEST_FIXTURE( EntityManagerFixture, AfterCreateAndDestroySizeIsZero ) {
        auto entity = entities.create();
        entity.destroy();
        CHECK_EQUAL( 0u, entities.size() );
    }
    
    TEST_FIXTURE( EntityManagerFixture, DestroyingInvalidatedEntityDoesNothing ) {
        auto entity = entities.create();
        entity.invalidate();
        entity.destroy();
        CHECK_EQUAL( 1u, entities.size() );
    }
    
    TEST_FIXTURE( EntityManagerFixture, InvalidatingEntityKeepsSizeSame ) {
        auto entity = entities.create();
        entity.invalidate();
        CHECK_EQUAL( 1u, entities.size() );
    }
    
    TEST_FIXTURE( EntityManagerFixture, AfterCreatingTwiceSizeIsTwo ) {
        entities.create();
        entities.create();
        CHECK_EQUAL( 2u, entities.size() );
    }
    
    TEST_FIXTURE( EntityManagerFixture, AfterCreatingTwiceAndDestroyingOneSizeIsOne ) {
        entities.create();
        entities.create().destroy();
        CHECK_EQUAL( 1u, entities.size() );
    }
    
    TEST_FIXTURE( EntityManagerFixture, AfterCreatingEntityIsValid ) {
        auto entity = entities.create();
        CHECK( entity.isValid() );
    }
    
    TEST_FIXTURE( EntityManagerFixture, AfterInvalidatingEntityIsNotValid ) {
        auto entity = entities.create();
        entity.invalidate();
        CHECK( !entity.isValid() );
    }
    
    TEST_FIXTURE( EntityManagerFixture, AfterDestroyingEntityIsNotValid ) {
        auto entity = entities.create();
        entity.destroy();
        CHECK( !entity.isValid() );
    }
    
    TEST_FIXTURE( EntityManagerFixture, AfterCreatingOneIteratorBehavesAsExpected ) {
        auto entity = entities.create();
        auto begin = entities.join().begin();
        auto end = entities.join().end();
        CHECK( begin != end );
        ++begin;
        CHECK( begin == end );
    }
    
    TEST_FIXTURE( EntityManagerFixture, IteratorWithComponentToEntityWithoutComponentBehavesAsExpected ) {
        auto entity = entities.create();
        auto begin = entities.join<TestStruct>().begin();
        auto end = entities.join<TestStruct>().end();
        CHECK( begin == end );
    }
    
    TEST_FIXTURE( EntityManagerFixture, IteratorWithComponentToEntityWithComponentBehavesAsExpected ) {
        auto entity = entities.create();
        entity.assign<TestStruct>();
        auto begin = entities.join<TestStruct>().begin();
        auto end = entities.join<TestStruct>().end();
        CHECK( begin != end );
    }
    
    TEST_FIXTURE( EntityManagerFixture, ComponentHasCorrectValueAfterGettingPointerFromComponent ) {
        Entity entity = entities.create();
        entity.assign<TestStruct>( 5 );
        CHECK_EQUAL( 5, entity.component<TestStruct>()->x );
    }
    
    TEST_FIXTURE( EntityManagerFixture, ComponentHasCorrectValueImmediatelyAfterAssignment ) {
        auto entity = entities.create();
        pg::ecs::ComponentHandle<TestStruct> component = entity.assign<TestStruct>( 5 );
        CHECK_EQUAL( 5, component->x );
    }
    
    TEST_FIXTURE( EntityManagerFixture, IteratorIsAtEndAfterEntityWithComponentsCreatedAndDestroyed ) {
        auto entity = entities.create();
        entity.assign<TestStruct>( 5 );
        entity.destroy();
        auto begin = entities.join<TestStruct>().begin();
        auto end = entities.join<TestStruct>().end();
        CHECK( begin == end );
    }
    
    TEST_FIXTURE( EntityManagerFixture, IteratorIsAtEndAfterEntityCreatedAndDestroyed ) {
        auto entity = entities.create();
        entity.destroy();
        auto begin = entities.join().begin();
        auto end = entities.join().end();
        CHECK( begin == end );
    }
    
    TEST_FIXTURE( EntityFixture, EntityHasReturnsTrueWhenComponentAssigned ) {
        entity.assign<TestStruct>();
        CHECK( entity.has<TestStruct>() );
    }
    
    TEST_FIXTURE( EntityFixture, EntityHasReturnsFalseWhenComponentNotAssigned ) {
        CHECK_EQUAL( false, entity.has<TestStruct>() );
    }
    
    TEST_FIXTURE( EntityFixture, EntityHasReturnsFalseAfterComponentAssignedAndRemoved ) {
        entity.assign<TestStruct>();
        entity.remove<TestStruct>();
        CHECK_EQUAL( false, entity.has<TestStruct>() );
    }
}
