# Wren API documentation

## Using the test frame work

In order to run tests, you need to import the test framework:

```js
import "pg/test_framework" for testRunner
```

You register your tests with the testRunner variable. The test consists of a description string, and a function variable:

```js
import "pg/assert" for Assert
import "pg/test_framework" for testRunner

testRunner.test("This will always fail", Fn.new {
    Assert.isTrue(false)
})
```

Registering tests does not execute them yet, however. To do so, call `testRunner`'s `run` method:

```js
// this will execute all tests and print the report to the command line
testRunner.run()
```

You can use the engine to execute your tests. From the command line, pass the module containing your tests to the engine:

```sh
./engine --test my_test.wren
```

This assumes that the `testRunner.run()` invocation is in `my_test.wren`.

#### Using fixtures

You can register a test along with a fixture class. The test body will be called with a new instance of the fixture for every test.

```js
import "pg/assert" for Assert
import "pg/test_framework" for testRunner

class MyFixture {
    construct new() {
        _list = []
    }

    list { _list }
}

testRunner.testFixture("List is zero on initialization", MyFixture, Fn.new { |fixture|
    Assert.isEqual(0, fixture.count)
})
```
