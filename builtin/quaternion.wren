
foreign class Quaternion {
    construct new( x, y, z, w )
    
    foreign norm()
    foreign normSquared()
}

var createQuaternion = Fn.new { | x, y, z, w |
    return Quaternion.new( x, y, z, w )
}
