#include <stdio.h>

enum state { OK = 0, UNDERFLOW = -1, OVERFLOW = -2 };

// -- 1 -- stos
#define STACK_SIZE 10

int stack[STACK_SIZE];
int top = 0;

int stack_push(int x) {

	if ( top < STACK_SIZE ){
		stack[top]=x;
		top ++;
		return OK;
	}
	else 
		return OVERFLOW;

}

int stack_pop(void) {

	if ( top > 0 ){
		top --;
		return stack[top];
	}
	else
		return UNDERFLOW;

}

int stack_state(void) {

	return top;

}

// -- 2 -- kolejka
// FIFO queue with shifts
#define QUEUE_SIZE 10

int queue[QUEUE_SIZE];
int in = 0, curr_nr = 0; // in - number of clients, curr_nr - last assigned number // in-1 - last used index

int queue_push(int in_nr) { // in_nr clients try to enter the queue
	
	for ( int i=0 ; i<in_nr ; i++ ){

		if ( in<QUEUE_SIZE ){ //available space
			curr_nr ++;
			queue[in]=curr_nr;
			in++;
		}
		else{ //not enough space
			curr_nr += in_nr-i;
			return OVERFLOW;
		}

	}
	
	return OK;

}

int queue_pop(int out_nr) { // out_nr clients leaves the queue

	for ( int i=0 ; i<out_nr; i++ ){

		if ( in != 0 ){
			for ( int j=0 ; j<in-1 ; j++){
				queue[j]=queue[j+1];
			}
			in --;
		}
		else
			return UNDERFLOW;

	}

	return in;
	
}

int queue_state(void) {

	return in;

}

void queue_print(void) {

	for ( int i=0 ; i<in ; i++ ){
		printf("%d ",queue[i]);
	}

}

// -- 3 --
// Queue with cyclic buffer
#define CBUFF_SIZE 10

int cbuff[CBUFF_SIZE];
int out = 0, len = 0; // out - first to go index, len - clients count


int cbuff_push(int cli_nr) { // client with number cli_nr enters the queue

	if ( len < CBUFF_SIZE ){
		int free_i= (out+len) % CBUFF_SIZE;
		cbuff[free_i]=cli_nr;
		len ++;
		return OK;
	}
	else
		return OVERFLOW;

}

int cbuff_pop(void) { // longest waiting client leaves the queue

	if ( len > 0){
		int i = out;
		out = (out+1) % CBUFF_SIZE;
		len --;
		return cbuff[i];
	}
	else
		return UNDERFLOW;

}

int cbuff_state(void) {

	return len;

}

void cbuff_print(void) {

	int i=out;
	for ( int cnt=0 ; cnt<len ; cnt++ ){
		printf("%d ",cbuff[i]);
		i = (i+1) % CBUFF_SIZE;
	}

}

int main(void) {
	int to_do, n, client_no, answer;
	scanf("%d", &to_do);
	switch(to_do) {
		case 1: // stack
			do {
				scanf("%d", &n);
				if (n > 0) {
					if ((answer = stack_push(n)) < 0) printf("%d ", answer);
				} else if (n < 0) {
					printf("%d ", stack_pop());
				} else printf("\n%d\n", stack_state());
			} while(n != 0);
			break;
		case 2: // FIFO queue with shifts
			do {
				scanf("%d", &n);
				if (n > 0) {
					if ((answer = queue_push(n)) < 0) printf("%d ", answer);
				} else if (n < 0) {
					if ((answer = queue_pop(-n)) < 0) printf("%d ", answer);
				} else {
					printf("\n%d\n", queue_state());
					queue_print();
				}
			} while(n != 0);
			break;
		case 3: // queue with cyclic buffer
			client_no = 0;
			do {
				scanf("%d", &n);
				if (n > 0) {
					if ((answer = cbuff_push(++client_no)) < 0) printf("%d ", answer);
				} else if (n < 0) {
					printf("%d ", cbuff_pop());
				} else {
					printf("\n%d\n", cbuff_state());
					cbuff_print();
				}
			} while(n != 0);
			break;
		default: 
			printf("NOTHING TO DO!\n");
	}
	return 0;
}
