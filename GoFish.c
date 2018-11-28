/*
Authors: Jonathan Goh and Andres Barragan
Date Created: 11/27/2018
Description of the Program: Plays a game of Go Fish
*/

#define _CRT_SECURE_NO_WARNINGS
#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include < math.h>
#include <time.h>

//To be able to prints the symbols
#if defined(_WIN32) || defined(__MSDOS__)
	#define SPADE   "\x06"
	#define CLUB    "\x05"
	#define HEART   "\x03"
	#define DIAMOND "\x04"
#else
	#define SPADE   "\xE2\x99\xA0"
	#define CLUB    "\xE2\x99\xA3"
	#define HEART   "\xE2\x99\xA5"
	#define DIAMOND "\xE2\x99\xA6"
#endif

//adds structure for card
typedef struct card_s
{
	char suit[9]; //FIXME: may need to change 7 to a 9 because "diamonds" is longer than 7
	int value;

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

	card hand;
	int points;

} player;

//--------------------------------------------------------------------------------------------------------DECK STUFF

//defines function that adds members to list
void addCards(card *p, card **hl, card **hr)
{
	//If node starts completly empty
	if (*hr == NULL) {
		*hl = p;
		*hr = p;
		return;
	}//Adds to the end of the doubly linked list
	else {
		(*hr)->next = p;
		p->previous = *hr;
		*hr = p;
	}
}

//Swaps values in a linked list
void swap(card *pt, int i, int j) {

	//Grabs the first node
	card *index1 = pt;
	for (int iter = 0; iter < i; iter++)
		index1 = index1->next;

	//Grabs the second node
	card *index2 = pt;
	for (int iter = 0; iter < j; iter++)
		index2 = index2->next;

	//Stores temp holders for swapped values
	char tempSuit[9];
	strcpy(tempSuit, index1->suit);
	int tempValue = index1->value;

	//Sets index1 from index2
	strcpy(index1->suit, index2->suit);
	index1->value = index2->value;

	//Sets index2 from the stored values of index1
	strcpy(index2->suit, tempSuit);
	index2->value = tempValue;

}

//defines function that obtains a random number
int rand_gen(int count)
{
	double frac;
	frac = (double)rand() / ((double)RAND_MAX + 1);
	return (int) floor(count*frac);
}

//Swaps 10000 randoms cards in the deck to shuffle
void shuffle(deck *myDeck) {

	//Randomly swaps the values in the double linked list as many times as user wants
	int first, second;
	for (int i = 0; i < 10000; i++) {
		first = rand_gen(myDeck->deckSize);
		second = rand_gen(myDeck->deckSize);
		swap(myDeck->headl, first, second);
	}

}

//Initilizes a deck
void initDeck(deck *myDeck) {

	myDeck->deckSize = 52;
	myDeck->headl = NULL;
	myDeck->headr = NULL;

}

//Prints a linked list of cards, goes to end of list
void printCards(card *cards) {

	card *indexer = cards;
	while (indexer != NULL) {

		//Checks what suit to print
		if (strcmp(indexer->suit, "diamonds"))
			printf("%s", DIAMOND);
		else if (strcmp(indexer->suit, "clubs"))
			printf("%s", CLUB);
		else if (strcmp(indexer->suit, "spades"))
			printf("%s", SPADE);
		else
			printf("%s", HEART);

		//Prints the card value next to suit
		printf("%d ", indexer->value);

		//Goes to next card in linked list
		indexer = indexer->next;
	}

}

//Removes a card from the end of a deck
card *removeCard(deck *myDeck) {

	//Removes and grabs card from deck
	card *endCard = myDeck->headr;
	myDeck->headr->previous->next = NULL;
	
	//Decrements decksize
	myDeck->deckSize--;

	//Returns grabbed card
	return endCard;
}

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

	//Shuffles the deck
	shuffle(myDeck);

	//Variations

	//Extra credit
	//A player gives only one card when asked.
	//A player forms and lays down pairs instead of 4 - card books.
	//A player whose call is unsuccessful and draws the card being asked for does not get another turn.
	//A player asks for a specific card instead of a rank.A player must still have at least one card of the named rank in order to ask, and must expose that card when asking.This is similar to Happy Families.
	//

	//PsuedoCode

	//Creates the players
	player players[2];

	//Players get cards in hand (Default 7)----------(Extra credit more less cards, with more players)

	//While(until everyone has an empty hand, player with most matches wins at the end)
	//{
	//	//(Starts at player 1)

	//	Player picks a player (Not themselves)
	//	Player asks for a card (Default by rank)----------(Extra credit specific card)

	//	If card is answered correctlys
	//	{

	//		Player that was asked gives card(Default all of them that match)----------(Extra credit, just one)

	//	}
	//	else card is wrong
	//	{

	//		Player picks from pile of cards

	//		If card from pile is not the card the player asked for ----------(This does not happen with extra credit)
	//		{
	//			next player set;
	//		}
	//	}

	//	Game checks if player got match got full match(Four default)----------(Extra credit 2)

	//}

}
