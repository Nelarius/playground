#pragma once

#include "utils/Log.h"
#include <stdexcept>
#include <string>

namespace pg {

/**
 * @class PlaygroundException
 * @author Muszynski Johann M
 * @date 24/08/15
 * @file Exception.h
 * @brief A logged runtime exception class.
 */
class PlaygroundException : public std::runtime_error {
public:
    explicit PlaygroundException(const std::string& what)
        : std::runtime_error(what) {
        LOG_ERROR << "PlaygroundException was thrown: " << what;
    }
    explicit PlaygroundException(const char* what)
        : std::runtime_error(what) {
        LOG_ERROR << "PlaygroundException was thrown: " << what;
    }
};

}

