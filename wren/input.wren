class Input {
    static isMovement(key) {
        return isMoveUp(key) || isMoveDown(key) || isMoveLeft(key) || isMoveRight(key)
    }

    static isMoveUp(key) {
        return key == "UP"
    }

    static isMoveLeft(key) {
        return key == "LEFT"
    }

    static isMoveRight(key) {
        return key == "RIGHT"
    }

    static isMoveDown(key) {
        return key == "DOWN"
    }
}