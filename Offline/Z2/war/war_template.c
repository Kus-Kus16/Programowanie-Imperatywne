#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_CARDS 52
#define NUMBER_OF_COLORS 4

int random_from_interval(int a, int b) {
}
void shuffle(int t[], int size) {
}
void game(int n, int player_A[], int player_B[], int max_conflicts, int simplified_mode) {
}
void first_takes_cards(int number_of_cards_on_table, int player_1[], int *pout_1, int *p_cards_1,
		const int player_2[], int *pout_2, int *p_cards_2, int size) {
}
void both_take_cards(int player_1[], int *pout_1, const int *p_cards_1,
		int player_2[], int *pout_2, const int *p_cards_2, int size) {
}

int main(void) {
	int player_A[NUMBER_OF_CARDS], player_B[NUMBER_OF_CARDS];
	int deck[NUMBER_OF_CARDS];
	int max_conflicts;
	int simplified_mode;
	int seed;
	scanf("%d", &seed);
	scanf("%d", &simplified_mode);
	scanf("%d", &max_conflicts);

	for(int i = 0; i < NUMBER_OF_CARDS; i++) deck[i] = i;
	srand((unsigned) seed);
	shuffle(deck, NUMBER_OF_CARDS);
	for(int i = 0; i < NUMBER_OF_CARDS / 2; i++) {
		player_A[i] = deck[i];
		player_B[i] = deck[i + NUMBER_OF_CARDS / 2];
	}
	game(NUMBER_OF_CARDS, player_A, player_B, max_conflicts, simplified_mode);
	return 0;
}

