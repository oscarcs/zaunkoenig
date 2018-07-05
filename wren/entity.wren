import "component" for Component, PlayerComponent

class Entity {
    construct new() {
        _components = []
    }

    add(component) {
        _components.add(component)
    }

    listen(event) {
        for (c in _components) {
            c.listen(event)
        }
    }
}

class EntityFactory {
    construct new() {

    }

    create(type) {
        var e = Entity.new()
        if (type == "PLAYER") {
            e.add(PlayerComponent.new())
        }
        return e
    }
}

class Event {
    construct new(type, data) {
        _type = type
        _data = data
    }

    type {
        return _type
    }

    data {
        return _data
    }
}