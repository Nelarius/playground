
class EventManager {
    /*
     * Key values are formatted as "Key*", with letters capitalized.
     */
    foreign static listenToKeyDown(entity, key)
    foreign static listenToKeyPressed(entity, key)
    foreign static listenToKeyUp(entity, key)

    /*
     * Valid button values are "Left", "Middle", and "Right"
     */
    foreign static listenToMouseDown(entity, button)
    foreign static listenToMousePressed(entity, button)
    foreign static listenToMouseUp(entity, button)

    foreign static listenToMouseScroll(entity)
}
