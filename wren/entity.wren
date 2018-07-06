import "component" for Component, PlayerComponent

var NextID = 0

class Entity {
    construct new(id, parent) {
        _id = id
        NextID = NextID + 1
        _parent = parent
        _components = []
    }

    add(component) {
        _components.add(component)
    }

    listen(event) {
        
    }

    id {
        return _id
    }
}

class EntityFactory {
    construct new(world) {
        _world = world
    }

    create(type) {
        var e = Entity.new(NextID, _world)
        if (type == "PLAYER") {
            e.add(PlayerComponent.new(e))
        }
        return e
    }
}