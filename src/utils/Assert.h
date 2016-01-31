#ifndef PG_ASSERT_H_INCLUDED
#define PG_ASSERT_H_INCLUDED

#include <cstdlib>
#include <iostream>

//defined as a do-while so that a semicolon can be used
#ifdef DEBUG
    #define PG_ASSERT( condition ) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion '" #condition "' failed in " << __FILE__ \
                      << " line " << __LINE__ << ".\n"; \
            std::exit(EXIT_FAILURE); \
        } \
    } while (false) \

#else
    #define PG_ASSERT( condition ) do {} while (false)
#endif


#endif // PG_ASSERT_H_INCLUDED
