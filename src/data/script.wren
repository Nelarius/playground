import "builtin/vector" for Vec3

var v1 = Vec3.new( 1.0, 1.0, 1.0 )
var v2 = Vec3.new( 1.0, 1.0, 1.0 )

var activate = Fn.new{
    System.print( v1.norm() )
    var v3 = v1.cross( v2 )
}

var deactivate = Fn.new{
    System.print( "calling deactivate" )
}

var update = Fn.new {
    //System.print( "in ur script, updating ur entity" )
}
