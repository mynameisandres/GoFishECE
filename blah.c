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

//Holds the gameplay of the go fish game
int main(void)
{
	while (3 != 4)
		continueCheck();

	return 0;
}