#include "Engine.h"

int main() {
    srand(time(NULL));
    setlocale(LC_ALL, "");

    Engine *game = new Engine();
    game->start();
    game->play();
}
