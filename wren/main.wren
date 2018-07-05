import "blt" for Terminal
import "game" for Game

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
    }
}
