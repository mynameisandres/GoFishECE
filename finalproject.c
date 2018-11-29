/*
Authors: Jonathan Goh and Andres Barragan
Date Created: 11/27/2018
Description of the Program: Plays a game of Go Fish
*/

/*Pseudocode outline of main steps
1)Read cards in from file
2)Shuffle cards
3)Deal cards to players
4)While game not over, take turns asking players which card they want to take and from whom
	a)If a card is successfully taken from someone, check for a match
	b)If no card is taken, draw from deck and check if it is the desired card
		i)If not the desired card, move to next player's turn
5)Once game is over, check who has the most points
*/

#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//To be able to prints the symbols
#define SPADE   "S_"
#define CLUB    "C_"
#define HEART   "H_"
#define DIAMOND "D_"

//adds structure for card
typedef struct card_s
{
	char suit[9];
	char value[3];

	struct card_s *next;
	struct card_s *previous;

} card;

//Structure for a deck
typedef struct deck_s {

	//Deck size
	int deckSize;

	//declares pointers
	card *headl;
	card *headr;

} deck;

//A structure for a player
typedef struct player_s {

	//stores player name
	char name[50];

	//Beginning and end of user hand
	card *headl;
	card *headr;

	//User match points
	int points;

} player;

//--------------------------------------------------------------------------------------------------------DECK STUFF

//defines function that adds members to list
void addCard(card *p, card **hl, card **hr)
{
	//If node is empty
	if (*hr == NULL) {
		*hl = p; //left head points to node
		*hr = p; //right head points to node
		return;
	}//Adds to the end of the doubly linked list
	else {
		(*hr)->next = p; //the last item now points to new node
		p->previous = *hr; //the new node points to the last item
		*hr = p; //the last item is now the new node
	}
}

//Swaps values in a linked list
void swap(card *pt, int i, int j) {

	//Grabs the first node
	card *index1 = pt;
	for (int iter = 0; iter < i; iter++) //loops through nodes until the right one
		index1 = index1->next;

	//Grabs the second node
	card *index2 = pt;
	for (int iter = 0; iter < j; iter++) //loops through nodes until the right one
		index2 = index2->next;

	//puts first node's contents in temp
	char tempSuit[9];
	char tempValue[3];
	strcpy(tempSuit, index1->suit);
	strcpy(tempValue, index1->value);

	//puts second node's contents in first node
	strcpy(index1->suit, index2->suit);
	strcpy(index1->value, index2->value);

	//puts temp's contents into second node
	strcpy(index2->suit, tempSuit);
	strcpy(index2->value, tempValue);
}

//defines function that obtains a random number
int rand_gen(int count)
{
	double frac;
	frac = (double)rand() / ((double)RAND_MAX + 1);
	return (int)floor(count*frac);
}

//Swaps 10000 randoms cards in the deck to shuffle
void shuffle(deck *myDeck) {

	//Randomly swaps the values in the double linked list as many times as user wants
	int first, second;
	for (int i = 0; i < 10000; i++) {
		//gets two random indexes
		first = rand_gen(myDeck->deckSize);
		second = rand_gen(myDeck->deckSize);

		//swaps the cards at the two indexes
		swap(myDeck->headl, first, second);
	}

}

//Initializes a deck
void initDeck(deck *myDeck) {

	myDeck->deckSize = 52;
	myDeck->headl = NULL; //no cards
	myDeck->headr = NULL; //no cards

}

//Initializes player
void initPlayer(player *user, char *playerName) {

	//Initializes the player
	strcpy(user->name, playerName); //assigns their name
	user->headl = NULL; //no cards
	user->headr = NULL; //no cards
	user->points = 0; //no points

}

