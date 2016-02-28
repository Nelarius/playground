
foreign class Entity {
    construct new() {}

    // this method is meant to be called externally
    foreign set_( id )

    foreign index
    foreign version

    foreign isValid()
    foreign destroy()
    foreign invalidate()

    foreign hasTransform()
    foreign hasRenderable()
    foreign hasCamera()
    foreign hasPointLight()
    foreign transform
    foreign transform=( t )

    foreign assignTransform( t )
    foreign assignRenderable( r )
}

var createEntity = Fn.new {
    return Entity.new()
}

class EntityManager {
    foreign static create() // returns the new entity
    foreign static entityCount
}
