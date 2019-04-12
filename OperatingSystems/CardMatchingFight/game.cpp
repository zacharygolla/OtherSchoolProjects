#include <iostream>
#include <stdio.h>
#include <fstream>

#include "game.h"

#define NUM_ROUNDS 3

/*
The game constructor will:
    Open the log file
    Srand the seed
    Initialize the Deck with all card values and which suit they belong to
        Card values will be 0-12 {0:2, 1:3, 2:4, 3:5, 4:6, 5:7, 6:8, 7:9, 8:10, 9:Jack, 10:Queen, 11:King, 12:Ace}
        Suit Values will be 0-4 {0:Hearts, 1:Diamonds, 2:Spades, 3:Clubs}
    Set the current round to 0
    Set the win value to false
*/
Game::Game(int seed) {
    //open the log_file file
    log_file.open("log.txt");
    
    //check file opened
    if(log_file.is_open())
        cout << "Opened the Log file" << endl;
    else
        cout << "Log file did not open" << endl;

    //srand the seed
    srand(seed);
    cout << "Completed srand on the seed" << endl;

    //initialize the deck
    int card_value = 0;
    int card_suit = 0;

    for(int i = 0; i < NUM_CARDS; i++) {
        Card temp_card;
        temp_card.value = card_value;
        temp_card.suit = card_suit;
        card_deck.push_back(temp_card);
        card_value++;
        if(card_value == 13) {
            card_value = 0;
            card_suit++;
        }
    }
    cout << "Deck initialized" << endl;

    //set the current round to 0
    current_round = 0;

    //set the win to false
    win = false;
}

void Game::run() {
    while(current_round < NUM_ROUNDS) {
        playRound();
        current_round++;
        win = false;
    }

    //close the log_file file
    log_file.close();
    cout << "Log file closed" << endl;
    
    cout << "Game Ran" << endl;
}

void Game::playRound() {
    cout << "Starting Round" << endl;
    log_file << "Starting Round:" << endl; 
}

