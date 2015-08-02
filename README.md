# Playground

----------


## Compiling

The source code uses GCC pragmas and C++14.

### Linux
On Linux, once the dependencies have been installed, just run `make` to build the program and run the tests. The result will appear in the `./Build` folder.

### Windows
Compiling on Windows isn't a great experience at the moment. Your locations of the dependencies should be entered into the `*_COMP` and `*_LINK` fields, at the beginning of the Makefile. Once that has been done, the build is done the same way as on Linux.

## TODO
* Read & render basic components
  * Transform, Renderable, Camera, Script components need to be defined
  * For script component, must create a pool of lua states
  * visible components need to be described in Lua scene file
  * create a simple scene in C++ to test systems and components
* Bind Matrix to Lua
* Quaternion math
* Rendering matrices use Matrix4f
* Implement Transform component & rendering system using my math module
* Script component should store a LuaRef to the script function being called.
  * EntityManager, EventManager, SystemManager, events, and components all need to be bound to Lua
  * the current context (entity id) needs to be bound to some Lua variable
* Add R'lyeh's profit lib to profile execution times of critical methods
* Resource managers should switch over to using `ce::Bundle` instead of unique pointers.

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

