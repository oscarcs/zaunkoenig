import "blt" for Terminal
import "game" for Game

var Roguelike

class App {
    static main() {
        Terminal.open()
        Terminal.refresh()
        Terminal.set("window: title='Zaunkönig', size=24x24, cellsize=32x32;")
        Terminal.set("font: res/font.ttf, size=32x48, size-reference=0, align=bottom-right, spacing=0x0;")
        Terminal.set("0x80: res/tiles.png, align=center, size=8x8, resize=32x32, resize-filter=nearest")
        Roguelike = Game.new()
    }

    static input(key) {
        Roguelike.input(key)
    }
}
