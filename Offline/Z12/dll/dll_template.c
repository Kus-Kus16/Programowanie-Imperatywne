#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// list node
typedef struct Node {
	int* data;
	size_t size;
	struct Node* next;
	struct Node* prev;
} Node;

// doubly linked list
typedef struct List {
	Node *head;
	Node *tail;
} List;

Node* create_node(int* data, size_t size, Node* prev, Node* next) {
	Node* new = malloc( sizeof(Node) );
	new -> data = data;
	new -> size = size;
	new -> next = next;
	new -> prev = prev;
	
	return new;
}

// initialize list
void init(List* list) {
	list -> head = NULL;
	list -> tail = NULL;
}

void free_node(Node* node) {
	if( node == NULL )
		return;
	
	if( node -> data != NULL )
		free( node -> data );

	free(node);
}

// push front
void push_front(List* list, int* data, size_t size) {
	Node* head = list -> head;
	Node* new = create_node(data, size, NULL, head);

	if( head != NULL )
		head -> prev = new;
	else
		list -> tail = new;

	list -> head = new;
}

// append element to the list
void push_back(List* list, int* data, size_t size) {
	Node* tail = list -> tail;
	Node* new = create_node(data, size, tail, NULL);
	
	if( tail != NULL )
		tail -> next = new;
	else
		list -> head = new;

	list -> tail = new;
}

// remove element from list front
int pop_front(List* list) {
	Node* old = list -> head;

	if( old == NULL )
		return 0;
	
	list -> head = old -> next;
	
	if( list -> head == NULL )
		list -> tail == NULL;
	else
		list -> head -> prev == NULL;

	free_node(old);
	return 1;
}

// remove element from list back
int pop_back(List* list) {
	Node* old = list -> tail;

	if( old == NULL )
		return 0;

	list -> tail = old -> prev;

	if( list -> tail == NULL )
		list -> head = NULL;
	else
		list -> tail -> next = NULL;

	free_node(old);
	return 1;
}

// forward iteration - get n-th cell in the list
int get_forward(List* list, size_t n) {
	Node* node = list -> head;

	while( n > node -> size ){
		n -= node -> size;
		node = node -> next;
	}

	return node -> data[n - 1];
}

// backward iteration - get n-th cell from the end of the list
int get_backward(List* list, size_t n) {
	Node* node = list -> tail;

	while( n > node -> size ){
		n -= node -> size;
		node = node -> prev;
	}

	return node -> data[node -> size - n];
}

// remove whole node
void remove_node(List* list, Node* node) {
	Node* prev = node -> prev;
	Node* next = node -> next;

	if( prev != NULL )
		prev -> next = node -> next;
	else
		list -> head = next;


	if( next != NULL )
		next -> prev = node -> prev;
	else
		list -> tail = prev;

	free_node(node);
}

// remove n-th cell; if array empty remove node
void remove_at(List* list, size_t n) {
	Node* node = list -> head;

	while( n > node -> size ){
		n -= node -> size;
		node = node -> next;
	}

	if( node -> size == 1 ){
		remove_node(list, node);
		return;
	}

	int* data = node -> data;
	memmove( node -> data + (n - 1), node -> data + n, (node -> size - n)*sizeof(int) );
	node -> size -= 1;
	realloc( node -> data, node -> size * sizeof(int) );

}

size_t digits(int n) {
	int cnt = 0;

	if( n == 0 )
		return 1;

	for( int x = abs(n) ; x > 0 ; cnt++ )
		x /= 10;

	return cnt;
}

void append_in_order(Node* node, int value){
	node -> data = realloc(node -> data, (node -> size + 1) * sizeof(int));
	int* array = node -> data;

	int i = node -> size;
	while( i > 0 && array[i - 1] > value ){
		array[i] = array[i - 1];
		i --;
	}
	
	array[i] = value;
	node -> size ++;
}

void insert_in_order(List* list, int value) {
	Node* node = list -> head;
	size_t d = digits(value);

	while( node != NULL && digits(node -> data[0]) < d ){
		node = node -> next;
	}

	if( node == NULL ){
		int* array = (int*) malloc(sizeof(int));
		array[0] = value;
		
		push_back(list, array, 1);
	}

	else if( digits(node -> data[0]) == d ){
		append_in_order(node, value);
	}

	else{ // new Node should be inserted before @node Node
		int* array = (int*) malloc(sizeof(int));
		array[0] = value;
		Node* new = create_node(array, 1, node -> prev, node);

		if( new -> prev != NULL )
			new -> prev -> next = new;
		else
			list -> head = new;

		if( new -> next != NULL )
			new -> next -> prev = new;
		else
			list -> tail = new;

	}

}

// -------------------------------------------------------------
// helper functions

// print list
void dumpList(const List* list) {
	for( Node* node = list -> head ; node != NULL ; node = node -> next ) {
		printf("-> ");

		for( int k = 0 ; k < node -> size ; k++ ){
			printf("%d ", node -> data[k]);
		}

		printf("\n");
	}
}

// free list
void freeList(List* list) {
	while( pop_front(list) );
}

// read int vector
void read_vector(int array[], size_t n) {
	for( size_t i = 0 ; i < n ; i++ ) { 
		scanf("%d", array + i);
	}
}

// initialize the list and push data
void read_list(List* list) {
	int n;
	size_t size;
	scanf("%d", &n);

	for( int i = 0 ; i < n ; i++ ) {
		scanf("%zu", &size);
		int* array = (int*) malloc(size * sizeof(int));

		read_vector(array, size);
		push_back(list, array, size);
	}
}

int main(void) {
	int to_do, value;
	size_t size, offset;
	List list;
	init(&list);

	scanf("%d", &to_do);
	switch (to_do) {
		case 1:
			read_list(&list);
			dumpList(&list);
			break;
		case 2:
			read_list(&list);
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%zu", &offset);
				printf("%d ", get_forward(&list, offset));
			}
			printf("\n");
			break;
		case 3:
			read_list(&list);
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%zu", &offset);
				printf("%d ", get_backward(&list, offset));
			}
			printf("\n");
			break;
		case 4:
			read_list(&list);
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%zu", &offset);
				remove_at(&list, offset);
			}
			dumpList(&list);
			break;
		case 5:
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%d", &value);
				insert_in_order(&list, value);
			}
			dumpList(&list);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}

	freeList(&list);
	return 0;
}

