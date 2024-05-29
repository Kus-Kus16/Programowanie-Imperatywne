#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

typedef void (*DataFp) (void*);
typedef void (*ConstDataFp) (const void*);
typedef int (*CompareDataFp) (const void*, const void*);

typedef struct ListElement {
	struct ListElement* next;
	void* data;
} ListElement;

typedef struct {
	ListElement* head;
	ListElement* tail;
	ConstDataFp dump_data;
	DataFp free_data;
	CompareDataFp compare_data;
	DataFp modify_data;
} List;

// --- generic functions --- for any data type

void init_list(List* p_list, ConstDataFp d_data, DataFp f_data, CompareDataFp c_data, DataFp m_data) {
	p_list -> head = NULL;
	p_list -> tail = NULL;
	p_list -> dump_data = d_data;
	p_list -> free_data = f_data;
	p_list -> compare_data = c_data;
	p_list -> modify_data = m_data;
}

// Print elements of the list
void dump_list(const List* p_list) {
	ListElement* p = p_list -> head;
	ConstDataFp dump = p_list -> dump_data;

	if( dump == NULL)
		return;

	while(p){
		dump(p -> data);
		p = p -> next;
	}

}

// Print elements of the list if comparable to data
void dump_list_if(List* p_list, void* data) {
	ListElement* p = p_list -> head;
	ConstDataFp dump = p_list -> dump_data;
	CompareDataFp cmp = p_list -> compare_data;

	if( dump == NULL || cmp == NULL )
		return;

	while(p){
		if( cmp(p -> data, data) == 0 )
			dump(p -> data);
		
		p = p -> next;
	}

}

// Free element pointed by data using free_data() function
void free_element(DataFp free_data, ListElement* to_delete) {
	free_data(to_delete);
}

// Free all elements of the list
void free_list(List* p_list) {
	ListElement* p = p_list -> head;
	DataFp del = p_list -> free_data;

	while(p){
		ListElement* tmp = p;
		p = p -> next;
		del(tmp);
	}

	p_list -> head = NULL;
	p_list -> tail = NULL;

}

// Push element at the beginning of the list
void push_front(List* p_list, void* data) {
	ListElement* new = malloc( sizeof(ListElement) );
	new -> next = p_list -> head;
	new -> data = data;

	p_list -> head = new;
	if( p_list -> tail == NULL )
		p_list -> tail = new;

}

// Push element at the end of the list
void push_back(List* p_list, void* data) {
	ListElement* new = malloc( sizeof(ListElement) );
	new -> next = NULL;
	new -> data = data;

	if( p_list -> tail == NULL )
		p_list -> head = new;
	else 
		p_list -> tail -> next = new;

	p_list -> tail = new;

}

// Remove the first element
void pop_front(List* p_list) {
	ListElement* p = p_list -> head;

	if( p == NULL)
		return;
	if( p_list -> head == p_list -> tail )
		p_list -> tail = NULL;

	p_list -> head = p_list -> head -> next;
	free_element(p_list -> free_data, p);
}

// Reverse the list
void reverse(List* p_list) {
	ListElement* curr = p_list -> head;
	ListElement* prev = NULL;
	ListElement* next = NULL;

	if( curr == NULL )
		return;

	p_list -> tail = p_list -> head;
	while( curr -> next != NULL){
		next = curr -> next;
		curr -> next = prev;

		prev = curr;
		curr = next;
	}
	curr -> next = prev;
	p_list -> head = curr;

}

// find element in sorted list after which to insert given element
ListElement* find_insertion_point(const List* p_list, ListElement* new) {
	ListElement* p = p_list -> head;
	ListElement* q = NULL;
	CompareDataFp cmp = p_list -> compare_data;

	while( p != NULL){
		int res = cmp(p -> data, new -> data);

		if( res > 0 )
			break;

		q = p;
		p = p -> next;

	}
	

	return q;

}

// Insert element after 'previous'
void push_after(List* p_list, ListElement* previous, ListElement* new) {

	if( previous == NULL ){
		new -> next = p_list -> head;
		p_list -> head = new;
	}
	else{
		new -> next = previous -> next;
		previous -> next = new;
	}

	if( p_list -> tail == previous )
		p_list -> tail = new;


}


// Insert element preserving order
void insert_in_order(List* p_list, void* data) {;
	ListElement* new = malloc( sizeof(ListElement) );
	DataFp mod = p_list -> modify_data;
	CompareDataFp cmp = p_list -> compare_data;

	new -> data = data;

	ListElement* location = find_insertion_point(p_list, new);

	if( location != NULL && cmp( location -> data, new -> data) == 0 ){
		if( mod != NULL )
			mod(location -> data);
		return;
	}

	push_after(p_list, location, new);

}

