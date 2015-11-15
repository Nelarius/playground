
foreign class Vec3 {
    construct new( x, y, z ) {}

    foreign norm()
    foreign normSquared()
    foreign dot( rhs )
    foreign cross( rhs )    // returns the result
    //foreign hadamard( rhs ) // returns the result
    foreign plus( rhs )     // returns the result
    foreign minus( rhs )    // returns the result
}

/*
 * These are creator objects for the C++ API to use in order to create foreign objects
 * and return them back to Wren
 */

var createVector3 = Fn.new { | x, y, z |
    return Vec3.new( x, y, z )
}
