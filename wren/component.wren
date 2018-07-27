import "event" for Event, Events
import "input" for Input
import "blt" for Terminal

class Component {
    construct new(parent) {
        _parent = parent
    }

    listen(event) {

    }

    id {
        return _parent.id
    }
}

class PlayerComponent is Component {
    construct new(parent) {
        super(parent)
        _x = 1
        _y = 1
        Events.listener(this, "KEYPRESS")
        Events.listener(this, "DRAW")
        Events.listener(this, "MOVE")
    }

    listen(event) {
        if (event.type == "KEYPRESS") {

            if (event.data == "UP")    Events.queue("CAN_MOVE", "%(_x),%(_y-1),%(id)")
            if (event.data == "DOWN")  Events.queue("CAN_MOVE", "%(_x),%(_y+1),%(id)")
            if (event.data == "LEFT")  Events.queue("CAN_MOVE", "%(_x-1),%(_y),%(id)")
            if (event.data == "RIGHT") Events.queue("CAN_MOVE", "%(_x+1),%(_y),%(id)")

        } else if (event.type == "MOVE") {

            var payload = event.data.split(",")
            var x = Num.fromString(payload[0])
            var y = Num.fromString(payload[1])
            _x = x
            _y = y

        } else if (event.type == "DRAW") {

            draw()

        }
    }

    draw() {
        Terminal.layer(1)
        Terminal.color(0xFFFF00FF)
        Terminal.print(_x, _y, "@")
    }
}