import "pg/queue" for Queue
import "pg/entity" for EntityManager, Entity

class EntityPool {
    construct new(count) {
        _queue = Queue.new()
        for (i in 0...count) {
            _queue.enqueue(EntityManager.create())
        }
    }

    dequeue() {
        return _queue.dequeue()
    }

}
