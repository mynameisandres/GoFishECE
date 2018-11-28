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

const DECKSIZE = 52;

//adds structure for card
typedef struct card_s
{
	char suit[9]; //FIXME: may need to change 7 to a 9 because "diamonds" is longer than 7
	int value;

	struct card_s *next;
	struct card_s *previous;

} card;

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

void shuffle(card *headl) {

	//Randomly swaps the values in the double linked list as many times as user wants
	int first, second;
	for (int i = 0; i < 1000; i++) {
		first = rand_gen(DECKSIZE);
		second = rand_gen(DECKSIZE);
		swap(headl, first, second);
	}

}

//add print function
//add delete function

//-------------------------------------------------------------------------------------------------GAMEPLAY

int main(void)
{
	//randomizes the seed for the random function
	srand((int)time(NULL));

	//declares pointers
	card *headl = NULL;
	card *headr = NULL;

	//declares variables
	FILE *inp;
	int numNodes = 0;
	char cont = 'y';
	int swaps = 100; //may need to increase

	//FIXME: READ CARDS IN FROM cards.txt file
	//opens file containing the 52 cards
	inp = fopen("cards.txt", "r");

	//checks if file exists
	if (inp == NULL)
	{
		printf("File not found.");
		return 0;
	}

	//Variations

	/*Extra credit
	A player gives only one card when asked.
	A player forms and lays down pairs instead of 4 - card books.
	A player whose call is unsuccessful and draws the card being asked for does not get another turn.
	A player asks for a specific card instead of a rank.A player must still have at least one card of the named rank in order to ask, and must expose that card when asking.This is similar to Happy Families.
	*/

	/*PsuedoCode

	Players get cards in hand (Default 7)----------(Extra credit more less cards, with more players)

	While(until everyone has an empty hand, player with most matches wins at the end)
	{
		//(Starts at player 1)

		Player picks a player (Not themselves)
		Player asks for a card (Default by rank)----------(Extra credit specific card)

		If card is answered correctlys
		{

			Player that was asked gives card(Default all of them that match)----------(Extra credit, just one)

		}
		else card is wrong
		{

			Player picks from pile of cards

			If card from pile is not the card the player asked for ----------(This does not happen with extra credit)
			{
				next player set;
			}
		}

		Game checks if player got match got full match(Four default)----------(Extra credit 2)

	}
	*/

}
