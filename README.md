# The Playground engine

A small, experimental, data-driven, entity-component-based, and Wren-scriptable game engine. I'm using this project for learning how game engines work.

## Building

The build system uses premake5, although some dependencies have to be built separately before buiding the engine iteslf. All dependencies are included in the `extern/` folder. SDL, Assimp, UnitTest++, and GLEW have to be built first. Currently, only the Visual Studio build has actually been tested, although full Linux and OSX support is possible, in theory.

The submodules that have to be built have been modified in such a way, that all `build/` folders are ignored.

### Visual Studio

* SDL can be built by building the solution in `VisualC/`.
* Assimp can be built by first generating the solution via CMake. IMPORTANT: remove library suffixes in the configuration stage, we don't want the compiler's name in the library. Premake links with the shared library.
* Glew: build the solution in the `vc14/` folder. Premake links with the shared library.
* CMake, then build the solution. Premake links with the static library.

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

See the `builtin` folder the Wren modules, and the `src/data` folder for actual script files. It's very, very minimal for now. For example, here's a WASD movement script:

```js
import "builtin/math" for Math
import "builtin/vector" for Vec3
import "builtin/quaternion" for Quat
import "builtin/event" for EventManager
import "builtin/component" for Transform

var activate = Fn.new{
    EventManager.listenToKeyPressed(entity, "KeyW")
    EventManager.listenToKeyPressed(entity, "KeyA")
    EventManager.listenToKeyPressed(entity, "KeyS")
    EventManager.listenToKeyPressed(entity, "KeyD")
}

var deactivate = Fn.new{
    //
}

var onKeyDown = Fn.new { |key|
    //
}

var speed = 10.0

var pos = Vec3.new( 0.0, 0.0, 10.0 )
var scale = Vec3.new( 1.0, 1.0, 1.0 )
var rot = Quat.new( 0.0, 0.0, 0.0, 1.0 )

var timeDelta = 0.016

var onKeyPressed = Fn.new { |key|
    if (key == "KeyW") {
        pos.z = pos.z - speed * timeDelta
    } else if (key == "KeyA") {
        pos.x = pos.x - speed * timeDelta
    } else if (key == "KeyS") {
        pos.z = pos.z + speed * timeDelta
    } else if (key == "KeyD") {
        pos.x = pos.x + speed * timeDelta
    }
    entity.transform = Transform.new(pos, rot, scale)
}

var onKeyUp = Fn.new { |key|
    //
}

var update = Fn.new { | dt |
    timeDelta = dt
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

