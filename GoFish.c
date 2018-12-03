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
#define SPADE   "(S)"
#define CLUB    "(C)"
#define HEART   "(H)"
#define DIAMOND "(D)"

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

	//Indicates win status of player
	int winStatus;

	//Indicates if is AI
	int isAI;

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
	user->winStatus = 0; //not a winner
	user->isAI = 0; //Starts with AI being zero
}

//Prints all the cards in a linked list
void printCards(card *cards)
{
	//declares variables
	char symbols[52][6];
	char currentSymbols[6];
	int numCards = 0;
	card *indexer = cards;

	//gets the symbols(rank & suit) from all the cards and counts how many cards there are
	while (indexer != NULL)
	{
		//sets currentSymbols variable to an empty string
		strcpy(currentSymbols, "");

		//concatenates the rank to the string
		strcat(currentSymbols, indexer->value);

		//determines which suit to concatenate
		if (strcmp(indexer->suit, "diamonds") == 0)
			strcat(currentSymbols, DIAMOND);
		else if (strcmp(indexer->suit, "clubs") == 0)
			strcat(currentSymbols, CLUB);
		else if (strcmp(indexer->suit, "spades") == 0)
			strcat(currentSymbols, SPADE);
		else
			strcat(currentSymbols, HEART);

		//concatenates a space if the rank was not 10
		if (strcmp(indexer->value, "10") != 0)
		{
			strcat(currentSymbols, " ");
		}

		//stores the current card's symbols into the multidimensional string array
		strcpy(symbols[numCards], currentSymbols);

		//Goes to next card in linked list
		indexer = indexer->next;

		//increments the number of cards
		numCards++;
	}

	if (numCards == 0) {
		printf("No cards\n");
		return;
	}

	//prints the tops of the ascii cards
	for (int i = 0; i < numCards; i++)
	{
		printf("---------");
		printf("   ");
	}
	printf("\n");

	//prints the symbols in the top left corner of the ascii card
	for (int i = 0; i < numCards; i++)
	{
		printf("|%s  |", symbols[i]);
		printf("   ");
	}
	printf("\n");

	//prints the middles of the ascii cards
	for (int n = 0; n < 3; n++)
	{
		for (int i = 0; i < numCards; i++)
		{
			printf("|       |");
			printf("   ");
		}
		printf("\n");
	}

	//prints the symbols in the top left corner of the ascii card
	for (int i = 0; i < numCards; i++)
	{
		printf("|  %s|", symbols[i]);
		printf("   ");
	}
	printf("\n");

	//prints the bottomss of the ascii cards
	for (int i = 0; i < numCards; i++)
	{
		printf("---------");
		printf("   ");
	}
	printf("\n");
}

//Removes a card from the end of a deck
card *removeCard(deck *myDeck) {

	//Removes and grabs card from deck
	card *endCard = myDeck->headr; //obtains the last card in the deck

	//If only one card in deck
	if (myDeck->deckSize == 1) {
		myDeck->headl = NULL;
		myDeck->headr = NULL;
	}//Otherwise
	else {
		myDeck->headr->previous->next = NULL; //the second-to-last card points to null
		myDeck->headr = myDeck->headr->previous; //the last card is the second-to-last card
		endCard->previous = NULL; // the card that was taken now has no card before it
	}

	//Decrements decksize
	myDeck->deckSize--;

	//Returns grabbed card
	return endCard;
}

//Adds a point to a player's score
void addPoint(player *scorer)
{
	(scorer->points)++;
}

//Checks if player has the specified card
card *lookForCard(char rank[], player *playerCheck) {

	card *returnMe = NULL;

	card *indexer = playerCheck->headl;
	//Loops through all the cards in hand
	while (indexer != NULL) {
		if (strcmp(indexer->value, rank) == 0) {

			//When there is only one card in the players hand
			if ((playerCheck->headl == indexer) && (playerCheck->headr == indexer)) {
				returnMe = playerCheck->headl;
				playerCheck->headl = NULL;
				playerCheck->headr = NULL;
			}//When the card that mathced is headl
			else if (playerCheck->headl == indexer) {
				returnMe = playerCheck->headl;
				playerCheck->headl->next->previous = NULL;
				playerCheck->headl = playerCheck->headl->next;
			}//When the card that mathced is headr
			else if (playerCheck->headr == indexer) {
				returnMe = playerCheck->headr;
				playerCheck->headr->previous->next = NULL;
				playerCheck->headr = playerCheck->headr->previous;
			}//When the card is simply in the middle of cards
			else {
				indexer->next->previous = indexer->previous;
				indexer->previous->next = indexer->next;
				returnMe = indexer;

			}

			returnMe->next = NULL;
			returnMe->previous = NULL;
			return returnMe;
		}
		//Goes to next card in linked list
		indexer = indexer->next;
	}
	return returnMe;
}

