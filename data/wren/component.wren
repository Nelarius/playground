
foreign class Transform {
    construct new( position, rotation, scale ) {}
    foreign position
    foreign position=( p )
    foreign rotation
    foreign rotation=( r )
    foreign scale
    foreign scale=( s )
}

foreign class Renderable {
    // construct with two resources: the model resource, and the shader resource
    construct new( model, shader ) {}
    foreign shininess
    foreign shininess=(value)
    foreign baseColor
    foreign baseColor=(color)
    foreign ambientColor
    foreign ambientColor=(color)
    foreign specularColor
    foreign specularColor=(color)
}
