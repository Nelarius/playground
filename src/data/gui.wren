import "builtin/imgui" for Imgui, ImguiFlag
import "builtin/array" for NumberArray
import "builtin/ringbuffer" for NumberRingBuffer
import "builtin/math" for Math
import "builtin/vector" for Vec2
import "builtin/entity" for EntityManager
import "builtin/utils" for ResourceHandle

var activate = Fn.new {
    var cubeFile = ResourceHandle.new("data/cube.obj")
    System.print("cubeFile = %(cubeFile)")
}

var deactivate = Fn.new {
    //
}

var pi = 3.1415927
var t = 0.0
var size = Vec2.new(400, 300)
var graphSize = Vec2.new(500, 300)

var r = NumberRingBuffer.new( 600 )

var update = Fn.new { | dt |
    t = t + dt
    r.pushBack(Math.sin(t))

    var opt = ImguiFlag.new()
    opt.setShowBorders()

    Imgui.begin("wren", opt)
    if (Imgui.treeNode("wren node")) {
        Imgui.text("text from wren")
        Imgui.treePop()
    }
    var node2 = Fn.new {
        //Imgui.plotArray( "asdf", a, 10, 0, graphSize )
        Imgui.plotRingBuffer( "noise", r, graphSize )
    }
    Imgui.treeNode( "graph node", node2 )
    Imgui.end()
}
