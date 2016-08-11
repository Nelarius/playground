#pragma once

#include "utils/Assert.h"
#include "mm_json.h"

#include <cstring>
#include <cstdlib>
#include <vector>

namespace pg {

struct Context;

// populate the entity manager with entities
void readScene(Context& context, const char* file);

// write the entity manager state into a file
void writeScene(Context& context, const char* file);

}
