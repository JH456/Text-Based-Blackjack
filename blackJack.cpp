/**
* Author: Jim Harris
* Title: ASCII BlackJack
* Description: A BlackJack game with ASCII graphics. Monospace font and single line spacing recommended for optimal appearance.
* Date: 3/4/15
**/

//Include additional libraries.
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//Use the standard namespace for convenience sake.
using namespace std;

//Constant arrays used when drawing the cards.
const char *vals[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
const char *suits[] = {"♠", "♥", "♦", "♣"};

//Struct prototypes for the Card and Hand struct.
struct Card;
struct Hand;

//Player class allows for some additional organization.
class Player;

//Function prototypes.
Card* dealCard(Hand* deck);
void printCards(Hand* hand, int n, int segmentNumber);
void deleteCard(Card* card);
void deleteHand(Hand* hand);
void fillDeck(Hand* deck);
bool splittable(Hand* hand);
int sumHand(Hand* hand);
string getDecision(string messagePrefix);
void play();

//Card struct is a linked list node. This way, the number of cards a player has can expand or contract.
struct Card {
	short suit;
	short val;
	short faceVal;
	Card* next;
};

//Hand struct is a linked list node. Each hand has the root node for a card linked list. This makes it possible to have multiple hands so a player can split their hand.
struct Hand {
	Card* root;
	Hand* next;
};

class Player {
public:
	Player() {
		hands = new Hand();
	}
	//The current hand that the player is using is the last node in the hands linked list.
	Hand* getCurrentHand() {
		Hand* hand = this->hands;
		while (hand->next != NULL) hand = hand->next;
		return hand;
	}
	//Gets the hand before the current hand (i.e the new one if the current hand is about to be deleted).
	Hand* getNewHand() {
		Hand* hand = this->hands;
		if (hand->next == NULL) return hand;
		while (hand->next->next != NULL) hand = hand->next;
		return hand;
	}
	Hand* getRootHand() {
		return hands;
	}
	//Gets input from the player.
	string getMove(string message = "Enter your move! (hit, split, hold, or fold): ") {
		cout << message;
		string move = "";
		cin >> move;
		return (move == "hit" || move == "hold" || move == "fold") ? move : (move == "split") ? splittable(this->getCurrentHand()) ? move : getMove("Invalid Move! Cannot split this hand! (hit, hold, or fold) : ") : getMove("Invalid Move! (hit, split, hold, or fold): ");
	}
	//Adds a card to the current hand. Assumes card has already been removed from a deck, and therefore has no child node.
	void addCard(Card* card) {
		Hand* hand = getCurrentHand();
		if (hand->root == NULL) {
			hand->root = new Card();
			hand->root->val = card->val;
			hand->root->faceVal = card->faceVal;
			hand->root->suit = card->suit;
		}
		else {
			Card* lastHandCard = hand->root;
			while (lastHandCard->next != NULL) lastHandCard = lastHandCard->next;
			lastHandCard->next = new Card();
			lastHandCard->next->val = card->val;
			lastHandCard->next->faceVal = card->faceVal;
			lastHandCard->next->suit = card->suit;
		}
		delete card;
	}
	//Splits the player's hand by removing the duplicate card from the current hand and adding it to the next hand node in the linked list.
	void splitCurrentHand() {
		Hand* hand = this->getCurrentHand();
		if (!splittable(hand)) return;
		hand->next = new Hand();
		//Safe to assume that during a successful split, the card going into the new hand is at the end of a linked list.
		hand->next->root = hand->root->next;
		hand->root->next = NULL;
	}
	//Deletes linked lists to prevent memory leaks.
	~Player() {
		deleteHand(hands);
	}
private:
	Hand* hands;
};

//Deletes a linked list of cards.
void deleteCard(Card* card) {
	if (card->next != NULL) deleteCard(card->next);
	delete card;
}
//Deletes a linked list of hands.
void deleteHand(Hand* hand) {
	if (hand->root != NULL) deleteCard(hand->root);
	if (hand->next != NULL) deleteHand(hand->next);
	delete hand;
}

//Pops a random card from a deck. Just make sure to save this card and delete it later.
Card* dealCard(Hand* deck) {
	Card* card = deck->root;
	int c = 0;
	//Count the number of cards in the deck.
	while (card != NULL) {
		c++;
		card = card->next;
	}
	if (c == 0) return NULL;
	//Select a random card and pop it from the deck.
	int n = rand() % c;
	Card* cardBefore = deck->root;
	if (n == 0) {
		card = deck->root;
		deck->root = deck->root->next;
	}
	else {
		for (int i = 1; i < n; i++) cardBefore = cardBefore->next;
		card = cardBefore->next;
		cardBefore->next = card->next;
	}
	card->next = NULL;
	return card;
}

//Prints out up to n cards in a hand. Leave segment number empty in calls. Also prints out some borders for the game table.
void printCards(Hand* hand, int n = 52, int segmentNumber = 0) {
	if (segmentNumber >= 5) return;

	Card* card = hand->root;

	int i = 0;

	cout << "║ ";
	
	while (card != NULL && i < n) {
		switch (segmentNumber) {
			case 0:
				cout << "╔═══╗";
				break;
			case 1:
				cout << "║" << suits[card->suit] << (strlen(vals[card->faceVal]) < 2 ? " " : "") << vals[card->faceVal] << "║";
				break;
			case 2:
				cout << "║   ║";
				break;
			case 3:
				cout << "║" << vals[card->faceVal] << (strlen(vals[card->faceVal]) < 2 ? " " : "") << suits[card->suit] << "║";
				break;
			default:
				cout << "╚═══╝";
		}
		card = card->next; i++;
	}

	for (int j = 0; j < 60 - (5 * i); j++) cout << " ";

	cout << " ╠╣";

	cout << endl;

	//Recursive call to print out next line.
	printCards(hand, n, segmentNumber + 1);
}

//Given an empty hand, populates the hand with a whole french deck of cards.
void fillDeck(Hand* deck) {
	if (deck->root != NULL) return;
	deck->root = new Card();
	Card* card = deck->root;
	
	//Just does one suit for now.
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 13; j++) {
			card->suit = i;
			card->val = (j == 0) ? 11 : min(j + 1, 10);
			card->faceVal = j;
			if (!(i == 3 && j == 12)) {
				card->next = new Card();
				card = card->next;	
			}
		}
	}
}

