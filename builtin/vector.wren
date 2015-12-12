
foreign class Vec2 {
    construct new( x, y ) {}

    foreign x
    foreign x=( rhs )
    foreign y
    foreign y=( rhs )

    foreign norm()
    foreign normSquared()
    foreign dot( rhs )
}

foreign class Vec3 {
    construct new( x, y, z ) {}

    foreign x
    foreign x=( rhs )
    foreign y
    foreign y=( rhs )
    foreign z
    foreign z=( rhs )
    foreign norm()
    foreign normSquared()
    foreign dot( rhs )
    foreign cross( rhs )    // returns the result
    foreign hadamard( rhs ) // returns the result
    foreign plus( rhs )     // returns the result
    foreign minus( rhs )    // returns the result
}

foreign class Vec4 {
    construct new( x, y, z, w ) {}

    foreign x
    foreign x=( rhs )
    foreign y
    foreign y=( rhs )
    foreign z
    foreign z=( rhs )
    foreign w
    foreign w=( rhs )

    foreign norm()
    foreign normSquared()
    foreign dot( rhs )
}

/*
 * These are creator objects for the C++ API to use in order to create foreign objects
 * and return them back to Wren
 */

var createVec4 = Fn.new { | x, y, z, w |
    return Vec4.new( x, y, z, w )
}

var createVector3 = Fn.new { | x, y, z |
    return Vec3.new( x, y, z )
}

var createVec2 = Fn.new { | x, y |
    return Vec2.new( x, y )
}
