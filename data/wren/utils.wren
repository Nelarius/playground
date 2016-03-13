
/*
 * A resource handle at the moment is an interned string (a string stores as a hash
 * value). The idea is to use these lightweight strings to refer to resources in the
 * file system. Construct once, and reuse without any performance penalties due to
 * strings!
 */
foreign class ResourceHandle {
    construct new( str ) {} // construct with a string
    foreign toString
}
