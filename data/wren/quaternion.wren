
foreign class Quat {
    construct new( x, y, z, w ) {}

    foreign norm()
    foreign normSquared()
    foreign conjugate() // returns the result as a new Quaternion
    foreign inverse()   // returns the result as a new quaternion
    foreign axis()      // returns axis as Vec3
    foreign angle()     // returns angle in radians
    foreign multiply( rhs ) // returns the result as a new Quaternion

    foreign xaxis()     // returns the normalized axis as a Vec3
    foreign yaxis()
    foreign zaxis()

    foreign v
    foreign v=( rhs )
    foreign w
    foreign w=( rhs )
}
