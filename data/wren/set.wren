import "pg/test_framework" for testRunner
import "pg/assert" for Assert

class Set {
    construct new() {
        _map = {}
    }

    values { _map.keys }

    insert(value) {
        _map[value] = null
    }

    contains(value) {
        return _map.containsKey(value)
    }

    remove(value) {
        _map.remove(value)
    }
}

testRunner.test("After adding value to set, it is contained", Fn.new {
    var set = Set.new()
    set.insert(5)
    Assert.isTrue(set.contains(5))
})

testRunner.test("After removing a value from the set, it is no longer contained", Fn.new {
    var set = Set.new()
    set.insert(5)
    set.remove(5)
    Assert.isFalse(set.contains(5))
})
