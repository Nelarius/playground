
class Assert {
    static abortCurrentFiber(msg) {
        Fiber.abort("assertion error: %(msg)")
    }

    static isTrue(expr, str) {
        if (expr != true) {
            abortCurrentFiber("isTrue " + str)
        }
    }

    static isTrue(expr) {
        isTrue(expr, "")
    }

    static isFalse(expr, str) {
        if (expr != false) {
            abortCurrentFiber("isFalse. " + str)
        }
    }

    static isFalse(expr) {
        isFalse(expr, "")
    }

    static isEqual(a, b, str) {
        if (a != b) {
            abortCurrentFiber("isEqual, values are %(a) and %(b). " + str)
        }
    }

    static isEqual(a, b) {
        isEqual(a, b, "")
    }

    static notEqual(a, b, str) {
        if (a == b) {
            abortCurrentFiber("notEqual, values are %(a) and %(b). " + str)
        }
    }

    static isNull(value, str) {
        if (value != null) {
            abortCurrentFiber("isNull. " + str)
        }
    }

    static isNull(value) {
        isNull(value, "")
    }

    static notNull(value, str) {
        if (value == null) {
            abortCurrentFiber("notNull. " + str)
        }
    }
    static notNull(value) {
        notNull(value, "")
    }

    static succeed(callable) {
        var fiber = Fiber.new {
            callable.call()
        }
        fiber.try()
        if (fiber.error != null) {
            abortCurrentFiber("%(callable) aborts with error %(fiber.error)")
        }
    }

    static fail(callable) {
        var fiber = Fiber.new {
            callable.call()
        }
        fiber.try()
        if (fiber.error == null) {
            abortCurrentFiber("%(callable) doesn't abort")
        }
    }
}
