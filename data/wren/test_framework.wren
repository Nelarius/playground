import "pg/assert" for Assert

class Test {
    construct new(name, fixture, body) {
        _name = name
        _fixtureType = fixture
        _fiber = Fiber.new {
            if (_fixtureType != null) {
                var fixture = _fixtureType.new()
                body.call(fixture)
            } else {
                body.call()
            }
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

    test(name, body) {
        _tests.add(Test.new(name, null, body))
    }

    testFixture(name, fixtureType, body) {
        _tests.add(Test.new(name, fixtureType, body))
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
