# Playground

A small, experimental, data-driven, entity-component-based, Lua-scriptable game engine. See below for a small programming example.

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
    rotation = {Quaternion},
    scale = {Vector3f}
}
```

The custom data type `pg.Vector3f` can be used to describe vectors in the scene script. They have the following constructor:

```lua
vec = pg.Vector3f( 0.0, 0.0, 0.0 )
```

`Quaternion` can be used to calculate rotations. For instance, you could describe a 0.5 radian rotation about the z-axis like
```cpp
q = pg.Quaternion( 0.0, 0.0, math.sin( 0.5 ), math.cos( 0.5 ) )
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
    <material> = {Table}
}
```

There are currently two `material` types supported: ambient and specular shading. The only data we need to give the ambient material is the base color. We do it like this.

```lua
ambient = {
  color = pg.Vector3f( 0.5, 0.5, 0.1 )
}
```

More about the specular shading later.

#### PointLight

This component casts light on all renderables in the scene.

*Component dependency: transform*

```lua
pointLight = {
    intensity = pg.Vector3f( 1.0, 1.0, 1.0 )
    attenuation = 0.2
    ambientCoefficient = 0.005
}
```

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
            position = pg.Vector3f( 0.0, 0.0, -2.0 ),
            scale = pg.Vector3f( 1.0, 1.0, 1.0 )
        },
        camera = {
            fov =           1.15,
            nearPlane =    0.1,
            farPlane =     1000.0,
            perspective =   true,
            active =        true
        }
    },
    
    {
        transform = {
            position = pg.Vector3f( 0.0, 0.0, 0.0 ),
            scale = pg.Vector3f( 1.0, 1.0, 1.0 )
        },
        renderable = {
            model = "data/puck.obj",
            specular = {
                shininess = 80.0,
                specularColor = pg.Vector3f( 1.0, 1.0, 1.0 ),
                ambientColor = pg.Vector3f(0.941, 0.455, 0.804 )
            }
        }
    }
    
    
}
```

`data/template.lua`:

```lua
-- this gets called just after the component is assigned
function activate()
end

-- accumulated time
t = 0.0
-- angular velocity
av = 0.5
r = 8.0

-- this gets called in the update loop
function update( dt )
    if entity:hasTransform() then
        t = t + dt
        entity.transform.position = pg.Vector3f( r*math.cos( t*av ), 0.0, r*math.sin( t*av ) )
    end
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
* Reconsider the assertions in MeshManager and ShaderManager.
* Towards the first demo
  * Figure out the time unit deltas in radians
  * instantiate three times
* Write tests for Bundle
* Implement specular shaders
  * Add array of lights to specular shader
  * Add Light count
  * Read orientation from DirectionalLight component
  * add gamma correction to specular shader
* Material should use a union of material type structs, get rid of the verbosity of per-float uniform names.
  * I think this is justified, because the Render system is the one which provides the rendering service; materials are a part of that process.
  * Drawback: makes extension more difficult. Not a problem for now.
* Implement free-look camera script
* Normalize resource names in MeshManager and ShaderManager using r-lyeh's Unify lib.
* Rendering matrices use Matrix4f
* Implement Transform component & rendering system using my math module
* Figure out a mechanism to make `componentPointer<C>` private/not part of Entity API.
* Bundle has iterators
* Add R'lyeh's profit lib to profile execution times of critical methods
* Integrate ImGui into a system
  * I need to figure out how to render textured panels isometrically using modern OpenGL
    * This is explained in `imgui/examples/opengl3_example/imgui_impl_glfw_gl3::CreateDeviceObjects()`
  * Add event handling functions for the Ui system:
  * Add handler for text input event
  * add handler for keyup input event
* `EntityManager` needs to expose the pool size in its public API.
Here's a great way to handle developer errors: if, for instance, a mesh isn't found, use an "error" mesh instead which will be easy to spot. There is no need to halt execution because of an erroneous path to a resource.

Remember to distinguish between hard and soft errors. Hard errors occur in places like `Bundle` where an off-by one error leads to corrupted state. Soft errors occur e.g. when a user supplies an incorrect file name. The program still works correctly, it just has been given incorrect information.

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

