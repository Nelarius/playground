import "pg/test_framework" for testRunner
import "pg/assert" for Assert

class Stack {
    construct new() {
        _list = []
        _position = 0
    }

    count { _list.count }

    isEmpty { _list.count == 0 }

    push( val ) {
        _list.add( val )
    }

    pop() {
        if ( this.isEmpty ) {
            return null
        } else {
            var val = _list[-1]
            _list.removeAt(-1)
            return val
        }
    }
}

testRunner.test("Stack count is correct", Fn.new {
    var stack = Stack.new()
    Assert.isEqual(0, stack.count)
    stack.push(1)
    stack.push(2)
    Assert.isEqual(2, stack.count)
})

testRunner.test("Stack behaves as expected", Fn.new {
    var stack = Stack.new()
    stack.push(1)
    stack.push(2)
    Assert.isEqual(2, stack.pop())
    Assert.isEqual(1, stack.pop())
    Assert.isNull(stack.pop())
})
