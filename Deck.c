typedef struct s_card{

	char suit[7];
	int value;
	struct s_card *pt;

}card;

typedef struct s_deck{

	card cards[52];


}deck;