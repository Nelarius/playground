#include "utils/StringId.h"
#include <UnitTest++/UnitTest++.h>

namespace pg {

SUITE(StringTest) {
    struct StringDB {
        StringId::Database database{};

        StringDB() {
            StringId::setDatabase(&database);
        }
    };

    TEST_FIXTURE(StringDB, SameStringValuesAreEqual) {
        StringId asdf1{ "asdf" };
        StringId asdf2{ "asdf" };
        CHECK(asdf1 == asdf2);
    }

    TEST_FIXTURE(StringDB, DifferentStringValuesAreNotEqual) {
        StringId str1{ "asdf" };
        StringId str2{ "qwer" };
        CHECK(str1 != str2);
    }

    TEST_FIXTURE(StringDB, StringReturnsOriginalStringValue) {
        StringId str{ "asdf" };
        const char* val = str.cString();
        CHECK_EQUAL(0, strcmp("asdf", val));
    }
}

}
