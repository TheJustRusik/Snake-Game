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

    //generate first apple
    field[height / 2][width / 2] = APPLE;

    print();
}

Engine::~Engine() {

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

bool Engine::isInBody(short X, short Y) {
    for (auto i : body) 
        if (i.Y == Y and i.X == X)
            return true;
    return false;
}

void Engine::printLine(std::string str, COORD pos, int color = -1) {
    SetConsoleCursorPosition(h, pos);
    if (color == -1) {
        int j = 2;
        for (auto i : str) {
            SetConsoleTextAttribute(h, j);
            std::cout << i;
            if (i != ' ')
                j++;
            if (j > 15)
                j = 2;
        }
    }
    else {
        SetConsoleTextAttribute(h, color);
        for (auto i : str)
            std::cout << i;
    }
}

void Engine::start() {
    printLine("SNAKE GAME by TheJustRusik", {23,0});
    printLine("CHOOSE GAME DIFFICULTY:", {23, 1}, 15);
    printLine("EASY - 1", { 23, 2 }, 10);
    printLine("NORMAL - 2", { 23, 3 }, 14);
    Sleep(100);
    printLine("HARD - 3", { 23, 4 }, 12);
RES:
    Sleep(100);
    printLine(">> ", { 23, 5 }, 15);
    Sleep(100);
    int a;
    try {
        std::cin >> a;
        if (a < 1 or a > 3)
            throw -1;
    }
    catch (...) {
        printLine("Invalid input (try 1, 2 or 3)", {23, 5}, 4);
        Sleep(3000);
        printLine("                             ", { 23, 5 });
        goto RES;
    }

    printLine("GAME DIFFICULTY:                  ", { 23, 1 }, 15);
    switch (a)
    {
    case 1:
        printLine("EASY", { 40, 1 }, 10);
        timeMS = 300;
        break;
    case 2:
        printLine("NORMAL", { 40, 1 }, 14);
        timeMS = 200;
        break;
    case 3:
        printLine("HARD", { 40, 1 }, 12);
        timeMS = 100;
        break;
    }
   
    printLine("                         ", { 23, 2 }, 10);
    printLine("                         ", { 23, 3 }, 14);
    printLine("                         ", { 23, 4 }, 12);
    printLine("                         ", { 23, 5 }, 15);
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
                if (code == key::LEFT and direction != RIGHT) {
                    direction = LEFT;
                    Sleep(timeMS);
                }
                else if (code == key::RIGHT and direction != LEFT) {
                    direction = RIGHT;
                    Sleep(timeMS);
                }
                else if (code == key::UP and direction != DOWN) {
                    direction = UP;
                    Sleep(timeMS);
                }
                else if (code == key::DOWN and direction != UP) {
                    direction = DOWN;
                    Sleep(timeMS);
                }
            }
        }
    });

    //main game cycle
    std::thread thread2([&]() {
        unsigned long long int i = 0;
        short apples = 0;
        short appleX = width / 2, appleY = height / 2;
        COORD oldPos = body[body.size() - 1];
        while (true) {
            //debug information
            /*
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
            SetConsoleCursorPosition(h, { 25,5 });
            std::cout << "Apple pos: x: "<< appleX<<" y: "<<appleY<<"                 ";
            */

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
                
                
                
                system("pause");
                exit(0);
                
            }

            //check for colision with body
            for (int i = 1; i < body.size(); i++) {
                if (body[0].Y == body[i].Y and body[0].X == body[i].X) {
                    SetConsoleCursorPosition(h, { 0,height });
                    SetConsoleTextAttribute(h, 4);
                    

                    
                    system("pause");
                    exit(0);
                    
                }
            }

            

            //check for colision with apple
            if (body[0].Y == appleY and body[0].X == appleX) {
                field[appleY][appleX] = AIR;
                apples++;

                body.push_back(oldPos);


                //generate new apple and draw it
                LOL:
                appleX = 1 + ((rand() % width) - 1), appleY = 1 + ((rand() % height) - 1);
                while (field[appleY][appleX] != AIR or isInBody(appleX, appleY)) {
                    for (auto i : body) {
                        if (appleX == i.X and appleY == i.Y)
                            goto LOL;
                    }
                    appleX = 1 + ((rand() % width) - 1);
                    appleY = 1 + ((rand() % height) - 1);
                }
                SetConsoleCursorPosition(h, { appleX,appleY });
                SetConsoleTextAttribute(h, 12);
                std::cout << 'A';

            }

            //draw player on new position
            SetConsoleCursorPosition(h, body[0]);
            SetConsoleTextAttribute(h, 11);
            std::cout << '*';
            
            i++;
            Sleep(timeMS);
            
        }
    });
    thread1.join();
    thread2.join();
    
}

void Engine::loose() {
    printLine("You loose", { 23, 3 }, 12);
    printLine("Want play agayn? (yes = y/no = n):", { 23, 4 }, 14);
TB2:
    printLine("(y/n) >> ", { 23, 5 }, 15);
    std::string a = "";
        std::cin >> a;
        if (a == "y" or a == "1" or a == "yes") {
            loosed = 1;
        }
        else if (a == "n" or a == "0" or a == "no") {
            loosed = 0;
        }
        else{
            printLine("(y/n) >> ERROR INPUT TRY y OR n !!!", { 23, 5 }, 12);
            Sleep(3000);
            printLine("                                   ", { 23,5 });
            goto TB2;
        }
            
        
    
        
    
    
}