//Prints all the cards in a linked list
void printCards(card *cards) {

	card *indexer = cards;
	while (indexer != NULL) { //loops until no card

		//Checks what suit to print
		if (strcmp(indexer->suit, "diamonds") == 0)
			printf("%s", DIAMOND);
		else if (strcmp(indexer->suit, "clubs") == 0)
			printf("%s", CLUB);
		else if (strcmp(indexer->suit, "spades") == 0)
			printf("%s", SPADE);
		else
			printf("%s", HEART);

		//Prints the card value next to suit
		printf("%s ", indexer->value);

		//Goes to next card in linked list
		indexer = indexer->next;
	}
	printf("\n");
}

//Removes a card from the end of a deck
card *removeCard(deck *myDeck) {

	//Removes and grabs card from deck
	card *endCard = myDeck->headr; //obtains the last card in the deck
	myDeck->headr->previous->next = NULL; //the second-to-last card points to null
	myDeck->headr = myDeck->headr->previous; //the last card is the second-to-last card
	endCard->previous = NULL; // the card that was taken now has no card before it

	//Decrements decksize
	myDeck->deckSize--;

	//Returns grabbed card
	return endCard;
}

//Draws a specified number of cards from a deck and puts them in a player's hand
void getCards(player *user, deck *myDeck, int amount) {

	card *cardHolder = NULL; //represents the card drawn from the deck
	for (int i = 0; i < amount; i++) {
		cardHolder = removeCard(myDeck); //takes a card from the end of the deck
		addCard(cardHolder, &(user->headl), &(user->headr)); //adds the removed card to the player's hand
	}

}

//Gives if user hand is empty
int emptyHand(player *user) {
	//If left head of user's hand is null, then the user's hand is empty
	return (user->headl == NULL);
}

//Checks if every player has empty hands
int gameOver(player players[], int size) {

	//Loops through players
	for (int i = 0; i < size; i++) {

		//If any user does not have an empty hand the game keeps going
		if (!emptyHand(&players[i]))
			return 0;
	}

	//FIXME: should check for winner if the game is over. Can call a function

	//If all players have empty hands, then game is over
	return 1;
}

//Reads cards from a file and puts them in a deck
void readFile(FILE *inp, deck *myDeck) {

	//Creates holds for the rank and suit
	char rank[3];
	char inSuit[9];

	//Scans in rank and suit at the same time
	while (fscanf(inp, "%2s %8s", rank, inSuit) > 0) {

		//Creates the card
		card *temp = malloc(sizeof(card)); //allocates space for card
		temp->next = NULL; //nothing after card
		temp->previous = NULL; //nothing before card
		strcpy(temp->suit, inSuit); //puts the suit from the file into the card
		strcpy(temp->value, rank); //puts the value from the file into the card

		//Adds the card to the deck
		addCard(temp, &(myDeck->headl), &(myDeck->headr));

	}

}


//Asks user if they want to continue
void continueCheck() {
	char answer;

	printf("*** Hit C to Continue ***:");
	scanf("%c", &answer);

	//makes sure that the input is not a newline character
	while (answer == '\n')
		scanf("%c", &answer);

	//ends the program if they don't hit C
	if (answer != 'C' && answer != 'c')
		exit(0);
}

/*
//Checks for a match
void checkMatch(player *current, char rank[3])
{
	//has variable for holding number of times the rank appears

	//loops through all the cards and increments variable every time rank appears

	//after loop, if variable says rank appeared four times, then add point to player and remove cards from player's hand

}
*/

//Adds a point to a player's score
/*void addPoint(player scorer)
{
	scorer.points++;
}
*/

////Checks if player has the specified card
//int lookForCard(card *checkCard, player *playerCheck, player *playerGive) {
//
//	card *indexer = playerCheck->headl;
//	while (indexer != NULL) {
//		if (strcmp(indexer->value, checkCard->value) == 0) {
//
//		}
//		//Goes to next card in linked list
//		indexer = indexer->next;
//	}
//
//}

/*
//Checks which player won
int getWinner(player players[])
{
	//creates variables to hold which player currently has the most points and how many points they have

	//loops through each player and checks if they have the maximum number of points

	//returns the index of the player who won
}

*/


