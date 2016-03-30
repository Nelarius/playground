# The managers

The managers in the Playground engine hold various resources which are used during runtime. The purpose is to save memory by storing the one instance of a resource that many different entities can refer to. As such, most of the managers implement the flyweight pattern.

## Mesh manager

The mesh manager holds OpenGL buffer objects. When a mesh file is loaded, the data is sent to the GPU, and the mesh manager retains the object handle to that buffer object. The mesh manager is simple to use. Just do:

```cpp
MeshManager models{};
// if the cow hasn't yet been loaded, it will be now
auto cow = models.get( "cow.obj" );
```

By default, the mesh manager contains a unit cube, with the file name of "cube". The buffer object of the cube is returned if you try to get a file not in the manager.

## Shader manager

The shader manager holds the shader programs that materials use. Shaders are inserted into the manager in two steps. First, the shader source is compiled into separate shader objects:

```cpp
ShaderManager shaders{};
shaders.addShader( "specular.vert.glsl", GL_VERTEX_SHADER );
shaders.addShader( "specular.frag.glsl", GL_FRAGMENT_SHADER );
```

The shader objects are added into an internal list. When we compile these objects into the final shader program, the list is cleared. When compiling, we additionally specify the string tag with which we can later get the shader program:

```cpp
shaders.compile( "specular_shader" );
```

Get shaders by writing

```cpp
auto specularShader = shaders.get( "specular_shader" );
```

## String manager

The string manage stores the contents of a file as a string. This is useful for e.g. scripts. Many components may refer to the same script, and we save a bit of time by not reloading the script from file each time.

Get strings by writing

```cpp
StringManager strings{};
auto source = strings.get( "script.wren" );
```
