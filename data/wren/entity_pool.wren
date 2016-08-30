import "pg/queue" for Queue
import "pg/entity" for EntityManager, Entity

class EntityPool {
    construct new(count) {
        _queue = Queue.new()
        for (i in 0...count) {
            _queue.enqueue(EntityManager.create())
        }
    }

    count { _queue.count }

    fill(count) {
        for (i in 0...count) {
            _queue.enqueue(EntityManager.create())
        }
    }

    dequeue() {
        Assert.isFalse(_queue.isEmpty, "Dequeued an entity from an empty entity pool!")
        return _queue.dequeue()
    }

}
