
/*
 * @brief Sort a list.
 * @param l: {List} the list to sort
 * @param predicate: {Fn} the predcicate for comparing entries in the list.
 * The predicate must take two arguments (left, right) and must represent
 * the operation (left <= right).
 **/
var bubbleSort = Fn.new { |l, predicate|
    var swap = new Fn { |l, i1, i2|
        var temp = l[i1]
        l[i1] = l[i2]
        l[i2] = temp
    }
    var n = l.count
    var swapped = true
    while ( swapped ) {
        swapped = false
        for ( i in 1...n ) {
            // if left is not less than right
            if ( !predicate.call( l[i-1], l[i] ) ) {
                // swap & remember that something changed
                swap.call( l, i-1, i )
                swapped = true
            }
        }
        n = n-1
    }
}
