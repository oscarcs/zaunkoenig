import "blt" for Terminal
import "event" for Events

class World {
    construct new() {
        _w = 100
        _h = 100

        _entities = []
        _map = List.filled(_w * _h, Tile.new(0, false))
        
        // Generate the map
        generate()

        Events.listener(this, "DRAW_WORLD")
        Events.listener(this, "CAN_MOVE")
        Events.listener(this, "MOVE")
    }

    add(entity) {
        _entities.add(entity)
    }

    listen(event) {
        if (event.type == "DRAW_WORLD") {
        
            draw()
        
        } else if (event.type == "CAN_MOVE") {
        


        }

    }

    draw() {
        //@@TODO: this is probably going to have to be smarter...
        Terminal.layer(0)

        for (x in 0..._w) {
            for (y in 0..._h) {
                _map[x + _w * y].draw(x, y)       
            }
        }
    }

    readMap(x, y) {
        return _map[x + _w * y]
    }

    writeMap(x, y, t) {
        _map[x + _w * y] = t
    }

    generate() {
        for (x in 15...20) {
            for (y in 15...20) {
                writeMap(x, y, Tile.new(1, true))
            }
        }
    }
}

class Tile {
    construct new(type, solid) {
        _type = type
        _solid = solid

        __typeToTile = {
            0: ".",
            1: "#"
        }

        __typeToColor = {
            0: 0xFF006019,
            1: 0xFF000000
        }

        __typeToBkcolor = {
            0: 0xFF004713,
            1: 0xFF004713
        }
    }

    draw(x, y) {
        var tile = __typeToTile[_type]
        Terminal.color(__typeToColor[_type])
        Terminal.bkcolor(__typeToBkcolor[_type])
        Terminal.print(x, y, tile)
    }
}