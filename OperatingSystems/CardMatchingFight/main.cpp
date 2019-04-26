#include <iostream>
#include <stdio.h>
#include <fstream>
#include <random>
#include <vector>

#define NUM_PLAYERS 3
#define NUM_CARDS 52
#define NUM_ROUNDS 3

using namespace std;

//Thread variables
pthread_mutex_t deck_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t dealer_wait_condition = PTHREAD_COND_INITIALIZER;
pthread_mutex_t dealer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t win_condition = PTHREAD_COND_INITIALIZER;
pthread_mutex_t player_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t player_wait_condition = PTHREAD_COND_INITIALIZER;
pthread_t player_thread[NUM_PLAYERS];
pthread_t dealer_thread;

//Other variables 
ofstream log_file;
bool win, done;
int current_round, turn, seed;
vector<int> card_deck;

pair<int,int> player1_hand,
              player2_hand,
              player3_hand;

//Functions
void buildGame(char *argv[]);
void shuffleDeck();
void buildDeck();
void dealCards();
void *dealerThreadFunction();
void *playerThreadFunction(void * id);
void run();
void playRound();
void useDeck(void * id, pair<int, int> hand);
void printDeck();
void discardPlayerHands();

int main(int argc, char *argv[]) {

	// Open a file
    log_file.open("log.txt");

    // Seed the rand() function with given argument
    srand(atoi(argv[1]));

    // Run the game
    run();

    // Exit
    return 0;
}

// Shuffle the deck using the given seed
void shuffleDeck(){

	log_file << "DEALER: shuffle" << endl;

    for(int i = 0; i < NUM_CARDS; i++){
        int randPos = i + (rand() % (NUM_CARDS - i));
        int temp = card_deck[i];
        card_deck[i] = card_deck[randPos]; 
        card_deck[randPos] = temp;
    }
}

// Create the deck
void buildDeck(){

    int card_value = 1;
    for(int i = 0; i < NUM_CARDS; i++) {
        card_deck.push_back(card_value);
        card_value++;
        if(card_value == 14) {
            card_value = 1;
        }
    }
}

// Print the deck
void printDeck(){

	cout << "DECK: ";
	for (vector<int>::iterator it = card_deck.begin() ; it != card_deck.end(); ++it)
		cout << *it << " ";
	cout << endl << endl;
}

// Dealer thread function
void *dealerThreadFunction(void * id){

	useDeck(id, make_pair(0,0));

	// Tell the player threads that the round is ready to be played
	done = true;
	pthread_cond_broadcast(&player_wait_condition);
	
	// Lock the dealer until a victory
	pthread_mutex_lock(&dealer_mutex);
	// Wait for a victory
	while(win == false)
		pthread_cond_wait(&win_condition, &dealer_mutex);
	// Unlock the dealer to finish 
	pthread_mutex_unlock(&dealer_mutex);


    pthread_exit(NULL);
}

// Player thread function
void *playerThreadFunction(void * id){

	// Lock the players 
	pthread_mutex_lock(&player_mutex);
	// Wait for dealer to be done
	while(done == false)
		pthread_cond_wait(&player_wait_condition, &player_mutex);
	// Unlock the players to play the game
	pthread_mutex_unlock(&player_mutex);

	long player = (long)id;
	pair<int, int> hand = make_pair(0,0);

	// While the round isn't over
	while(win == false){

		// Give the players their appropriate hand
		if(current_round == 0){
			if(player == 1)
				hand = player1_hand;
			else if(player == 2)
				hand = player2_hand;
			else if(player == 3)
				hand = player3_hand;
		}
		else if(current_round == 1){
			if(player == 2)
				hand = player1_hand;
			else if(player == 3)
				hand = player2_hand;
			else if(player == 1)
				hand = player3_hand;
		}
		else if(current_round == 2){
			if(player == 3) 
				hand = player1_hand;
			else if(player == 1)
				hand = player2_hand;
			else if(player == 2)
				hand = player3_hand;
		}

		// Lock the deck
		pthread_mutex_lock(&deck_mutex);

		// If the round isn't over and it is their turn, give the player access to the deck
		if(win == false && player == turn)
			useDeck(id, hand);

		// If the round isn't over and its not their turn, make the player wait
		while(win == false && player != turn)
			pthread_cond_wait(&dealer_wait_condition, &deck_mutex);

		// If the round is over the player tells us round is over
		if(win == true)
			log_file << "PLAYER " << player << ": round completed" << endl;

		// Unlock the deck
		pthread_mutex_unlock(&deck_mutex);
	}

	pthread_exit(NULL);
}

void run() {

	// Set the game variables
    current_round = 0;
    win = false;
    done = false;

    log_file << "Starting Game" << endl;

    // Initialize empty hands
    player1_hand = make_pair(0,0);
    player2_hand = make_pair(0,0);
    player3_hand = make_pair(0,0);

    // Create the deck of cards
    buildDeck();

    // Run the game for the desired number of rounds
    while(current_round < NUM_ROUNDS) {

    	// Set the starting turn (starting player) based on the round
    	if(current_round == 0){
    		turn = 1;
    	}
    	else if(current_round == 1){
    		turn = 2;

    		// Set all hands to empty before the round
    		player1_hand.first = 0;
            player2_hand.first = 0;
            player3_hand.first = 0;

            player1_hand.second = 0;
            player2_hand.second = 0;
            player3_hand.second = 0;
    	}
    	else if(current_round == 2){
    		turn = 3;

    		// Set all hands to empty before the round
    		player1_hand.first = 0;
            player2_hand.first = 0;
            player3_hand.first = 0;

            player1_hand.second = 0;
            player2_hand.second = 0;
            player3_hand.second = 0;
    	}

    	// Play round and increment round counter
        playRound();
        current_round++;

        // Reset values
        win = false;
        done = false;
    }

    card_deck.clear();

    log_file << "Game Over" << endl;

    //close the log file
    log_file.close();
}

