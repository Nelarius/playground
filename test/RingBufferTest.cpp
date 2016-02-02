#include "utils/RingBuffer.h"
#include "utils/Log.h"
#include <UnitTest++/UnitTest++.h>

using pg::RingBuffer;

SUITE( RingBufferTest ) {

    struct RingBufferWithElements {
        RingBuffer< int > buf;
        RingBufferWithElements()
        :   buf{5} {
            buf.pushBack(1);
            buf.pushBack(2);
        }

    };

    TEST( AfterInitializationSizeIsZero ) {
        RingBuffer<int> buf{ 5 };
        CHECK_EQUAL( 0u, buf.size() );
    }

    TEST( AfterPushBackSizeAndElementAreCorrect ) {
        RingBuffer<int> buf{ 5 };
        buf.pushBack( 1 );
        CHECK_EQUAL( 1u, buf.size() );
        CHECK_EQUAL( 1, buf.at( 0 ) );
    }

    TEST( CorrectBehaviorAfterCapacityReachedWithPushBack ) {
        RingBuffer<int> buf{ 2 };
        buf.pushBack(1);
        buf.pushBack(2);
        buf.pushBack(3);
        CHECK_EQUAL( 2u, buf.size() );
        CHECK_EQUAL( 2, buf.at(0) );
        CHECK_EQUAL( 3, buf.at(1) );
        CHECK_EQUAL( 2, buf.front() );
        CHECK_EQUAL( 3, buf.back() );
    }

    TEST( CorrectBehaviorAfterCapacityReachedWithEmplace ) {
        RingBuffer<int> buf{ 2 };
        buf.emplaceBack(1);
        buf.emplaceBack(2);
        buf.emplaceBack(3);
        CHECK_EQUAL( 2u, buf.size() );
        CHECK_EQUAL( 2, buf.at(0) );
        CHECK_EQUAL( 3, buf.at(1) );
        CHECK_EQUAL( 2, buf.front() );
        CHECK_EQUAL( 3, buf.back() );
    }

    TEST_FIXTURE( RingBufferWithElements, SizeAndElementsCorrectAfterPopBack ) {
        buf.popBack();
        CHECK_EQUAL( 1u, buf.size() );
        CHECK_EQUAL( 1, buf.front() );
        CHECK_EQUAL( 1, buf.back() );
    }
    
    TEST_FIXTURE( RingBufferWithElements, SizeAndElementsCorrectAfterPopFront ) {
        buf.popFront();
        CHECK_EQUAL( 1u, buf.size() );
        CHECK_EQUAL( 2, buf.front() );
        CHECK_EQUAL( 2, buf.back() );
    }

    TEST(ForwardIteratorWorksAsExpected) {
        RingBuffer<int> buf{5};
        buf.pushBack(1);
        buf.pushBack(2);
        buf.pushBack(3);
        auto it = buf.begin();
        CHECK_EQUAL(1, *(it++));
        CHECK_EQUAL(2, *(it++));
        CHECK_EQUAL(3, *(it++));
        CHECK_EQUAL(buf.end(), it);
    }

}
