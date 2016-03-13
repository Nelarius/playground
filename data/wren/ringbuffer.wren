
foreign class NumberRingBuffer {
    construct new( capacity ) {}
    
    // element access
    foreign at( index )
    foreign front()
    foreign back()

    // modifiers
    foreign pushBack( element )
    foreign popBack()
    foreign popFront()
    foreign clear()
    
    // container data
    foreign size
    foreign capacity
}
