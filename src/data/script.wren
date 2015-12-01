import "builtin/vector" for Vec3
import "builtin/quaternion" for Quat
import "builtin/entity" for Entity
import "builtin/math" for Math

var v1 = Vec3.new( -1.0, 1.0, 1.0 )
var v2 = Vec3.new( 0.0, -2.3, 1.0 )
var q1 = Quat.new( 0.0, 0.0, 0.0, 1.0 )

var activate = Fn.new{
    System.print( v1.norm() )
    var v3 = (v2.cross( v1 ))
    v1.x = 5.0
    v1.y = 5.0
    v1.z = 5.0
    System.print("v1.cross(v2): (%(v3.x), %(v3.y), %(v3.z))")
    System.print("v1.dot(v2): %(v1.dot(v2))")
}

var deactivate = Fn.new{
    System.print( "calling deactivate" )
}

var update = Fn.new {
    //System.print( "in ur script, updating ur entity" )
}
