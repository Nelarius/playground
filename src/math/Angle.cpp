#include "math/Angle.h"


namespace pg {
namespace math {

Radian::Radian( float v )
:   value( v )
    {}

Radian::operator float() const {
    return value;
}

Radian::operator Degree() const {
    return Degree( value*180.0f / 3.1415926f );
}
    
Degree::Degree( float v )
:   value( v )
    {}
    
Degree::operator float() const {
    return value;
}
    
Degree::operator Radian() const { 
    return Radian( value*3.1415926f / 180.0f ); 
}

}
}
