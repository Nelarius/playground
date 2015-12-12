
foreign class ImguiFlag {
    construct new() {}
    foreign setTitleBar()   // on by default
    foreign unsetTitleBar()
    foreign setResize()     // on by default
    foreign unsetResize()
    foreign setMove()       // on by default
    foreign unsetMove()
    foreign setShowBorders()    // off by default
    foreign unsetShowBorders()
}

class Imgui {

    // windows
    foreign static begin( name )    // begin a window scope
    foreign static begin( name, flags ) // begin a window scope with window flags
    foreign static end()            // end a window scope
    foreign static treeNode( name )
    foreign static treePop()
    foreign static text( str )
    foreign static textColored( color, str )

    // window control
    foreign static setNextWindowPos( size )
    foreign static setNextWindowSize( size )    // call before begin()

    // formatting
    foreign static spacing()
    foreign static dummy( size )    // add a dummy of a given size, as Vec2
    foreign static indent()
    foreign static unindent()

    // elements
    foreign static bullet()             // a bullet point without text
    foreign static bulletText( label )  // a bullet point with text
    foreign static button( label )
    foreign static button( label, size) // size is of type Vec2
    foreign static radioButton( label, active ) // WTF, how do I use this?
    foreign static plotNumberArray( label, array, count )
    foreign static plotNumberArray( label, array, count, offset )
    foreign static plotNumberArray( label, array, count, offset, size )

    static treeNode( name, body ) {
        if ( treeNode( name ) ) {
            body.call()
            treePop()
        }
    }
}
