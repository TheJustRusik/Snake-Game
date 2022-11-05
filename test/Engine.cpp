#include "Engine.h"

void Engine::showBody(COORD pos) {
    SetConsoleCursorPosition(h, pos);
    SetConsoleTextAttribute(h, 5);
    std::cout << 'P';
}

void Engine::eraseBody(COORD pos) {
    SetConsoleCursorPosition(h, pos);
    std::cout << " ";
}

Engine::Engine() {
    //hide cursor
    CONSOLE_CURSOR_INFO cursor;
    cursor.bVisible = false;
    cursor.dwSize = 1;
    SetConsoleCursorInfo(h, &cursor);
    position.X = 1; position.Y = 1;
    //generate borders snake and apple
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0 or i == (height - 1) or j == 0 or j == (width - 1))
                field[i][j] = WALL;
            else
                field[i][j] = AIR;
        }
    }
    field[1][1] = BODY;
    //field[1][2] = BODY;
    //field[1][3] = BODY;
    int x = 0, y = 0;
    do
    {
        x = (1 + rand() % width) - 1;
        y = (1 + rand() % height) - 1;

    } while (field[y][x] != AIR);
    field[y][x] = APPLE;
}


   

void Engine::print() {
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            switch (field[i][j])
            {
            case AIR:
                SetConsoleTextAttribute(h, 7);
                std::cout << ' ';
                break;
            case WALL:
                SetConsoleTextAttribute(h, 14);
                std::cout << '#';
                break;
            case BODY:
                SetConsoleTextAttribute(h, 2);
                std::cout << '*';
                break;
            case APPLE:
                SetConsoleTextAttribute(h, 4);
                std::cout << 'A';
                break;
            }
        }
        std::cout << std::endl;
    }
}

void Engine::play() {
    std::thread getArrowKey([&]() {
        while (true) {
            if (_kbhit()) {
                int code = _getch();
                if (code == 224) {
                    code = _getch();
                }
                if (code == key::LEFT)
                    direction = LEFT;
                else if (code == key::RIGHT)
                    direction = RIGHT;
                else if (code == key::UP)
                    direction = UP;
                else if (code == key::DOWN)
                    direction = DOWN;
            }
        }
    });

    while (true) {
        
        eraseBody(position);
        
       
        switch (direction) {
        case UP:
            position.Y--;
            break; 
        case DOWN: 
            position.Y++;
            break; 
        case RIGHT: 
            position.X++;
            break; 
        case LEFT: 
            position.X--;
            break; 
        }
        //if (position.Y < 1 or position.Y >= height - 1 or position.X < 1 or position.X >= width - 1) {
        //    std::cout << "Проебал";
        //    break;
        //}
        showBody(position);
        Sleep(timeMS);



        /*
        SetConsoleCursorPosition(h, position);
        SetConsoleTextAttribute(h, 5);
        std::cout << 'P';*/
    }
}