#include <iostream>
#include <pthread.h>
#include <fstream>
#include <vector>

using namespace std;

const int NUM_PLAYERS = 3;
const int NUM_CARDS = 52;

typedef struct Dealer {
    pthread_t dealer_thread;
} Dealer;

typedef struct Card {
    int value;
    int suit;
} Cards;

typedef pair<Cards,Cards> Hand;

typedef struct Player {
    pthread_t player_thread[NUM_PLAYERS];
    Hand hand;
} Player;

class Game {
    private:
        ofstream log_file;

        bool win;

        int num_rounds,
            current_round,
            whos_turn;

        Player player[NUM_PLAYERS];

        Dealer dealer;
        vector<Cards> card_deck;

    public:
        Game(int seed);

        void run();

        void playRound();
};