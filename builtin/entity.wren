
foreign class Entity {
    construct new() {}
    // this method is meant to be called externally
    foreign set( id )
    foreign transform=( t )

    foreign isValid()
    foreign destroy()
    foreign invalidate()
    foreign index
    foreign version
    foreign hasTransform()
    foreign hasRenderable()
    foreign hasCamera()
    foreign hasPointLight()
}

var createEntity = Fn.new {
    return Entity.new()
}

class EntityManager {
    foreign static create() // returns the new entity
    foreign static entityCount
}
