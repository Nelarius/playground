#ifndef USE_H_INCLUDED
#define USE_H_INCLUDED

#include "opengl/Program.h"
#include "opengl/VertexAttributes.h"

namespace pg {
namespace opengl {

class UseProgram {
public:
    UseProgram(Program& program)
        : program_{ program } {
        program_.use();
    }

    ~UseProgram() {
        program_.stopUsing();
    }
private:
    Program&    program_;
};

class UseArray {
public:
    UseArray(VertexAttributes& array)
        :array_{ array } {
        array.bind();
    }

    ~UseArray() {
        array_.unbind();
    }
private:
    VertexAttributes&  array_;
};

}
}

#endif // USE_H_INCLUDED
