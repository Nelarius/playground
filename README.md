# Playground

A small, experimental, data-driven, entity-component-based, OpenGL-enabled, Lua-scripted game engine. See below for a small programming example.

----------


## Compiling

The source code uses GCC pragmas and C++14.

#### Linux
On Linux, once the dependencies have been installed, just run `make` to build the program and run the tests. The result will appear in the `./Build` folder.

#### Windows
Compiling on Windows isn't a great experience at the moment. Your locations of the dependencies should be entered into the `*_COMP` and `*_LINK` fields, at the beginning of the Makefile. Once that has been done, the build is done the same way as on Linux.

## Usage

The scene is described in a data file, which is actually nothing but Lua code. The scene file is required to be in `data/scene.lua` for now.

Entities are stored in the global table `entities`. Each entity is an anonymous table, containing components. The following components can be assigned to an entity.

#### Transform

This component describes the position and scale of an entity. There is no rotation for now, due to the lack of quaternions in my math library.

```lua
transform = {
    position = {Vector3f},
    scale = {Vector3f}
}
```

The custom data type `pg.Vector3f` can be used to describe vectors in the scene script. They have the following constructor:

```lua
vec = pg.Vector3f( 0.0, 0.0, 0.0 )
```

#### Script

Entities can be scripted in Lua. The script component simply contains the location of the script file. More about scripting later.

```lua
script = {String}
```

#### Renderable

This component will be rendered with OpenGL. The component consists of a 3d model, and a shader. The following two shaders can be used at the moment:


*Component dependency: transform*

```lua
renderable = {
    model = {String}
}
```

The `Vector3f` is a custom data type, which can be used to describe vectors. 

#### Camera

The camera component is used for rendering the renderable components.

*Component dependency: transform*

```lua
camera = {
    fov: {Number}, -- the vertical field of view, in radians
    near_plane: {Number}, -- the near plane of the view frustum
    far_plane: {Number}, -- the far plane of the view frustum
    perspective: {Boolean}, -- true, to view in perspective projection, false otherwise
    active: {Boolean} -- true, to use this camera for rendering, false otherwise
}
```

#### A small scene

Here is a small example of how to build a simple scene.

`data/scene.lua`:

```lua
entities = {
    {
        script = "data/template.lua",
        transform = {
            position = pg.Vector3f( 0.0, 0.0, 0.0 ),
            scale = pg.Vector3f( 1.0, 1.0, 1.0 )
        },
        camera = {
            fov =           1.15,
            near_plane =    0.1,
            far_plane =     1000.0,
            perspective =   true,
            active =        true
        }
    }
}
```

`data/template.lua`:

```lua

v = pg.Vector3f( 10, 0.0, 0.0 )

-- this gets called just after the component is assigned
function activate()
    print( entity:hasTransform() )
end

-- this gets called in the update loop
function update( dt )
    ds = pg.Vector3f( v.x*dt, v.y*dt, v.z*dt )
    -- this really isn't optimum
    -- but so far I have no luck in binding custom methods to __add, and __mul
    entity.transform.position.x = entity.transform.position.x + ds.x
    entity.transform.position.y = entity.transform.position.y + ds.y
    entity.transform.position.z = entity.transform.position.z + ds.z
end

--  this gets called just before the component is removed
function deactivate()
    print("Script gonna hang up now!")
end
```

`config.lua`:

```lua

targetFrameRate = 60.0

window = {
    width = 800,
    height = 600,
    name = "Playground engine",
    opengl = {
        major = 3,
        minor = 3,
        stencil_bits = 8,
        depth_bits = 24,
        multisample_buffers = 1,
        multisample_samples = 4
    }
}

```

## Scripting interface

Here is a list of functions and variables available for use in the script component.

## TODO
* Read & render basic components
  * visible components need to be described in Lua scene file
  * create a simple scene in C++ to test systems and components
* Bind Matrix to Lua
* Quaternion math
* Rendering matrices use Matrix4f
* Implement Transform component & rendering system using my math module
* Scripting system needs context, managers, and ECS subsystems to be bound
  * Entity needs to be bound in such a way that we have direct access to component pointers
  * There needs to be read/write access for the components.
* GameState, and other app states should be a part of app.
* Bundle has iterators
* Add R'lyeh's profit lib to profile execution times of critical methods

## Dependencies
### SDL2
zlib license
Feel free to do anything you want with it, so long as you don't misrepresent who wrote the original software, license must be included
### glm
Happy bunny license, MIT license
Feel free to use and redistribute, but include the license
### glew
MIT license
### Lua 5.2
MIT license
### Assimp
3-clause BSD license, do what you want, but include the license text.
### LuaBridge
MIT license

