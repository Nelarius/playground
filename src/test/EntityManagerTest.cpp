#include "ecs/Entity.h"
#include <UnitTest++/UnitTest++.h>

using ce::ecs::EntityManager;
using ce::ecs::Entity;

struct TestStruct {
    int x;
};

SUITE(EntityManagerTest) {
    
    class EntityManagerFixture {
        public:
            EntityManagerFixture() : entities() {}
            ce::ecs::EntityManager entities;
    };
    
    TEST( AfterCreateSizeIsOne ) {
        EntityManager entities{};
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
    
    TEST_FIXTURE( EntityManagerFixture, IteratorIsAtEndAfterEntityWithComponentsCreatedAndDestroyed ) {
        auto entity = entities.create();
        entity.assign<TestStruct>( 5 );
        CHECK( false );
    }
    
    TEST_FIXTURE( EntityManagerFixture, IteratorIsAtEndAfterEntityWithoutComponentsCreatedAndDestroyed ) {
        auto entity = entities.create();
        auto begin = entities.join().begin();
        auto end = entities.join().end();
        CHECK( false );
    }
}
