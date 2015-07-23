# Playground

## Compiling

The source code uses GCC pragmas.

### Linux
Run `make test` to build the tests, and then `make all` to build the program, copy resources and run all the tests.
The build will appear in the `./Build` folder.

### Windows
Compiling on Windows is... tricky at the moment. You can edit the included Makefile in order to add the library locations. Other than that, it should work in the
same way as the Linux compilation does.

## TODO
* Iterators for entities containing certain components
--* Iterator should point to the desired entity; `Entity::component` should be used for component access
* Destructor for EntityManager using these iterators
* Quaternion math
* Rendering matrices use Matrix4f
* Implement Transform component & rendering system using my math module


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

