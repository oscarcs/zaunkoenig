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
}