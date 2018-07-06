class Event {
    construct new(type, data) {
        _type = type
        _data = data
    }

    type {
        return _type
    }

    data {
        return _data
    }
}

class EventQueue {
    construct new() {
        //  Store the listeners a map, mapping an event type to a list of components.
        _listeners = {
            "DRAW": [],
            "KEYPRESS": [],
            "DRAW_WORLD": [],
            "CAN_MOVE" : [],
            "MOVE": []
        }
    }

    listener(object, eventType) {
        if (_listeners.containsKey(eventType)) {
            _listeners[eventType].add(object)
        }
    }

    queue(type, data) {
        var e = Event.new(type, data)
        queue(e)
    }

    queue(event) {
        if (_listeners.containsKey(event.type)) {
            // broadcast the event...?
            for (l in _listeners[event.type]) {
                l.listen(event)    
            }
        }
    }

    //@@TODO: not sure if this is the best way to do this
    queue_id(id, event) {
        if (_listeners.containsKey(event.type)) {
            for (l in _listeners[event.type]) {
                if (l.id == id) {
                    l.listen(event)
                }    
            }
        }
    }
    
    queue_id(id, type, data) {
        var e = Event.new(type, data)
        queue_id(id, e)
    }
}

// Initialize a singleton to be imported
var Events = EventQueue.new()