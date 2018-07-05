class World {
    construct new() {
        _entities = []
    }

    add(entity) {
        _entities.add(entity)
    }

    listen(event) {
        for (entity in _entities) {
            entity.listen(event)
        }  
    }
}