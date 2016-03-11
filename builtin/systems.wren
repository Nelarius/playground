
foreign class Pick3d {

    /*
     * Returns the enity that the camera ray hit. If it didn't hit anything, then the ray is invalid.
     */
    foreign static castCameraRay(x, y)
}

class DebugRenderer {
    foreign static addDebugBox(position, scale, color)
    foreign static addDebugBox(position, scale, color, lifeTime)
}
