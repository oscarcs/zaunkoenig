import "blt" for Terminal

var Roguelike

class App {
    static main() {
        Terminal.open()
        Terminal.refresh()
        Terminal.set("window.title='Zaunkönig';")
        Roguelike = Game.new()
    }

    static input(key) {
        Roguelike.input(key)
        Roguelike.draw()
    }
}

class Game {
    construct new() {
        _x = 1
        _y = 1
    }

    draw() {
        Terminal.color(0xFFFF00FF)
        Terminal.bkcolor(0xFFFFFF00)
        Terminal.print(_x, _y, "@")
        Terminal.refresh()
        Terminal.color(0xFFFFFFFF)
        Terminal.bkcolor(0xFF000000)
        Terminal.clear()
    }

    input(key) {
        if (key == "UP")    _y = _y - 1
        if (key == "DOWN")  _y = _y + 1
        if (key == "LEFT")  _x = _x - 1
        if (key == "RIGHT") _x = _x + 1
    }
}