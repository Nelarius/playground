
class Stack {
    new {
        _list = []
        _position = 0
    }

    isEmpty { _position == 0 }

    push( val ) {
        _list.add( val )
        _position = _position + 1
    }

    pop() {
        if ( this.isEmpty ) {
            return null
        } else {
            var val = _list[ _position - 1 ]
            _list.removeAt( _position - 1 )
            _position = _position - 1
            return val
        }
    }
}