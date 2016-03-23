
import "pg/stack" for Stack

class Queue {
    construct new() {
        _inbox = Stack.new()
        _outbox = Stack.new()
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
