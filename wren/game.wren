import "random" for Random
import "blt" for Terminal
import "world" for World
import "entity" for Entity, EntityFactory
import "event" for Event, Events
import "input" for Input

var RNG = Random.new()

class Game {
    construct new() {
        _world = World.new()
        _entityFactory = EntityFactory.new(_world)
        _world.add(_entityFactory.create("PLAYER"))
        draw()
    }

    draw() {
        Terminal.bkcolor(0xFF000000)
        Terminal.clear()
        Events.queue("DRAW_WORLD", null)
        Events.queue("DRAW", null)
        Terminal.refresh()
    }

    input(key) {
        key = Input.normalize(key)
        Events.queue("KEYPRESS", key)

        draw()
    }
}