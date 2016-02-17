
foreign class Transform {
    construct new( position, rotation, scale ) {}
}

foreign class Renderable {
    // construct with two resources: the model resource, and the shader resource
    construct new( model, shader ) {}
}
