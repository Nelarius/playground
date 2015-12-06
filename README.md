# The Playground engine

A small, experimental, data-driven, entity-component-based, and Wren-scriptable game engine. I'm using this project for learning how game engines work.

## Compiling

The source code uses GCC pragmas and C++14.

#### Linux

On Linux, once the dependencies have been installed, just run `make` to build the program and run the tests. The result will appear in the `./Build` folder.

#### Windows

Compiling on Windows isn't a great experience at the moment. Your locations of the dependencies should be entered into the `*_COMP` and `*_LINK` fields, at the beginning of the Makefile. Once that has been done, the build is done the same way as on Linux.

## Organization

The C/C++ source code is in `src/`. Some documentation exists within the `src` folder for the contained modules. The engine depends on some data, such as scripting files and shader source. These are located in `builtin/`.

External dependencies are in `extern`.

## How it works

The engine is based around an entity-component system. The components are simply structs containing data. The components live in contiguous arrays. The game engine logic is implemented in systems which iterate over any component arrays that it needs. Entities are merely handles that tie a number of components together.

It's natural to represent each component in JSON, and this is what the scene file essentially is: a list of entities, represented as objects, containing components.

A small look at the currently available components in their JSON notation follows.

#### Transform

This component describes the position, orientation, and scale of an entity. It's JSON notation is:

```
"transform": {
  "position": [ Number, Number, Number ],
  "rotation": [ Number, Number, Number, Number ],
  "scale": [ Number, Number, Number ]
}
```

`position` is a 3d vector containing the world coordinates of the entity. `rotation` is a quaternion, and it's components should be normalized. `scale` is also a 3d vector, and its components should be non-zero.

#### Script

A script component can be attached to entities. Scripts are written in the Wren programming language. See below, in the Scripting API section, for more.

```
"script": String
```

#### Renderable

This component will be rendered with OpenGL. The component consists of a 3d model, and a shader. Currently, only the specular shader should be used at the moment.

**Component dependency: Transform**

```
"renderable": {
  "model": String,
  "material": {
    "shininess": Number,
    "baseColor": [ Number, Number, Number ],
    "ambientColor": [ Number, Number, Number ],
    "specularColor": [ Number, Number, Number ]
  }
}
```

`"model"` is the just the file path where the 3d model is located.

Here's the way shading works. The base color, diffuse color and specular color are all summed together to generate the final value. So if you want to turn, say specular shading, off, just set the specularColor elements to zero. The base color is what all the other colors are added on to, and it isn't affected by light.

#### PointLight

** Component dependency: Transform**

```
"pointLight": {
  "intensity": [ Number, Number, Number ],
  "attenuation": Number,
  "ambientCoefficient": Number
}
```

#### Camera

**Component dependency: Transform**

The camera component is used for rendering the renderable components.

```
"camera": {
  "fov": Number,
  "nearPlane": Number,
  "farPlane": Number 
}
```

## Scripting interface

See the `builtin` folder the Wren modules, and the `src/data` folder for any actual script files. It's very, very minimal for now. Here's an example for what a script looks like which makes an object rotate around the origin:

```js
import "builtin/math" for Math
import "builtin/entity" for Transform
import "builtin/vector" for Vec3
import "builtin/quaternion" for Quat

var activate = Fn.new {
  // gets called when the script component is created
}

var deactivate = Fn.new {
  // gets called just before the component is destroyed
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
```

You need to import everything you use, except for `Entity`. This gets done by the engine when the script is executed.

## Dependencies
* SDL2
  * for windowing and OpenGL access
* Assimp
  * model asset import
* GLEW
* Wren
  * scripting language
* Wrenly
  * C++ language bindings for Wren
* Filesentry
  * file system state change monitoring
  * used to reload files at runtime

