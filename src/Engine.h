#pragma once

#include <conio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <thread>
#include <vector>
#include <windows.h>

const int width = 20;
const int height = 10;

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
enum Direction { UP, DOWN, RIGHT, LEFT };
enum objects { AIR, WALL, WALL1, BODY, APPLE };

class Engine {
  private:
    short unsigned timeMS = 100;

    int loosed = -1;

    objects field[height][width];
    Direction direction = RIGHT;
    COORD position;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    std::vector<COORD> body;

    bool isInBody(short X, short Y);
    void print();
    void printLine(std::string str, COORD pos, int color);
    void loose();

  public:
    Engine();
    ~Engine();
    void start();
    void play();
};
