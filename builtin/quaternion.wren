
foreign class Quat {
    construct new( x, y, z, w ) {}

    foreign norm()
    foreign normSquared()
    foreign conjugate() // returns the result as a new Quaternion
    foreign inverse()   // returns the result as a new quaternion
    foreign axis()      // returns axis as Vec3
    foreign angle()     // returns angle in radians
    foreign multiply( rhs ) // returns the result as a new Quaternion

    foreign v=( rhs )
    foreign w
    foreign w=( rhs )
}

var createQuaternion = Fn.new { | x, y, z, w |
    return Quat.new( x, y, z, w )
}
