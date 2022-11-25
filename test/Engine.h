#pragma once

#include <thread>
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <stdlib.h> 
#include <vector>
#include <string>


const int width = 20;
const int height = 10;
const unsigned short timeMS = 200;


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
    WALL1,
    BODY,
    APPLE
};


class Engine {
private:
    objects field[height][width];
    Direction direction = RIGHT;
    COORD position;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    std::vector<COORD> body;

    bool isInBody(short X, short Y);
    

public:
    Engine();
    void print();
    void play();
    
};
