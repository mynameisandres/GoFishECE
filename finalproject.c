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

	//shuffles the cards in the list

	//asks players for their names and displays the greeting

	//deals cards to the players by adding cards to player lists and deleting cards from the pool

	//prints the player's hands

	//loops until all cards are matched

	//asks player to enter C to continue

	//takes turns asking players what card they want to take

	//checks if their move is valid

	//checks if opponents have the card
		//adds card to player's hand
		//checks if a match is found
			//adds point to player's score and removes cards from play
		//allows the player to go again
}


//defines function that adds members to list
void addCards(node*p, node **hl, node **hr, FILE *input)
{
	//loops until reaching the end of file
	//check ece 175 notebook for fgets function, declare string variable for holding lines
	//while

	//creates temporary node and allocates space
	node *temp;
	temp = (node*)malloc(sizeof(node));

	//records input
	printf("Enter an integer: ");
	scanf("%d", &temp->x);

	//adds the member to the end of the list
	if (*hl == NULL) //checks if list is empty
	{
		temp->previous = NULL;
		temp->next = NULL;
		*hl = temp;
		*hr = temp;
	}
	else
	{
		temp->previous = p;
		temp->next = NULL;
		p->next = temp;
		*hr = temp;
	}
}

//defines function that swaps the contents of two elements of the list
void swap(node *pt, int i, int j)
{
	//declares variables
	node *first = pt;
	int ival = -1;
	int jval = -1;
	int counter = 0;
	int iChanged = 0;
	int jChanged = 0;

	while (ival == -1 || jval == -1)
	{
		if (counter == i)
			ival = pt->x;
		if (counter == j)
			jval = pt->x;

		counter++;
		if (pt->next != NULL)
			pt = pt->next;
	}

	pt = first; //resets pointer 
	counter = 0;

	while (iChanged == 0) //changes i element to have j value
	{
		if (counter == i)
		{
			pt->x = jval;
			iChanged = 1;
		}

		counter++;
		if (pt->next != NULL)
			pt = pt->next;
	}

	pt = first; //resets pointer 
	counter = 0;

	while (jChanged == 0) //changes i element to have j value
	{
		if (counter == j)
		{
			pt->x = ival;
			jChanged = 1;
		}

		counter++;
		if (pt->next != NULL)
			pt = pt->next;
	}
}

//defines function that obtains a random number
int rand_gen(int count)
{
	double frac;
	frac = (double)rand() / ((double)RAND_MAX + 1);
	return floor(count*frac);
}
