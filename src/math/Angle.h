
#pragma once

namespace pg {
namespace math {

struct Degree;

struct Radian {
    Radian( float );
    operator float() const;
    operator Degree() const;
    
    float value;
};

struct Degree {
    Degree( float );
    operator float() const;
    operator Radian() const;
    
    float value;
};

}
}
