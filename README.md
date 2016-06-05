# The Playground engine

A small, experimental, data-driven, entity-component-based, and Wren-scriptable game engine. I'm using this project for learning how game engines work.

## Build

Visual studio builds are supported via the `premake` build system. Build files are generated in the `build/<vs action>/` folder. Dependencies are included in the repository, so all you need to do is `premake5 vs2015`.

## Organization

The C/C++ source code is in `src/`. Some documentation exists within the `src` folder for the contained modules. The builtin Wren scripts that the engine uses are in `data/wren`. The shader source code is in `data/glsl`.

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

**Component dependency: Transform**

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

A flexible scripting system is under development. Hotswapping of Wren scripts while the engine is running is currently supported.

You need to import everything you use, except for `Entity`. This gets done by the engine when the script is executed.

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

#### Mouse Events

Here's how you handle mouse presses:

```js
var activate = Fn.new {
    EventManager.listenToMouseDown(entity, "Left")
}

var onMouseDown = Fn.new { |button|
    // do stuff
}
```

Currently valid mouse button events to subscribe to are `Left, Middle, Right`.

#### Keyboard events

Here are the currently valid keyboard events to subscribe to:

```
KeyNull, KeyReturn, KeyEscape, KeyBackspace, KeyTab, KeySpace, 
KeyExclaim, KeyQuoteDouble, KeyHash, KeyPercent, KeyDollar, KeyAmpersand,
KeyA, KeyB, ..., Key1, Key2, ..., KeyF1, KeyF2, ...
```

#### Systems

All systems are defined in `systems.wren`.

##### 3d Picking

Cast a camera ray from a screen coordinate, and get the entity that the ray struck.

```js
import "builtin/systems" for Pick3d
import "builtin/mouse" for Mouse

// returns an entity
var e = Pick3d.castCameraRay(Mouse.x, Mouse.y)
if (e.isValid()) {
    // do magic here
}
```

##### Debug renderer

Add debug rendering primitives. Currently provided debug rendering primitives are the cube and line. Debug rendering primitives can be either static, or you can give them a life time, after which they will disappear. Debug rendering primitives can also be given a color.

```js
import "builtin/systems" for DebugRenderer

// add a debug box with a position, scale, and color
// the lifetime, in seconds, is an optional fourth parameter
DebugRenderer.addDebugBox(Vec3.new(0.0, 0.0, 0.0), Vec3.new(1.0, 1.0, 1.0), Vec3.new(0.8, 0.2, 0.2))
```

## Bizarre issues
* Calling a constructor in a Wren script's `onMouseScroll` event handler causes a segfault due to the current fiber being null.
