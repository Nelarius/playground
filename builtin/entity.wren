
foreign class Entity {
    construct new() {}
    // this method is meant to be called externally
    foreign set( id )
    foreign transform=( t )

    foreign isValid()
    foreign index
    foreign version
    foreign hasTransform()
    foreign hasRenderable()
    foreign hasCamera()
    foreign hasPointLight()
}

foreign class Transform {
    construct new( position, rotation, scale ) {}
}
