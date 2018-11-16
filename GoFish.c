//Variations

/*Extra credit
A player gives only one card when asked.
A player forms and lays down pairs instead of 4 - card books.
A player whose call is unsuccessful and draws the card being asked for does not get another turn.
A player asks for a specific card instead of a rank.A player must still have at least one card of the named rank in order to ask, and must expose that card when asking.This is similar to Happy Families.
*/

/*PsuedoCode

Players get cards in hand (Default 7)----------(Extra credit more less cards, with more players)

All players go(alternating) until everyone has an empty hand, player with most matches wins at the end
{
	Player goes{ //(Starts at player 1)
	{
		
		Player picks a player (Not themselves)
		Player1 asks for a card (Default by rank)(Extra credit specific card)

		If card is answered correctlys
		{
			Player that was asked gives card(Default all of them that match)----------(Extra credit, just one)
			Game checks if they got full match(Four default)----------(Extra credit 2)
			continue;
		}

		If card is wrong
		{
			Player picks from pile of cards

			Game checks if they got full match(Four default)----------(Extra credit 2)

			If card from pile is the card the player asked for ----------(This does not happen with extra credit)
			{
				continue;
			}
			else ----------(Only this with extra credit)
			{
				next player set;
				break;
			}

		}
	}
}
*/


#include <stdio.h>
#include "Player.h"
#include "Deck.h"

int main() {

	card *card1 = NULL;

}