//Checks for a match
void checkMatch(player *current, char rank[3], int matchNum)
{
	int match = 0;

	card *indexer = current->headl;
	//Loops through the cards in the users hand
	while (indexer != NULL) {

		if (strcmp(indexer->value, rank) == 0) {
			match++;

			//When the amount of matches meets the requirements for the function
			if (match == matchNum) {
				addPoint(current);

				//prints ascii art
				printf(" __       __   ______   ________  ______   __    __  __ \n");
				printf("/  \\     /  | /      \\ /        |/      \\ /  |  /  |/  |\n");
				printf("$$  \\   /$$ |/$$$$$$  |$$$$$$$$//$$$$$$  |$$ |  $$ |$$ |\n");
				printf("$$$  \\ /$$$ |$$ |__$$ |   $$ |  $$ |  $$/ $$ |__$$ |$$ |\n");
				printf("$$$$  /$$$$ |$$    $$ |   $$ |  $$ |      $$    $$ |$$ |\n");
				printf("$$ $$ $$/$$ |$$$$$$$$ |   $$ |  $$ |   __ $$$$$$$$ |$$/ \n");
				printf("$$ |$$$/ $$ |$$ |  $$ |   $$ |  $$ \\__/  |$$ |  $$ | __ \n");
				printf("$$ | $/  $$ |$$ |  $$ |   $$ |  $$    $$/ $$ |  $$ |/  |\n");
				printf("$$/      $$/ $$/   $$/    $$/    $$$$$$/  $$/   $$/ $$/ \n");
				
				printf("\n%s got a match of %s\n", current->name, rank);

				//Removes cards from the hand
				card *found = current->headl;
				while (found != NULL) {//Gets all the specified card from the user hand
					found = lookForCard(rank, current);
				}

				return;
			}
		}
		indexer = indexer->next;
	}
}

