#include "utils/Container.h"
#include <UnitTest++/UnitTest++.h>

using pg::Container;

SUITE( ContainerTest ) {

    struct ContainerFixture {
        ContainerFixture() {
            c.emplace(2);
        }
        Container<int, 1> c{};
    };

    TEST( AfterInitializationSizeIsZero ) {
        Container< int, 1 > c{};
        CHECK_EQUAL( 0u, c.size() );
    }

    TEST_FIXTURE( ContainerFixture, AfterAddingOneSizeIsOne ) {
        CHECK_EQUAL( 1u, c.size() );
    }

    TEST_FIXTURE( ContainerFixture, AddedElementHasCorrectValue ) {
        CHECK_EQUAL( 2, c[0] );
    }

    TEST_FIXTURE( ContainerFixture, AfterClearingSizeIsZero ) {
        c.clear();
        CHECK_EQUAL( 0u, c.size() );
    }

}