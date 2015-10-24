
# TODO

* The whole event handling framework needs to overhauled.
  * An `EventHandler` class gets passed events.
  * Commands are registered with certain events, such as key down, mouse button down, etc.
  * MouseEvents, KeyboardHandler are not needed.
  * Each application state is responsible for filling the event handler with events.
  * In this way, no events are erroneously "leaked" to application states lower in the state stack.
* Reconsider the assertions in MeshManager and ShaderManager.
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
* Think about the time step. See `Game Programming Patterns::Game Loop` chapter for some ideas about variable time step vs. fixed time step with rendering independent of the updating.

Consider adding the following API to `VertexArrayObjectFactory`:

```cpp
opengl::VertexArrayObject vao = opengl::AttributeHandler{}
  .add( opengl::VertexAttribute::Vertex )
  .add( opengl::VertexAttribute::Normal )
  .end();   // this returns the actual VertexArrayObject instance, which gets moved into vao
``` 

### Handling hard and soft errors

Remember to distinguish between hard and soft errors. Hard errors occur in places like `Bundle` where an off-by one error leads to corrupted state. Soft errors occur e.g. when a user supplies an incorrect file name. The program still works correctly, it just has been given incorrect information.

For MeshManager: if a mesh isn't found, return an easily recognizable default model.

Likewise, if a texture material isn't found, return a horrifically ugly default texture.

### Hot swapping Wren code

When each script component contains its own VM instance, the entire VM can be swapped out for a new instance during run time. The state is stored via the JSON serialization mechanism; just before the current VM instance is swapped out, the `toJSON` function is called on it, and the resulting string is stored. Then a new instance with the new source is created and executed, and it is deserialized using the string.

In order for this to work, the relevant files must be watched.

How do we know which script components to hot-swap? Each component stores a hash "handle" which indicates their source. The Wren system stores a tree, from which the actual file name can be queried. The dependencies of each script are parsed, and those files are hashed too. The tree file indicates the dependency hierarchy. The file watch system watches all files in the dependency hierarchy. If one of the files in the dependency hierarchy are changed, it walks up the tree to figure out which script hash needs to be reloaded.

Then, all we have to do is iterate over all the script components and do the afore mentioned hot-swap on them.

### Conservative frustum culling

For each renderable, a bounding box component is assigned. A bounding box is simple three vertices. Now, when we iterate through the renderable components, we will not render an object if none of the bounding box vertices are contained in the view frustum volume.

The bounding box may be one of two types:
* AABB, which is trivial to calculate
* OBB, which requires PCA to calculate the three axes along which the maximal extents of the model are oriented on
