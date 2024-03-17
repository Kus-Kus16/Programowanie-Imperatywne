#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_CARDS 52
#define NUMBER_OF_COLORS 4

enum state { OK = 4 };

// -- Module A - Shuffling --

int random_from_interval(int a, int b) {

	return ( rand() % ( b-a+1 ) ) + a;

}

void swap(int array[], int a, int b){

	int tmp=array[a];
	array[a]=array[b];
	array[b]=tmp;

}

void shuffle(int deck[], int size) {

	int k;
	for ( int i=0 ; i<size-1 ; i++ ){
		k=random_from_interval(i,size-1);
		swap(deck,i,k);
	}

}

// -- Module B - Core gameplay --

int top_A=0, top_B=0;
int len_A=26, len_B=26; //!!! 26

int n_cards_ontable=0; //per player
int conflicts_cnt=0;

int strength(int ID){
	//S: 2-2, 3-3, ... , 10-10, J-11, Q-12, K-13, A-14

	int i = ( ID + NUMBER_OF_COLORS ) / NUMBER_OF_COLORS;
	return i+1;

}

void one_takes(int winner[], int loser[], int *top_W, int *top_L, int *len_W, int *len_L){
	int free_space;

	//own cards
	free_space = ( *top_W + n_cards_ontable + *len_W ) % NUMBER_OF_CARDS;
	for ( int i=0 ; i < n_cards_ontable ; i++ ){
		winner[ free_space + i ] = winner[ *top_W + i ];
		(*len_W) ++;
	}
	*top_W += n_cards_ontable;

	//opponent cards
	free_space = ( *top_W + *len_W ) % NUMBER_OF_CARDS;
	for ( int i=0 ; i < n_cards_ontable ; i++ ){
		winner[ free_space + i ] = loser[ *top_L + i ];
		(*len_W) ++;
	}
	*top_L += n_cards_ontable;

}

void both_take(int A_deck[], int B_deck[]){
	int free_space;

	//A cards
	free_space = ( top_A + n_cards_ontable + len_A ) % NUMBER_OF_CARDS;
	for ( int i=0 ; i<n_cards_ontable ; i++ ){
		A_deck[ free_space + i ] = A_deck[ top_A + i ];
		len_A ++;
	}
	top_A += n_cards_ontable;

	//B cards
	free_space = ( top_B + n_cards_ontable + len_B ) % NUMBER_OF_CARDS;
	for ( int i=0 ; i<n_cards_ontable ; i++ ){
		B_deck[ free_space + i ] = B_deck[ top_B + i ];
		len_B ++;
	}
	top_B += n_cards_ontable;


}

void print_deck(int deck[], int *top){
	// from deck top
	for ( int i=0 ; i<52 ; i++){
		printf("%d ", deck[ ( *top + i ) % 52 ] );
	}

}

int resolve(int A, int B, int A_deck[], int B_deck[], int max_conflicts, int simplified_mode){
	
	conflicts_cnt++;

	A=strength(A);
	B=strength(B);

	if ( A>B )
		one_takes( A_deck, B_deck, &top_A, &top_B, &len_A, &len_B );
	else if ( B>A )
		one_takes( B_deck, A_deck, &top_B, &top_A, &len_B, &len_A );
	else {
		// WAR
		if ( conflicts_cnt == max_conflicts ){
			both_take(A_deck, B_deck);
			return 0;
		}

		if ( simplified_mode == 0 ){
		
			if ( len_A < 2 || len_B < 2){
				//end on not enough cards
				both_take(A_deck,B_deck);
				return 1;
			}

			A=A_deck[ ( top_A + n_cards_ontable + 1 ) % NUMBER_OF_CARDS ];
			B=B_deck[ ( top_B + n_cards_ontable + 1 ) % NUMBER_OF_CARDS ];
			len_A -= 2;
			len_B -= 2;
			n_cards_ontable += 2;

			int result = resolve(A,B,A_deck,B_deck,max_conflicts,simplified_mode);

			if ( result == 1 ){
				// returned no cards left
				return 1;
			}
			else if ( result == 0 ){
				// returned max conficts reached
				return 0;
			}
		}

		else { // simplified_mode == 1
			both_take(A_deck,B_deck);
		}

	}

	return OK;

}

void game(int A_deck[], int B_deck[], int max_conflicts, int simplified_mode) {
	
	while (	1 ){
		int A=A_deck[top_A], B=B_deck[top_B];

		len_A--;
		len_B--;

		n_cards_ontable ++;

		int result = resolve(A,B,A_deck,B_deck,max_conflicts,simplified_mode);

		n_cards_ontable=0; 
		
		if ( result == 1 ){
			// no cards left
			printf("1 %d %d",len_A,len_B);
			return;
		}
		else if ( len_B == 0 ){
			// A wins
			printf("2 %d",conflicts_cnt);
			return;
		}
		else if ( len_A == 0 ){
			// B wins
			printf("3\n");
			print_deck(B_deck,&top_B);
			return;
		}
		else if ( conflicts_cnt == max_conflicts){
			// max conficts reached
			printf("0 %d %d",len_A,len_B);
			return;
		}
		

	}

	
}

// -- Module C - Init --

int main(void) {
	//
	int A_deck[NUMBER_OF_CARDS], B_deck[NUMBER_OF_CARDS];
	int deck[NUMBER_OF_CARDS];
	int simplified_mode;
	int max_conflicts;
	int seed;

	scanf("%d", &seed);
	scanf("%d", &simplified_mode);
	scanf("%d", &max_conflicts);

	// deck preparation and shuffling
	for (int i = 0; i < NUMBER_OF_CARDS; i++){
		deck[i] = i;
	} 
	srand((unsigned) seed);
	shuffle(deck, NUMBER_OF_CARDS);
	
	// handing out cards
	for(int i = 0; i < NUMBER_OF_CARDS / 2; i++) {
		A_deck[i] = deck[i];
		B_deck[i] = deck[i + NUMBER_OF_CARDS / 2];
	}

	// gameplay
	game(A_deck, B_deck, max_conflicts, simplified_mode);
	
	return 0;
}


//	end codes:
//	Max conficts reached: 0
//	No cards left: 1
//	A wins: 2
//	B wins: 3
