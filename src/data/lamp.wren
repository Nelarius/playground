import "builtin/math" for Math
import "builtin/component" for Transform
import "builtin/vector" for Vec3
import "builtin/quaternion" for Quat

var activate = Fn.new {
    //
}

var deactivate = Fn.new {
    //
}

var t = 0.0
var r = 6.0
var pos = Vec3.new( 0.5, 0.5, 0.5 )
var rot = Quat.new( 0.0, 0.0, 0.0, 1.0 )
var scale = Vec3.new( 0.001, 0.001, 0.001 )

var update = Fn.new { | dt |
    t = t + dt
    pos.x = r * Math.cos(t)
    pos.z = r * Math.sin(t)
    entity.transform = Transform.new( pos, rot, scale )
}
