
import "pg/stack" for Stack

class Queue {
    new {
        _inbox = new Stack
        _outbox = new Stack
    }

    // this seems to cause a seg fault
    // isEmpty { return _inbox.isEmpty && _outbox.isEmpty }
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
