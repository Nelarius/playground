import "pg/stack" for Stack
import "pg/test_framework" for testRunner
import "pg/assert" for Assert

class Queue {
    construct new() {
        _inbox = Stack.new()
        _outbox = Stack.new()
    }

    count {
        return _inbox.count + _outbox.count
    }

    isEmpty { _inbox.isEmpty && _outbox.isEmpty }

    enqueue( val ) {
        _inbox.push( val )
    }

    dequeue() {
        if ( _outbox.isEmpty ) {
            while ( ! _inbox.isEmpty ) {
                _outbox.push( _inbox.pop() )
            }
        }
        return _outbox.pop()
    }
}

testRunner.test("Queue count is correct", Fn.new {
    var queue = Queue.new()
    Assert.isEqual(0, queue.count)
    queue.enqueue(1)
    queue.enqueue(2)
    queue.enqueue(3)
    Assert.isEqual(3, queue.count)
    // inbox content emptied to outbox
    queue.dequeue()
    // add one more element to inbox
    queue.enqueue(4)
    Assert.isEqual(3, queue.count)
    queue.dequeue()
    queue.dequeue()
    // this element is from the inbox
    queue.dequeue()
    Assert.isEqual(0, queue.count)
})

testRunner.test("Queue behaves as expected", Fn.new {
    var queue = Queue.new()
    Assert.isEqual(0, queue.count)
    queue.enqueue(1)
    queue.enqueue(2)
    queue.enqueue(3)
    // inbox content emptied to outbox
    Assert.isEqual(1, queue.dequeue())
    // add one more element to inbox
    queue.enqueue(4)
    Assert.isEqual(2, queue.dequeue())
    Assert.isEqual(3, queue.dequeue())
    // this element is from the inbox
    Assert.isEqual(4, queue.dequeue())
    Assert.isNull(queue.dequeue())
})

testRunner.test("Count remains zero while dequeueing empty queue", Fn.new {
    var queue = Queue.new()
    Assert.isNull(queue.dequeue())
    Assert.isEqual(0, queue.count)
})
