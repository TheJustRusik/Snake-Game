#include "Engine.h"



int main(){
    srand(time(NULL));
    setlocale(LC_ALL, "");

    Engine game;
    game.print();
    game.play();

}


