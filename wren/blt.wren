class Terminal {
    foreign static open()
    foreign static close()
    foreign static refresh()
    foreign static clear()
    foreign static clear_area(x, y, w, h)
    foreign static set(settings)
    foreign static put(x, y, code)
    foreign static pick(x, y, index)
    foreign static print(x, y, str)
    foreign static print_ext(x, y, width, height, align, str)
    foreign static state(slot)
    foreign static color(color)
    foreign static bkcolor(color)
    foreign static layer(layer)
    foreign static delay(period)
    
    // Wrapper method for tile drawing
    static tile(x, y, tile) {
        put(x, y, tile + 0x80)
    }

    // Wrapper method for printing text
    static text(x, y, text) {
        print(x, y - 1, text)
    }
}