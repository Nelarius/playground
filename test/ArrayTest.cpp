#include "utils/Array.h"
#include <UnitTest++/UnitTest++.h>

using pg::DynamicArray;
using pg::StaticArray;

SUITE(ArrayTest) {

    struct DynamicArrayWithElements {
        DynamicArray<int> array{};

        DynamicArrayWithElements()
            : array{10} {
            for (int i = 0; i < 10; i++) {
                array.pushBack(i);
            }
        }
    };

    struct StaticArrayWithElements {
        StaticArray<int, 3> array{};

        StaticArrayWithElements()
            : array{} {
            for (int i = 0; i < 3; i++) {
                array.fill(i);
            }
        }
    };

    TEST(AfterInitializationSizeIsZero) {
        DynamicArray<int> array{};
        CHECK_EQUAL(0u, array.size());
    }

    TEST(ExpectedResultAfterPushBack) {
        DynamicArray<int> array{};
        array.pushBack(3);
        CHECK_EQUAL(1u, array.size());
        CHECK_EQUAL(3, array[0]);
        CHECK_EQUAL(3, array.front());
    }

    TEST(ExpectedResultAfterEmplaceBack) {
        DynamicArray<int> array{};
        array.emplaceBack(5);
        CHECK_EQUAL(1u, array.size());
        CHECK_EQUAL(5, array[0]);
    }

    TEST(SwapAndRemoveIndexHasExpectedResultWithTwoElements) {
        DynamicArray<int> array{};
        array.pushBack(0);
        array.pushBack(2);
        array.swapAndRemove(0u);
        CHECK_EQUAL(2, array[0]);
    }

    TEST_FIXTURE(DynamicArrayWithElements, SwapAndRemoveIteratorHasExpectedResult) {
        auto it = array.begin();
        array.swapAndRemove(it);
        CHECK_EQUAL(9, array.size());
        CHECK_EQUAL(9, array[0]);
        CHECK_EQUAL(9, array.front());
    }

    TEST_FIXTURE(DynamicArrayWithElements, SwapAndRemoveReverseIteratorHasExpectedResult) {
        auto it = array.rbegin();
        ++it;
        array.swapAndRemove(it);
        CHECK_EQUAL(9, array.size());
        CHECK_EQUAL(9, array[8]);
        CHECK_EQUAL(9, array.back());
    }

    TEST_FIXTURE(DynamicArrayWithElements, ExpectedResultAfterClear) {
        array.clear();
        CHECK_EQUAL(0u, array.size());
        array.pushBack(9);
        CHECK_EQUAL(9, array[0]);
        CHECK_EQUAL(1, array.size());
    }

    TEST_FIXTURE(DynamicArrayWithElements, ExpectedResultAfterPopBack) {
        array.popBack();
        CHECK_EQUAL(9, array.size());
        CHECK_EQUAL(8, array.back());
        CHECK_EQUAL(0, array.front());
    }

    TEST(ForwardIteratorWorksAsExpected) {
        DynamicArray<int> array{};
        array.pushBack(1);
        array.pushBack(2);
        array.pushBack(3);
        auto it = array.begin();
        CHECK_EQUAL(1, *(it++));
        CHECK_EQUAL(2, *(it++));
        CHECK_EQUAL(3, *(it++));
        CHECK_EQUAL(array.end(), it);
    }

    TEST(ReverseIteratorWorksAsExpected) {
        DynamicArray<int> array{};
        array.pushBack(1);
        array.pushBack(2);
        array.pushBack(3);
        auto it = array.rbegin();
        CHECK_EQUAL(3, *(it++));
        CHECK_EQUAL(2, *(it++));
        CHECK_EQUAL(1, *(it++));
        CHECK_EQUAL(array.rend(), it);
    }

    TEST(AfterInitializationStaticArraySizeIsZero) {
        StaticArray<int, 5> array{};
        CHECK_EQUAL(0, array.size());
    }

    TEST(FillStaticArrayWorksAsExpected) {
        StaticArray<int, 5> array{};
        array.fill(1);
        CHECK_EQUAL(1u, array.size());
        CHECK_EQUAL(1, array[0]);
        array.fill(2);
        CHECK_EQUAL(2u, array.size());
        CHECK_EQUAL(2, array[1]);
    }

    TEST_FIXTURE(StaticArrayWithElements, StaticArrayForwardIteratorWorksAsExpected) {
        auto it = array.begin();
        CHECK_EQUAL(0, *(it++));
        CHECK_EQUAL(1, *(it++));
        CHECK_EQUAL(2, *(it++));
        CHECK_EQUAL(array.end(), it);
    }

    TEST_FIXTURE(StaticArrayWithElements, StaticArrayReverseiteratorWorksAsExpected) {
        auto it = array.rbegin();
        CHECK_EQUAL(2, *(it++));
        CHECK_EQUAL(1, *(it++));
        CHECK_EQUAL(0, *(it++));
        CHECK_EQUAL(array.rend(), it);
    }

}
