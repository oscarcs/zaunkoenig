import "blt" for Terminal
import "event" for Events

class World {
    construct new() {
        _w = 100
        _h = 100

        _entities = []
        _map = List.filled(_w * _h, Tile.new(1, false))
        
        // Generate the map
        generate()

        Events.listener(this, "DRAW_WORLD")
        Events.listener(this, "CAN_MOVE")
        Events.listener(this, "MOVE")
    }

    // Hardcode the ID
    id { -1 }

    add(entity) {
        _entities.add(entity)
    }

    listen(event) {
        if (event.type == "DRAW_WORLD") {
        
            draw()
        
        } else if (event.type == "CAN_MOVE") {
        
            var payload = event.data.split(",")
            var x = Num.fromString(payload[0]) 
            var y = Num.fromString(payload[1]) 
            var id = Num.fromString(payload[2]) 
            if (!readMap(x, y).solid) {
                Events.queue_id(id, "MOVE", "%(x),%(y)")
            }
        }

    }

    draw() {
        //@@TODO: this is probably going to have to be smarter...
        Terminal.layer(0)
        Terminal.color(0xFFFFFFFF)
        Terminal.bkcolor(0xFFFFFFFF)

        for (x in 0...(_w-1)) {
            for (y in 0...(_h-1)) {
                var u = _map[x + _w * (y-1)]
                var d = _map[x + _w * (y+1)]
                var l = _map[(x-1) + _w * y]
                var r = _map[(x+1) + _w * y]
                _map[x + _w * y].draw(x, y, u, d, l, r)       
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
    }

    // x: x position
    // y: y position
    // u: type of tile above
    // d: type of tile below
    // l: type of tile left
    // r: type of tile right
    draw(x, y, u, d, l, r) {
        var tile = 0

        // Road
        if (_type == 0) {
            tile = 1
        } else if (_type == 1) {
            tile = 7 * 8
        }

        Terminal.tile(x, y, tile)
    }

    solid {
        return _solid
    }
}