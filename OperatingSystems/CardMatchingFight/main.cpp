#include <iostream>
#include <pthread.h>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

//global variables
const int NUM_ROUNDS = 3;
const int NUM_PLAYERS = 3;
const int NUM_CARDS = 52;

int current_round = 0,
    whos_turn = 1;
bool win = false;

vector<int> card_deck;
ofstream log_file;

pair<int,int> player1_hand, 
              player2_hand, 
              player3_hand;

//Prototypes for regular functions
void initializeGame(int);
void buildDeck();
void printDeck();
void runGame();
void playRound();
void shuffleDeck();
void dealCards();
void useDeck(long, pair<int,int>);
void putPlayersCardsBack();

//Thread Variables
pthread_mutex_t mutex_useDeck = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex_dealerExit = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_win = PTHREAD_COND_INITIALIZER;
pthread_t player_thread[NUM_PLAYERS];
pthread_t dealer_thread;

//Prototypes for thread functions
void * dealerFunction(void * arg);
void * playerFunction(void * id);


int main(int argc, char *argv[]) {
    //open the log_file file
    log_file.open("log.txt");
    
    //check file opened
    if(log_file.is_open())
        cout << "Opened the Log file" << endl;
    else
        cout << "Log file did not open" << endl;
    //initialize the game
    srand(atoi(argv[1]));

    //run the game
    runGame();

    cout << "Game finished running" << endl;
    log_file.close();
    cout << "Log File Close" << endl;
    exit(EXIT_SUCCESS);
}

void buildDeck() {
    //initialize the deck
    int card_value = 0;
    card_deck.erase(card_deck.begin(), card_deck.end());
    for(int i = 0; i < NUM_CARDS; i++) {
        int temp_card;
        temp_card = card_value;
        card_deck.push_back(temp_card);
        card_value++;
        if(card_value == 13) {
            card_value = 0;
        }
    }
    cout << "Deck initialized" << endl;
}

void runGame() {
    buildDeck();

    while(current_round < NUM_ROUNDS) {
        playRound();
        current_round++;
        win = false;
        putPlayersCardsBack();
    }
    
    cout << "Game Ran" << endl;
}

void playRound() {
    int success_check;
    cout << "Starting Round" << endl;
    log_file << "Starting Round:" << endl; 
    cout << "Round " << current_round + 1 << ":" << endl;

    //create dealer thread
    cout << "Creating Dealer Thread" << endl;
    success_check = pthread_create(&dealer_thread, NULL, &dealerFunction, NULL);
    if(success_check != 0) {
        cout << "Something is wrong" << endl;
    }

    //create player thread
    cout << "Creating Player Threads" << endl;
    for(int i = 0; i < NUM_PLAYERS; i ++) {
        success_check = pthread_create(&player_thread[i], NULL, &playerFunction, (void *) (i + 1));
        if(success_check != 0) {
            cout << "Something is wrong" << endl;
        }
    }
 
    pthread_join(dealer_thread, NULL);
    for(int i = 0; i < NUM_PLAYERS; i++) {
        pthread_join(player_thread[i], NULL);
    }
}

void printDeck() {
    for(int i = 0; i < card_deck.size(); i++) {
        cout << card_deck[i] << " ";
    }
    cout << endl;
}

void * dealerFunction(void * arg) {
    long id = 0;       // identify the dealer as player 0
    whos_turn = 0;      
    pair<int, int> dealerHand;    // dealer gets a NULL hand
    useDeck(id, dealerHand); // let the dealer use the deck
   
    // leave the dealer thread
    pthread_mutex_lock(&mutex_dealerExit);  // lock the exit ............
    while(!win) {
        pthread_cond_wait(&cond_win, &mutex_dealerExit);
    }      
    pthread_mutex_unlock(&mutex_dealerExit); // unlock the exit .........
    log_file << "Dealer exits round" << endl;
    pthread_exit(NULL);
}

