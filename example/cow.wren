import "builtin/vector" for Vec3
import "builtin/quaternion" for Quat
import "builtin/math" for Math
import "builtin/component" for Transform

var pos = Vec3.new( 0.0, 0.0, 0.0 )
var scale = Vec3.new( 1.0, 1.0, 1.0 )
var rot = Quat.new( 0.0, 0.0, 0.0, 1.0 )

var activate = Fn.new{
    System.print( "Greetings from script-land." )
}

var deactivate = Fn.new{
    System.print( "calling deactivate" )
}

var pi = 3.1415927
var t = 0.0

var update = Fn.new { | dt |
    t = t + dt
    rot.v = Vec3.new( 0.0, Math.sin(-0.2*t), 0.0 )
    rot.w = Math.cos(-0.2*t)
    entity.transform = Transform.new( pos, rot, scale )
}
