import "random" for Random
import "blt" for Terminal
import "world" for World
import "entity" for Entity, EntityFactory, Event
import "input" for Input

var RNG = Random.new()

class Game {
    construct new() {
        _world = World.new()
        _entityFactory = EntityFactory.new()
        _world.add(_entityFactory.create("PLAYER"))
    }

    draw() {
        Terminal.clear()
        _world.listen(Event.new("DRAW", null))
        Terminal.refresh()
    }

    input(key) {
        if (Input.isMovement(key)) {
            _world.listen(Event.new("KEYPRESS", key))
        }

        draw()
    }
}