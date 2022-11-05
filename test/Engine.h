#pragma once

#include <thread>
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <stdlib.h> 
#include <vector>


const int width = 60;
const int height = 20;
const unsigned short timeMS = 150;


namespace key {
    enum Code {
        ENTER = 13,
        ESCAPE = 27,
        SPACE = 32,
        LEFT = 75,
        RIGHT = 77,
        UP = 72,
        DOWN = 80
    };
}
enum Direction {
    UP,
    DOWN,
    RIGHT,
    LEFT
};
enum objects {
    AIR,
    WALL,
    BODY,
    APPLE
};
enum color {
    DARKGREEN = 2,
    YELLOW = 14,
    RED = 12,
    BLUE = 9,
    WHITE = 15,
    DARKYELLOW = 6,
    DARKRED = 4
};

class Engine {
private:
    objects field[height][width];
    int length = 3;
    Direction direction = LEFT;
    COORD position;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    std::vector<COORD> body;

    void showBody(COORD pos);
    void eraseBody(COORD pos);

public:
    Engine();
    void print();
    void play();
    
};
