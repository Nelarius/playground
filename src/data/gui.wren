import "builtin/imgui" for Imgui, ImguiFlag
import "builtin/array" for NumberArray
import "builtin/math" for Math
import "builtin/vector" for Vec2

var activate = Fn.new{
    //
}

var deactivate = Fn.new{
    //
}

var pi = 3.1415927
var t = 0.0
var size = Vec2.new(400, 300)
var graphSize = Vec2.new(300, 150)

var update = Fn.new { | dt |
    var a = NumberArray.new()
    for (i in 1..10) {
        a.pushBack(Math.rand(0.0, 3.0))
    }

    var opt = ImguiFlag.new()
    opt.setShowBorders()

    Imgui.begin("wren", opt)
    if (Imgui.treeNode("wren node")) {
        Imgui.text("text from wren")
        Imgui.treePop()
    }
    var node2 = Fn.new {
        Imgui.text("testing the wrapper")
        Imgui.plotNumberArray( "asdf", a, 10, 0, graphSize )
    }
    Imgui.treeNode( "node2", node2 )
    Imgui.end()
}
