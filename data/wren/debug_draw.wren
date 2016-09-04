
class DebugDraw {
    // {Vec3} center
    // {Vec3} color
    // {Vec3} extents
    foreign static box(center, color, extents)

    // {Vec3} center
    // {Vec3} color
    // {Num}  radius
    foreign static sphere(center, color, radius)

    // {String} text
    // {Vec3}   position
    // {Vec3}   color
    // {Mat4}   camera view-projection matrix
    foreign static projectedText(text, position, color, vpMat)
}