//Returns whether or not the given hand can be split.
bool splittable(Hand* hand) {
	return (hand->root != NULL && hand->root->next != NULL && hand->root->faceVal == hand->root->next->faceVal && hand->root->next->next == NULL);
}

//Calculates the numerical value of a hand. Takes into account the dual values of aces.
int sumHand(Hand* hand) {
	Card* card = hand->root;
	int sum = 0;
	while (card != NULL) {
		sum += card->val;
		card = card->next;
	}
	//Changes the values of aces from 11 to 1 as neccessary.
	if (sum > 21) {
		card = hand->root;
		while (card != NULL && sum > 21) {
			if (card->faceVal == 0) sum -= 10;
			card = card->next;
		}
	}
	return sum;
}

//Gets the user's decision about whether or not to play another game after the termination of one.
string getDecision(string messagePrefix = "") {
	string decision = "";
	cout << messagePrefix << "Would you like to play again? (y/n): ";
	cin >> decision;
	return (decision == "y" || decision == "n") ? decision : getDecision("Invalid decision! ");
}

//Plays a game of BlackJack.
void play() {
	//Create deck.
	Hand* deck = new Hand();
	fillDeck(deck);

	//Create player and dealer, and deal them each two cards.
	Player player = Player();
	player.addCard(dealCard(deck));
	player.addCard(dealCard(deck));

	Player dealer = Player();
	dealer.addCard(dealCard(deck));
	dealer.addCard(dealCard(deck));

	//Whether or not the game is still going on.
	bool alive = true;
	//If the user won; 1 is victory, -1 is defeat by the dealer, 0 is a push.
	short won = -1;
	while (alive) {
		//Clear screen somehow.
		printf("\033[2J");
    	printf("\033[%d;%dH", 0, 0);

		Hand* playerHand = player.getCurrentHand();
		Hand* dealerHand = dealer.getCurrentHand();

		int playerSum = sumHand(playerHand);
		int dealerSum = sumHand(dealerHand);

		//Prints out cards, game table, and numerical values of hands.
		cout << "╔══════════════════════════════════════════════════════════════╦╗" << endl;
		printCards(dealer.getCurrentHand());
		cout << "╠══════════════════════════════════════════════════════════════╬╣" << endl;
		printCards(player.getCurrentHand());
		cout << "╠╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╬╣" << endl;
		cout << "╚╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╝" << endl;

		cout << "The dealer's sum is " << dealerSum << endl << endl;
		cout << "Your sum is " << playerSum << endl << endl;

		//Either switches the player to their next hand or terminates the game if he/she busts.
		if(playerSum > 21) {
			if (playerHand == player.getRootHand()) alive = false;
			else {
				Hand* hand = playerHand;
				Hand* newHand = player.getNewHand();
				newHand->next = NULL;
				deleteHand(hand);
			}
		}
		//Terminates the game if the dealer wins.
		else if (dealerSum == 21) alive = false;
		//Terminates the game with victory if the dealer busts or the player wins.
		else if (dealerSum > 21 || playerSum == 21) {
			won = 1;
			alive = false;
		}
		//If the game does not terminate (first case in if-else chain does not always terminate game), queries the user for a decision.
		if (alive) {
			string move = player.getMove();
			if (move == "hit") {
				player.addCard(dealCard(deck));
			}
			else if (move == "split") {
				player.splitCurrentHand();
			}
			else if (move == "fold") {
				alive = false;
			}
			//Update player information.
			playerHand = player.getCurrentHand();
			playerSum = sumHand(playerHand);
			//Give the dealer a card if he wants one.
			if ((playerSum > dealerSum && playerSum < 21) || dealerSum < 17) dealer.addCard(dealCard(deck));
			//Terminates the game in a push if the user decides to hold when the dealer refuses to draw more cards.
			else if (playerSum >= 17 && playerSum == dealerSum && move == "hold") {
				won = 0;
				alive = false;
			}
		}
	}
	cout << (won == 1 ? "Congratulations! You won!" : (won == 0 ? "Push." : "Sorry, you lost!")) << endl;
	deleteHand(deck);
}

//Entry point for program
int main(int argc, char const *argv[]) {
	
	srand(time(NULL));

	bool playing = true;

	while(playing) {
		play();
		playing = (getDecision() == "y");
	}

	return 0;
}