void * playerFunction(void * id) {
    long player_id = (long)id;
   
   // assign hands to players based on which round is being played   
    pair<int,int> new_hand;
    switch (current_round) {
        case 0:
            if( player_id == 1 ) 
                new_hand = player1_hand; 
            else if( player_id == 2 ) 
                new_hand = player2_hand;
            else 
                new_hand = player3_hand;
            break;
        case 1:
            if( player_id == 1 ) 
                new_hand = player1_hand; 
            else if( player_id == 2 ) 
                new_hand = player2_hand;
            else 
                new_hand = player3_hand;
            break;
        case 2:
            if( player_id == 1 ) 
                new_hand = player1_hand; 
            else if( player_id == 2 ) 
                new_hand = player2_hand;
            else 
                new_hand = player3_hand;
            break;
        default:
            cout << "There is an incorrect number of players playing" << endl;
            break;
    }
   
    while( win == 0 ) {
        pthread_mutex_lock(&mutex_useDeck); // lock the deck ...............  
        while( player_id != whos_turn && win == 0 ) { // make players wait for their turn
            pthread_cond_wait(&condition_var, &mutex_useDeck); 
        }
      
        if( win == 0 ) {   
         useDeck(player_id, new_hand); // let players use the deck
        }          
        pthread_mutex_unlock(&mutex_useDeck); // unlock the deck ...........
    }
   
    // leave the player thread
    log_file << "Player exits round" << endl;
    pthread_exit(NULL); 
}

void useDeck(long id, pair<int,int> hand) {
    if(id == 0) {
        //shuffle the deck
        shuffleDeck();
        printDeck();
        dealCards();

        //deal the cards
    }
    else {
        // show hand
        log_file << "Player " << id << ": hand " << hand.first << endl;
        // draw a card
        hand.second = card_deck.back(); 
        card_deck.pop_back();
        log_file << "Player " << id << ": hand " << hand.second << endl;

      // show hand     
        log_file << "Hand: " << hand.first << " " << hand.second << endl;
        log_file << "Player " << id << ": hand " << hand.first << " " << hand.second << endl;

      // compare the cards
        if( hand.first == hand.second ) {
            // if the cards match, then declare a winner            
            printf("WIN yes\n");      
            log_file << "Player " << id << ": Wins!" << endl;
            win = true;   
            pthread_cond_signal(&cond_win); // signal dealer to exit           
        }
        else {
            // randomly select discard and put it back in the deck
            int discard = rand() % 2;
            if( discard == 0 ) {
                log_file << "Player " << id << ": discards " << hand.first << endl;
                card_deck.insert(card_deck.begin(), hand.first);  // put card back in deck
                hand.first = hand.second; // set card1 to remaining card value
            }     
            else {
                log_file << "Player " << id << ": discards " << hand.second << endl;
                card_deck.insert(card_deck.begin(), hand.second);
            }   

         // print the contents of the deck
        printDeck();                 
        }      
    }  
    whos_turn++; // inc turn so next player may use the deck
    if( whos_turn > NUM_PLAYERS) 
        whos_turn = 1;      // if player3 went, move to player1
    pthread_cond_broadcast(&condition_var); // broadcast that deck is available
}

void shuffleDeck() { // dealer swaps current card w/ rand card til all shuffled
    for( int i = 0; i < (card_deck.size() - 1); i++ ){
        int randPos = i + (rand() % (card_deck.size() - i));  
        int temp = card_deck[i];
        card_deck[i] = card_deck[randPos]; 
        card_deck[randPos] = temp;
    }
} // end function


void dealCards() { // the dealer deals one card into each hand
    player1_hand.first = card_deck.back();
    card_deck.pop_back();
    player2_hand.first = card_deck.back();
    card_deck.pop_back();
    player3_hand.first = card_deck.back();
    card_deck.pop_back();         
} // end function

void putPlayersCardsBack() {
    card_deck.push_back(player1_hand.first);
    card_deck.push_back(player2_hand.first);
    card_deck.push_back(player3_hand.first);
}