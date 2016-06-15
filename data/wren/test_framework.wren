import "pg/assert" for Assert

class Test {
    construct new( name, body ) {
        _name = name
        _fiber = Fiber.new {
            body.call()
        }
    }

    try() {
        _fiber.try()
    }
    abort(msg) {
        _fiber.abort(msg)
    }
    error { _fiber.error }
    name { _name }
}

class TestRunner {

    construct new() {
        _tests = []
    }

    add( name, body ) {
        _tests.add(Test.new(name, body))
    }

    test(name, body) {
        var t = Test.new(name, body)
        t.try()
        if (t.error) {
            System.print("Test \"%(name)\" failed with %(t.error)")
        } else {
            System.print("Test \"%(name)\" OK")
        }
    }

    run() {
        var failedCount = 0
        for ( t in _tests) {
            t.try()
            if (t.error) {
                failedCount = failedCount + 1
                System.write("\nTest\"%(t.name)\" failed with %(t.error)\n\n")
            } else {
                System.print("Test \"%(t.name)\" OK")
            }
        }
        if (failedCount > 0) {
            System.print("\n%(failedCount) out of %(_tests.count) tests failed.")
        } else {
            System.print("\n%(_tests.count) tests passed.")
        }
    }
}

var testRunner = TestRunner.new()
