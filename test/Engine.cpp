#include "Engine.h"



Engine::Engine() {
    //hide cursor
    CONSOLE_CURSOR_INFO cursor;
    cursor.bVisible = false;
    cursor.dwSize = 1;
    SetConsoleCursorInfo(h, &cursor);
    position.X = 1; position.Y = 1;
    //generate borders snake and apple
    for (int i = 0, k = 2; i < height; i++) {
        for (int j = 0; j < width; j++, k++) {
            if (i == 0 or i == (height - 1) or j == 0 or j == (width - 1))
                if(k % 2 == 0)
                    field[i][j] = WALL;
                else
                    field[i][j] = WALL1;
            else
                field[i][j] = AIR;
        }
        k++;
    }
    
    body.push_back({ 3,1 });
    body.push_back({ 2,1 });
    body.push_back({ 1,1 });
    field[1][3] = BODY;
    field[1][2] = BODY;
    field[1][1] = BODY;
    position = { 3,1 };
}


   
//draw game map and first apple (once at the beginning)
void Engine::print() {
    SetConsoleCursorPosition(h, { 0,0 });
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            switch (field[i][j])
            {
            case AIR:
                SetConsoleTextAttribute(h, 8);
                std::cout << ' ';
                break;
            case WALL:
                SetConsoleTextAttribute(h, 15);
                std::cout << '#';
                break;
            case WALL1:
                SetConsoleTextAttribute(h, 10);
                std::cout << '#';
                break;
            case BODY:
                SetConsoleTextAttribute(h, 11);
                std::cout << '*';
                break;
            case APPLE:
                SetConsoleTextAttribute(h, 12);
                std::cout << 'A';
                break;
            }
        }
        std::cout << std::endl;
    }
}

void Engine::play() {
    //check keystrokes (independent of everything else)
    std::thread thread1([&]() {
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

    //main game cycle
    std::thread thread2([&]() {
        unsigned long long int i = 0;
        short apples = 0;
        while (true) {
            //debug information
            SetConsoleCursorPosition(h, { 25,0 });
            SetConsoleTextAttribute(h, 15);
            std::cout << "Debug information";
            SetConsoleCursorPosition(h, { 25,1 });
            SetConsoleTextAttribute(h, 12);
            std::cout << "direction: "<<direction;
            SetConsoleCursorPosition(h, { 25,2 });
            std::cout << "main cycle iteration: " << i;
            SetConsoleCursorPosition(h, { 25,3 });
            std::cout << "Position: x: " << position.X << " y: " << position.Y << "             ";
            SetConsoleCursorPosition(h, { 25,4 });
            std::cout << "Eaten apples: " << apples;

            //remove player from old position
            SetConsoleCursorPosition(h, body[body.size() - 1]);
            SetConsoleTextAttribute(h, 11);
            std::cout << ' ';

            //do move
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
            //give new cords for all parts of players body
            for (int i = body.size() - 1; i > 0; i--)
            {
                body[i] = body[i-1];
            }
            body[0] = position;

            //check for colision with borders
            if (position.X == 0 or position.X == (width - 1) or position.Y == 0 or position.Y == (height - 1)) {
                SetConsoleCursorPosition(h, { 0,height });
                SetConsoleTextAttribute(h, 4);
                std::cout << "YOU LOST";
                exit(0);
            }

            //check for colision with apple
            if (field[position.X][position.Y] == APPLE) {
                apples++;
            }

            //draw player on new position
            SetConsoleCursorPosition(h, body[0]);
            SetConsoleTextAttribute(h, 11);
            std::cout << '*';

            
            i++;
            Sleep(timeMS);
            //MessageBoxA(0, "Next step?", "GO!", 0);
        }
    });
    thread1.join();
    thread2.join();
    
}