
foreign class Vec2 {
    construct new( x, y ) {}

    foreign x
    foreign x=( rhs )
    foreign y
    foreign y=( rhs )
    foreign i
    foreign i=( rhs )
    foreign j
    foreign j=( rhs )

    foreign norm()
    foreign normSquared()
    foreign normalize() // normalizes the vector in place
    foreign dot( rhs )
    foreign hadamard( rhs )
    foreign plus( rhs )
    foreign minus( rhs )
    foreign scale( s )
}

foreign class Vec3 {
    construct new( x, y, z ) {}

    foreign x
    foreign x=( rhs )
    foreign y
    foreign y=( rhs )
    foreign z
    foreign z=( rhs )
    foreign r
    foreign r=( rhs )
    foreign g
    foreign g=( rhs )
    foreign b
    foreign b=( rhs )

    foreign norm()
    foreign normSquared()
    foreign normalize()
    foreign dot( rhs )
    foreign cross( rhs )    // returns the result
    foreign hadamard( rhs ) // returns the result
    foreign plus( rhs )     // returns the result
    foreign minus( rhs )    // returns the result
    foreign scale( s )      // returns the result
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
    foreign r
    foreign r=( rhs )
    foreign g
    foreign g=( rhs )
    foreign b
    foreign b=( rhs )
    foreign a
    foreign a=( rhs )

    foreign norm()
    foreign normSquared()
    foreign normalize()
    foreign dot( rhs )
    foreign hadamard( rhs )
    foreign plus( rhs )
    foreign minus( rhs )
    foreign scale( s )
}
