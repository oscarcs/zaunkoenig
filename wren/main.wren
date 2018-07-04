import "blt" for BearLibTerminal

class Roguelike {
    static main() {
        BearLibTerminal.open()
        BearLibTerminal.print(1, 2, "lol")
        BearLibTerminal.refresh()
    }
}