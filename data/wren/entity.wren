
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
    foreign renderable
    foreign renderable=( rhs )

    foreign assignTransform( t )
    foreign assignRenderable( r )
}

class EntityManager {
    foreign static create() // returns the new entity
    foreign static entityCount
}
