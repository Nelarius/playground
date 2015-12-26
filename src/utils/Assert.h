#ifndef ASSERT_H_INCLUDED
#define ASSERT_H_INCLUDED

#include <cstdlib>
#include <iostream>

//defined as a do-while so that a semicolon can be used
#ifdef DEBUG
    #define ASSERT( condition ) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion '" #condition "' failed in " << __FILE__ \
                      << " line " << __LINE__ << ".\n"; \
            std::exit(EXIT_FAILURE); \
        } \
    } while (false) \

#else
    #define ASSERT( condition ) do {} while (false)
#endif


#endif // ASSERT_H_INCLUDED
