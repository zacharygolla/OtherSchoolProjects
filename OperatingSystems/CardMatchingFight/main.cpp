#include <iostream>
#include "game.h"

using namespace std;

int main(int argc, char *argv[]) {
    Game game(atoi(argv[1]));

    game.run();
    return 0;
}