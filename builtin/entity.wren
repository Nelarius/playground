
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
    static create() {   // returns a new Entity
        var entity = Entity.new()
        var id = createEntity_()
        entity.set(id)
        return entity
    }
    foreign static createEntity_()   // creates a new Entity and returns it's id
    foreign static entityCount
}