//Draws a specified number of cards from a deck and puts them in a player's hand
void getCards(player *user, deck *myDeck, int amount) {

	card *cardHolder = NULL; //represents the card drawn from the deck
	for (int i = 0; i < amount; i++) {
		//If deck is empty
		if (myDeck->headl == NULL)
			break;
		cardHolder = removeCard(myDeck); //takes a card from the end of the deck
		addCard(cardHolder, &(user->headl), &(user->headr)); //adds the removed card to the player's hand
		checkMatch(user, cardHolder->value, 4);
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

//Grabs a card from the deck
card *goFish(deck *myDeck, player *giveCard) {

	//Does the go fish
	printf("\n><(((('> ><(((('> ><(((('>\n");
	printf("><(((('>          ><(((('>\n");
	printf("~~~    Go fish, %s ~~~\n", giveCard->name);
	printf("><(((('>          ><(((('>\n");
	printf("><(((('> ><(((('> ><(((('>\n");
	getCards(giveCard, myDeck, 1);

	//Returns the card that was added to the players hand
	return giveCard->headr;
}

//Checks which player won
void getWinners(player players[], int size)
{
	//Defaults max to the first players points

	int maxPoints = players[0].points;
	for (int i = 1; i < size; i++) {

		//Updates the max points a player has
		if (players[i].points > maxPoints)
			maxPoints = players[i].points;
	}
	int winners = 0;
	//Finds winners of the players, there points has to equal the max
	for (int i = 0; i < size; i++) {
		if (players[i].points == maxPoints) {
			winners++;
			players[i].winStatus = 1;
		}
	}

	//prints ascii art
	printf(" __      __  ______   __      __  __ \n");
	printf("/  \\    /  |/      \\ /  \\    /  |/  |\n");
	printf("$$  \\  /$$//$$$$$$  |$$  \\  /$$/ $$ |\n");
	printf(" $$  \\/$$/ $$ |__$$ | $$  \\/$$/  $$ |\n");
	printf("  $$  $$/  $$    $$ |  $$  $$/   $$ |\n");
	printf("   $$$$/   $$$$$$$$ |   $$$$/    $$/ \n");
	printf("    $$ |   $$ |  $$ |    $$ |     __ \n");
	printf("    $$ |   $$ |  $$ |    $$ |    /  |\n");
	printf("    $$/    $$/   $$/     $$/     $$/ \n\n");

	//Indicates if it was a tie or there is a single winner
	if (winners > 1) {
		printf("It is a tie, winners are: \n");
	}
	else {
		printf("Winner is: \n");
	}

	//Prints the winner
	for (int i = 0; i < size; i++) {
		if (players[i].points == maxPoints) {
			printf("\t%s with match count of %d\n", players[i].name, players[i].points);
		}
	}
}

//Checks which player won
int allHandsEmpty(player players[], int size)
{
	//Loops through all the players
	for (int i = 1; i < size; i++) {

		//Returns false if any of the players has non-empty hand
		if (players[i].headl != NULL)
			return 0;
	}
	//ALl players had empty hands
	return 1;

}

int findIndexOfPlayer(player *players, char *playerName, int playerAmount) {

	//Loops through players and check if name matches
	for (int i = 0; i < playerAmount; i++) {
		if (strcmp(players[i].name, playerName) == 0)
			return i;
	}

	return -1;


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
	readFile(inp, myDeck);

	//Shuffles Deck
	shuffle(myDeck);

	//Declares variables
	int playerAmount = 0;
	int cardsDealt;

	//Asks how many players there will be until they give a valid response
	while ((playerAmount < 2) || (playerAmount > 10))
	{
		printf("How many players are playing? (2-10): ");
		scanf("%d", &playerAmount);

		//checks if input was valid
		if ((playerAmount < 2) || (playerAmount > 10))
			printf("Invalid entry.\n");
		scanf("%*c"); //deletes newline in buffer
		printf("\n"); //prints newline
	}

	//determines how many cards to deal
	if (playerAmount > 4)
		cardsDealt = 5;
	else
		cardsDealt = 7;

	char playerName[50];

	//Creates an array of players
	player *players = malloc(sizeof(player) * playerAmount);

	//Makes a player struct for each player
	int AIIndexer = 1;
	for (int i = 0; i < playerAmount; i++) {

		//asks user for name
		printf("Player %d enter your name(Enter AI for AI): ", i + 1); //+ 1 necessary otherwise it will start at Player 0
		scanf("%s", playerName);

		//Initializes the player as an AI
		if (strcmp(playerName, "AI") == 0) {
			sprintf(playerName, "%s%d", playerName, AIIndexer);
			AIIndexer++;
			initPlayer(&players[i], playerName);
			players[i].isAI = 1;
		}//Not an AI
		else {
			initPlayer(&players[i], playerName);
		}


	}

	//Greets the players
	printf("\n| --------- --------- --------- --------- |\n");
	printf("| --------- --------- --------- --------- |\n");
	for (int i = 0; i < playerAmount - 1; i++)
	{
		printf("%s, ", players[i].name);
	}
	printf("%s", players[playerAmount - 1].name);
	printf("\nLet's play Go Fish\n");
	printf("| --------- --------- --------- --------- |\n");
	printf("| --------- --------- --------- --------- |\n\n");


	//Prints the players' cards
	for (int i = 0; i < playerAmount; i++) {
		//Gives them cards
		getCards(&players[i], myDeck, cardsDealt);

		//Debug to print their hand
		printf("%s has cards:\n", players[i].name);
		printCards(players[i].headl);
	}


	int player = 0; //specifies whose turn it is
	int pickedPlayer;
	char rank[3];

	//Asks player to pick cards
	int repeatTurns = 0;
	while (!gameOver(players, playerAmount))
	{
		//Asks the user if they want to continue. COMMENT OUT IF FEATURE UNWANTED
		//continueCheck();

		//Let's player go with the have cards
		if(players[player].headl != NULL){

			//Option of picking player when more than 2
			if (playerAmount > 2) {

				//Name is AI simply gets first card in hand
				card *askedCard = NULL;
				if (players[player].isAI) {

					printf("\n%s, pick a player to ask cards from:\n", players[player].name);
					//Keeps picking until the picked is not the same as the player
					pickedPlayer = player;
					while (pickedPlayer == player)
						pickedPlayer = rand_gen(playerAmount);

				}//Keeps asking the player for a card until they give a card they have in hand
				else {

					char pickedPlayerName[50] = "";
					int pickedSelf = 0;
					//Keeps asking user to pick a player until they pick a valid one
					while (((pickedPlayer = findIndexOfPlayer(players, pickedPlayerName, playerAmount)) == -1) || pickedSelf) {
						printf("\n%s, pick a player to ask cards from:\n", players[player].name);
						scanf("%s", pickedPlayerName);
						pickedSelf = 0;
						//Check if they picked themselves
						if ((strcmp(players[player].name, pickedPlayerName) == 0)) {
							//Cannot choose themselves
							pickedSelf = 1;
							printf("Cannot pick your self\n");
						}
					}
				}
				printf("%s chose %s\n", players[player].name, players[pickedPlayer].name);//Need to + 1 because indexing starts with 0
			}
			//If there are just two players
			else {
				pickedPlayer = !player;
			}

			//Name is AI simply gets first card in hand
			card *askedCard = NULL;
			if (players[player].isAI) {
				printf("\n%s, what rank do you want to ask %s for?\n", players[player].name, players[pickedPlayer].name);//Need to + 1 because indexing starts with 0
				askedCard = players[player].headl;
			}//Keeps asking the player for a card until they give a card they have in hand
			else {
				while (askedCard == NULL) {

					//Gets rank from the player and checks if they have the card in there hand
					printf("\n%s, what rank do you want to ask %s for?\n", players[player].name, players[pickedPlayer].name);//Need to + 1 because indexing starts with 0
					scanf("%2s", rank);
					askedCard = lookForCard(rank, &players[player]);

					//Adds card back to the users hand if they have it
					if (askedCard != NULL)
						addCard(askedCard, &players[player].headl, &players[player].headr);
					else
						printf("You do not have that card\n");
				}
			}
			printf("%s picked %s\n", players[player].name, askedCard->value);//Need to + 1 because indexing starts with 0

			//Run while loop at least once, unless user hand is empty(Might have to fix this later)
			card *found = players[player].headl;
			int cardsFound = 0;
			while (found != NULL) {//Gets all the specified card from the user hand

				found = lookForCard(askedCard->value, &players[pickedPlayer]);
				if (found != NULL) {
					cardsFound++;
					addCard(found, &players[player].headl, &players[player].headr);
					checkMatch(&players[player], found->value, 4);
				}
			}

			//This occurs when the user has to go fish
			card *goFishCard = NULL;
			if (!cardsFound) {
				goFishCard = goFish(myDeck, &players[player]);
				checkMatch(&players[player], goFishCard->value, 4);
			}//Tells the player how many cards they found form the user
			else {
				//Fixes the grammar of the sentences
				if (cardsFound == 1)
					printf("%s had %s, 1 time.\n", players[pickedPlayer].name, askedCard->value);
				else
					printf("%s had %s, %d times.\n", players[pickedPlayer].name, askedCard->value, cardsFound);
			}

			//Adds cards to hand if the opponents hand are empty
			if (players[player].headl == NULL)
				getCards(&players[player], myDeck, cardsDealt);

			//Adds cards to hand if the players hand are empty
			if (players[pickedPlayer].headl == NULL)
				getCards(&players[pickedPlayer], myDeck, cardsDealt);

			//prints newline to mark a new round
			printf("\n|-------------------------------------------------------------------------------------------------------|\n");

			//Prints the players' cards
			for (int i = 0; i < playerAmount; i++) {
				//Debug to print their hand
				printf("%s has cards:\n", players[i].name);
				printCards(players[i].headl);
			}

			//Moves to next player by incrementing and going back to player one if it's their turn again
			if ((!cardsFound && (goFishCard == NULL || (strcmp(goFishCard->value, askedCard->value) != 0)))) {
				player++;
				repeatTurns = 0;
			}//Do not ask if they want to continue
			else
				repeatTurns = 1;
		}else {
			player++;
		}
		//Can't exceed the player amount
		if (player > playerAmount - 1)//Arrays start with zero, thus I substract one
			player = 0;
	}

	//States who won
	getWinners(players, playerAmount);

}
