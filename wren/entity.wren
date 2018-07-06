import "component" for Component, PlayerComponent

class Entity {
    construct new(parent) {
        _parent = parent
        _components = []
    }

    add(component) {
        _components.add(component)
    }

    listen(event) {
        
    }
}

class EntityFactory {
    construct new(world) {
        _world = world
    }

    create(type) {
        var e = Entity.new(_world)
        if (type == "PLAYER") {
            e.add(PlayerComponent.new(e))
        }
        return e
    }
}