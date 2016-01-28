#ifndef USE_H_INCLUDED
#define USE_H_INCLUDED

#include "opengl/Program.h"
#include "opengl/VertexArrayObject.h"

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
    UseArray(VertexArrayObject& array)
        :array_{ array } {
        array.bind();
    }

    ~UseArray() {
        array_.unbind();
    }
private:
    VertexArrayObject&  array_;
};

}
}

#endif // USE_H_INCLUDED
