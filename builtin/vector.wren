
foreign class Vec2 {
    construct new( x, y ) {}

    foreign x
    foreign x=( rhs )
    foreign y
    foreign y=( rhs )

    foreign norm()
    foreign normSquared()
    foreign dot( rhs )
    foreign hadamard( rhs )
    foreign plus( rhs )
    foreign minus( rhs )
    //foreign scale( s )
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
    //foreign scale( s )
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
    foreign hadamard( rhs )
    foreign plus( rhs )
    foreign minus( rhs )
    //foreign scale( s )
}

/*
 * These are creator objects for the C++ API to use in order to create foreign objects
 * and return them back to Wren
 */

var createVec4 = Fn.new {
    return Vec4.new(0.0, 0.0, 0.0, 0.0)
}

var createVec3 = Fn.new {
    System.print("Creating a new Vec3!")
    return Vec3.new(0.0, 0.0, 0.0)
}

var createVec2 = Fn.new {
    return Vec2.new(0.0, 0.0)
}

var greet = Fn.new {
    System.print("Hello, sir!")
}

