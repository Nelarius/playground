# Playground

## Compiling

The source code uses GCC pragmas and C++14.

### Linux
Run `make test` to build the tests, and then `make all` to build the program, copy resources and run all the tests.
The build will appear in the `./Build` folder.

### Windows
Compiling on Windows is... tricky at the moment. You can edit the included Makefile in order to add the library locations. Other than that, it should work in the
same way as the Linux compilation does.

## TODO
* Read & render basic components
  * Transform, Renderable, Camera, Script components need to be defined
  * For script component, must create a pool of lua states
  * visible components need to be described in Lua scene file
  * 
* Get rid of compiler warning for narrowing conversion in Bundle.h
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
The happy bunny license is the MIT license, except that you're discouraged to use it for
military purposes...
Feel free to use and redistribute, but include the license
### EntityX
MIT license
### glew
MIT license
### Lua
MIT license