// Play a single round of the game
void playRound() {

    cout << endl << "Starting Round " << current_round+1 << endl << endl;
    log_file << endl << "Starting Round " << current_round+1 << endl << endl; 

    // Create the dealer thread and give it the dealer function to run
    pthread_create(&dealer_thread, NULL, dealerThreadFunction, 0);

    // Create the player threads and give them the player function to run
    for(long i = 1; i <= NUM_PLAYERS; i++){
        pthread_create(&player_thread[i], NULL, playerThreadFunction, (void *)i);
    }

    // Join the threads together so they can be synchronized
    pthread_join(dealer_thread, NULL);
    for(long i = 0; i < NUM_PLAYERS; i++){
        pthread_join(player_thread[i], NULL);
    }
}

// Discard the hands back into the deck
void discardPlayerHands(){

		if(player1_hand.first != 0) {
			card_deck.push_back(player1_hand.first);
			player1_hand.first = 0;
		}
		if(player1_hand.second != 0){
			card_deck.push_back(player1_hand.second);
			player1_hand.second = 0;
		}
        if(player2_hand.first != 0) {
			card_deck.push_back(player2_hand.first);
			player2_hand.first = 0;
		}
		if(player2_hand.second != 0){
			card_deck.push_back(player2_hand.second);
			player2_hand.second = 0;
		}

        if(player3_hand.first != 0) {
			card_deck.push_back(player3_hand.first);
			player3_hand.first = 0;
		}
		if(player3_hand.second != 0){
			card_deck.push_back(player3_hand.second);
			player3_hand.second = 0;
		}
}

void dealCards() {
    player1_hand.first = card_deck.front();
    card_deck.erase(card_deck.begin());

    player2_hand.first = card_deck.front();
    card_deck.erase(card_deck.begin());

    player3_hand.first = card_deck.front();
    card_deck.erase(card_deck.begin());
}

void useDeck(void * id, pair<int, int> hand){

	if((long)id == 0){

		shuffleDeck();

		dealCards();
	}

	else{
        long player_id = (long)id;

		log_file << "PLAYER " << player_id << ": hand " << hand.first << endl;

		hand.second = card_deck.front();
		card_deck.erase(card_deck.begin());
		log_file << "PLAYER " << player_id << ": draws " << hand.second << endl;

		log_file << "PLAYER " << player_id << ": hand " << hand.first << " " << hand.second << endl;

		if(hand.first == hand.second){

			if(current_round == 0){
				if(player_id == 1)
					player1_hand = hand;
				else if(player_id == 2)
					player2_hand = hand;
				else if(player_id == 3)
					player3_hand = hand;
			}
			else if(current_round == 1){
				if(player_id == 2)
					player1_hand = hand;
				else if(player_id == 3)
					player2_hand = hand;
				else if(player_id == 1)
					player3_hand = hand;
			}
			else if(current_round == 2){
				if(player_id == 3) 
					player1_hand = hand;
				else if(player_id == 1)
					player2_hand = hand;
				else if(player_id == 2)
					player3_hand = hand;
			}

			log_file << "PLAYER " << player_id << ": wins" << endl << endl;
			cout << "HAND: " << hand.first << " " << hand.second << endl;
			cout << "WIN yes" << endl;

			printDeck();
			discardPlayerHands();

			win = true;

			pthread_cond_signal(&win_condition);
		}
		else if(hand.first != hand.second){

			int discard = rand()%2;
			if(discard == 0){
				log_file << "PLAYER " << player_id << ": discards " << hand.first << endl << endl;
				card_deck.push_back(hand.first);
				hand.first = hand.second;
				hand.second = 0;
			}
			else if(discard == 1){
				log_file << "PLAYER " << player_id << ": discards " << hand.second << endl << endl;
				card_deck.push_back(hand.second);
				hand.second = 0;
			}

			cout << "HAND: " << hand.first << endl;
			cout << "WIN no" << endl;

			if(current_round == 0){
				if(player_id == 1)
					player1_hand = hand;
				else if(player_id == 2)
					player2_hand = hand;
				else if(player_id == 3)
					player3_hand = hand;
			}
			else if(current_round == 1){
				if(player_id == 2)
					player1_hand = hand;
				else if(player_id == 3)
					player2_hand = hand;
				else if(player_id == 1)
					player3_hand = hand;
			}
			else if(current_round == 2){
				if(player_id == 3) 
					player1_hand = hand;
				else if(player_id == 1)
					player2_hand = hand;
				else if(player_id == 2)
					player3_hand = hand;
			}

			printDeck();
		}

		turn++;
		if(turn > NUM_PLAYERS)
			turn = 1;
	}

	pthread_cond_broadcast(&dealer_wait_condition);
}
