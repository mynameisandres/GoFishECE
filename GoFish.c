/*
Authors: Jonathan Goh and Andres Barragan
Date Created: 11/27/2018
Description of the Program: Plays a game of Go Fish
*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>

//adds structure for card
typedef struct card_s
{
	char suit[9]; //FIXME: may need to change 7 to a 9 because "diamonds" is longer than 7
	int value;
	struct card_s *pt;
} card;

//adds structure for item in list FIXME: CHANGE THE INT X TO CARD
typedef struct node_s {
	int x;
	struct node_s *next;
	struct node_s *previous;
} node;

//function prototypes
void addCards(node *p, node **hl, node **hr, FILE *input);
void swap(node *pt, int i, int j);
int rand_gen(int count);
//add print function
//add shuffle function that calls swap function
//add delete function

//-------------------------------------------------------------------------------------------------GAMEPLAY

int main(void)
{
	//randomizes the seed for the random function
	srand((int)time(NULL));

	//declares pointers
	node *headl = NULL;
	node *headr = NULL;

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

	//calls function to read cards into list
	addCards(headr, &headl, &headr, inp);

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

//--------------------------------------------------------------------------------------------------------DECK STUFF

//defines function that adds members to list
void addCards(node*p, node **hl, node **hr)
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

void swap(node *pt, int i, int j) {

	//Grabs the first node
	node *index1 = pt;
	for (int iter = 0; iter < i; iter++)
		index1 = index1->next;

	//Grabs the second node
	node *index2 = pt;
	for (int iter = 0; iter < j; iter++)
		index2 = index2->next;

	//Swaps the values
	int temp = index1->x;
	index1->x = index2->x;
	index2->x = temp;

}

//defines function that obtains a random number
int rand_gen(int count)
{
	double frac;
	frac = (double)rand() / ((double)RAND_MAX + 1);
	return floor(count*frac);
}