// -----------------------------------------------------------
// --- type-specific definitions

// int element

void dump_int(const void* data) {
	int* a =  (int*)data;
	printf("%d ", *a);
}

void free_int(void* data) {
	int* a = (int*)data;
	free(a);
}

int cmp_int(const void* d1, const void* d2) {
	int a = *( (int*)d1 );
	int b = *( (int*)d2 );

	if( a < b )
		return -1;
	if( b < a )
		return 1;
	else
		return 0;

}

int* create_data_int(int v) {
	int* new = malloc( sizeof(int) );
	*new = v;
	return new;
}

// Word element


typedef struct DataWord {
	char* word;
	int counter;
} DataWord;


void dump_word(const void* data) {
	DataWord* a = (DataWord*)data;
	printf("%s ",a -> word);
}

void dump_word_lowercase(const void* data) {
	DataWord* a = (DataWord*)data;
	char* word = a -> word;

	for( int i = 0 ; word[i] != '\0' ; i++ ){
		word[i] = tolower(word[i]);
	}
	
	dump_word(data);

}

void free_word(void* data) {
	char* a = (char*)data;
	free(a);
}

int cmp_word_alphabet(const void* data1, const void* data2) {
	DataWord* a = (DataWord*)data1;
	DataWord* b = (DataWord*)data2;

	return strcmp(a -> word, b -> word);

}

int cmp_word_counter(const void* data1, const void* data2) {
	DataWord* alpha = (DataWord*)data1;
	DataWord* bravo = (DataWord*)data2;

	int a = alpha -> counter;
	int b = bravo -> counter;

	if( a < b )
		return -1;
	if( b < a)
		return 1;
	else
		return 0;

}

void modify_word(void* data) {
	DataWord* a = (DataWord*)data;
	a -> counter += 1;
}

DataWord* create_data_word(const char* string, int counter) {
	DataWord* new = malloc( sizeof(DataWord) );
	new -> counter = counter;
	new -> word = strdup(string);

	return new;
}

void lowercase_word(char* word){
	for( int i = 0 ; word[i] != '\0' ; i++ ){
		word[i] = tolower(word[i]);
	}
}

// read text, parse it to words, and insert those words to the list.
// Order of insertions is given by the last parameter of type CompareDataFp.
// (comparator function address). If this address is not NULL the element is
// inserted according to the comparator. Otherwise, read order is preserved.
void stream_to_list(List* p_list, FILE* stream, CompareDataFp cmp) {
	const char delimits[] = " \r\t\n.,?!:;-";
	char buff[BUFFER_SIZE] = {0};
	char* tmp;

	while( fgets(buff, BUFFER_SIZE, stream) ){
		tmp = strtok(buff, delimits);
		while(tmp){
			DataWord* new = create_data_word(tmp, 1);

			if( cmp != NULL ){
				lowercase_word(new -> word);
				insert_in_order(p_list, new);
			}
			else
				push_back(p_list, new);

			tmp = strtok(NULL, delimits);

		}

	}

}

// test integer list
void list_test(List *p_list, int n) {
	char op;
	int v;
	for (int i = 0; i < n; ++i) {
		scanf(" %c", &op);
		switch (op) {
			case 'f':
				scanf("%d", &v);
				push_front(p_list, create_data_int(v));
				break;
			case 'b':
				scanf("%d", &v);
				push_back(p_list, create_data_int(v));
				break;
			case 'd':
				pop_front(p_list);
				break;
			case 'r':
				reverse(p_list);
				break;
			case 'i':
				scanf("%d", &v);
				insert_in_order(p_list, create_data_int(v));
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
		//dump_list(p_list); // testing
	}
}

int main(void) {
	int to_do, n;
	List list;

	scanf ("%d", &to_do);
	switch (to_do) {
		case 1: // test integer list
			scanf("%d",&n);
			init_list(&list, dump_int, free_int, cmp_int, NULL);
			list_test(&list, n);
			dump_list(&list);
			free_list(&list);
			break;
		case 2: // read words from text, insert into list, and print
			init_list(&list, dump_word, free_word, NULL, NULL);
			stream_to_list(&list, stdin, NULL);
			dump_list(&list);
			free_list(&list);
			break;
		case 3: // read words, insert into list alphabetically, print words encountered n times
			scanf("%d",&n);
			init_list(&list, dump_word, free_word, cmp_word_alphabet, modify_word); 
			stream_to_list(&list, stdin, cmp_word_alphabet);
			list.compare_data = cmp_word_counter;
			DataWord data = { NULL, n };
			dump_list_if(&list, &data);
			free_list(&list);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

