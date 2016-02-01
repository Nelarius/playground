#ifndef PG_ASSERT_H_INCLUDED
#define PG_ASSERT_H_INCLUDED

// Define assertion handler.
#ifndef PG_ASSERT
#include <cassert>
#define PG_ASSERT(_EXPR)    assert(_EXPR)
#endif

#endif // PG_ASSERT_H_INCLUDED
