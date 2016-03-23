
class Math {
    // trigonometric functions
    foreign static cos( x )
    foreign static sin( x )
    foreign static tan( x )
    foreign static acos( x )
    foreign static asin( x )
    foreign static atan( x )
    foreign static atan2( x )

    // exponential and logarithmic functions
    foreign static exp( x ) 
    foreign static log( x )

    // power functions
    foreign static pow( x )
    foreign static sqrt( x )
    foreign static cbrt( x )

    // rounding and remainder functions
    foreign static ceil( x )
    foreign static floor( x )
    foreign static round( x )

    // other functions
    foreign static abs( x )

    // x and y are normalized mouse coordinates
    // they're provided by the Mouse class
    foreign static generateCameraRay(x, y) // returns the camera ray
}

/*
 * A ray (R) is described by its origin (O), direction (D), and scaling parameter (t):
 * R = O + t * D.
 */
foreign class Ray {
   construct new(origin, direction) {}

   foreign origin      // Vec3
   foreign origin=(rhs)
   foreign direction   // normalized Vec3
   foreign direction=(rhs)
   foreign scalingParameter   // a number
   foreign scalingParameter=(rhs)

   foreign planeIntersection(plane)    // retuns the intersection point as Vec3
}

/*
 * A plane is defined by three unique points. The plane's orientation is defined by the three points
 * according to the right hand rule.
 *
 * This means that the normal will point in the direction of the following cross product:
 * e1 x e2, where e1 = (p1 - p0) and e2 = (p2 - p0).
 */
foreign class Plane {
   construct new(p0, p1, p2) {}

   foreign point() // returns a point on the plane as Vec3
   foreign normal() // returns the plane normal as Vec3
}
