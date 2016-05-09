import "pg/math" for Math


foreign class Quat {
    construct new( x, y, z, w ) {}

    foreign norm()
    foreign normSquared()
    foreign normalize() // normalizes the quaternion in place
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

    // Create a quaternion representing the rotation between two axes
    // The axes must be normalized
    static fromAxes(axis1, axis2) {
        var axis = axis1.cross(axis2)
        axis.normalize()
        var angle = 0.5 * Math.acos(axis1.dot(axis2))
        var mul = Math.sin(angle)
        return Quat.new(mul*axis.x, mul*axis.y, mul*axis.z, Math.cos(angle))
    }

    // Create a quaternion representing the rotation of angle about the axis axis
    static fromAxisAngle(axis, angle) {
        var s = Math.sin(0.5 * angle)
        var u = axis.scale(s)
        return Quat.new(u.x, u.y, u.z, Math.cos(0.5 * angle))
    }
}
