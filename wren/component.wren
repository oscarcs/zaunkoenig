import "event" for Event, Events
import "input" for Input
import "blt" for Terminal

class Component {
    construct new(parent) {
        _parent = parent
    }

    listen(event) {

    }
}

class PlayerComponent is Component {
    construct new(parent) {
        super(parent)
        _x = 1
        _y = 1
        Events.listener(this, "KEYPRESS")
        Events.listener(this, "DRAW")
    }

    listen(event) {
        if (event.type == "KEYPRESS") {

            if (Input.isMoveUp(event.data))    _y = _y - 1
            if (Input.isMoveDown(event.data))  _y = _y + 1
            if (Input.isMoveLeft(event.data))  _x = _x - 1
            if (Input.isMoveRight(event.data)) _x = _x + 1

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