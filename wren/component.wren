import "entity" for Event
import "input" for Input
import "blt" for Terminal

class Component {
    construct new() {

    }

    listen(event) {

    }
}

class PlayerComponent is Component {
    construct new() {
        _x = 1
        _y = 1
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
        Terminal.color(0xFFFF00FF)
        Terminal.bkcolor(0xFFFFFF00)
        Terminal.print(_x, _y, "@")
        Terminal.color(0xFFFFFFFF)
        Terminal.bkcolor(0xFF000000)
    }
}