//-------------------------------------------------------------------------------------------------GAMEPLAY

//Holds the gameplay of the go fish game
int main(void)
{

	//randomizes the seed for the random function
	srand((int)time(NULL));

	//Creates a deck
	deck *myDeck = malloc(sizeof(deck));
	initDeck(myDeck);

	//Creates file
	FILE *inp;
	inp = fopen("cards.txt", "r");

	//checks if file exists
	if (inp == NULL)
	{
		printf("File not found.");
		return 0;
	}

	//Function to read file
	readFile(inp, myDeck);

	printf("Unshuffled deck is:\n");
	printCards(myDeck->headl);

	//Shuffles the deck
	shuffle(myDeck);

	printf("Shuffled deck is:\n");
	printCards(myDeck->headl);

	//Variations

	//Extra credit
	//A player gives only one card when asked.
	//A player forms and lays down pairs instead of 4 - card books.
	//A player whose call is unsuccessful and draws the card being asked for does not get another turn.
	//A player asks for a specific card instead of a rank.A player must still have at least one card of the named rank in order to ask, and must expose that card when asking.This is similar to Happy Families.

	//Creates the players
	//Players get cards in hand (Default 7)----------(Extra credit more less cards, with more players)
	int playerAmount = 2;
	char playerName[50];

	//Creates an array of players
	player *players = malloc(sizeof(player) * playerAmount);
	
	//Makes a player struct for each player
	for (int i = 0; i < playerAmount; i++) {
		//asks user for name
		printf("Enter your name, Player %d:", i + 1); //+ 1 necessary otherwise it will start at Player 0
		scanf("%s", playerName);

		//Initializes the player
		initPlayer(&players[i], playerName);

	}

	//Greets the players
	printf("| --------- --------- --------- --------- |\n"); 
	printf("| --------- --------- --------- --------- |\n");
	for (int i = 0; i < playerAmount; i++) 
	{
		printf("%s, ", players[i].name);
	}
	printf("\nLet's play Go Fish\n");
	printf("| --------- --------- --------- --------- |\n");
	printf("| --------- --------- --------- --------- |\n");


	//Prints the players' cards
	for (int i = 0; i < playerAmount; i++) {
		//Gives them cards
		getCards(&players[i], myDeck, 7);

		//Debug to print their hand
		printf("%s has cards:\n", players[i].name);
		printCards(players[i].headl);
	}


	int player = 0; //specifies whose turn it is
	int pickedPlayer;
	char rank[3];

	//Asks player to pick cards 
	while (!gameOver(players, playerAmount))
	{
		//ask user if they want to continue the game
		continueCheck();

		//Option of picking player when more than 2
		if (playerAmount > 2) {
			//Makes sure while loops runs at least once
			pickedPlayer = player + 1;
			//Keeps asking user to pick a player until they pick a valid one
			while (pickedPlayer == (player + 1)) {
				printf("Player %d, pick a player:\n", player + 1); //Need to + 1 because indexing starts with 0
				scanf("%d", &pickedPlayer);
			}
		}
		//If there are just two players
		else {
			pickedPlayer = !player;
		}

		printf("Player %d what rank do you want to ask Player %d for?\n", player + 1, pickedPlayer + 1);//Need to + 1 because indexing starts with 0
		scanf("%2s", rank);

		//	Player asks for a card (Default by rank)----------(Extra credit specific card)

		//	If someone else has that card
		//	{

		//		Player that was asked gives card(Default all of them that match)----------(Extra credit, just one)

		//	}
		//	else card is wrong
		//	{

		//		Player picks from pile of cards

		//		If card from pile is not the card the player asked for ----------(This does not happen with extra credit)
		//		{
		//			moves to next player;
		//		}
		//	}

		//	Game checks if player got match (Four default)----------(Extra credit 2)

		//Moves to next player by incrementing and going back to player one if it's their turn again
		player++;
		if (player > playerAmount - 1)//Arrays start with zero, thus I substract one
			player = 0;
	}

}
