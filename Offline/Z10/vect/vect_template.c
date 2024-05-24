#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_STR_LEN 64

#pragma GCC diagnostic error "-Wpointer-arith"

typedef struct Vector {
	void* data;
	size_t element_size; // size of 1 element
	size_t size;		 // stored elements cnt
	size_t capacity;	 // max stored elements
} Vector;

// function pointers defs:

typedef int(* cmp_ptr)(const void*, const void*);
typedef int(* predicate_ptr)(void*);
typedef void(* read_ptr)(void*);
typedef void(* print_ptr)(const void*);

// Allocate vector to initial capacity (block_size elements),
// Set element_size, size (to 0), capacity
void init_vector(Vector* vector, size_t b_size, size_t e_size) {
	vector -> element_size = e_size;
	vector -> size = 0;
	vector -> capacity = b_size; // ? IF cap == reserved space then b_size * e_size
	vector -> data = malloc( b_size * e_size );
}

// If new_capacity is greater than the current capacity,
// new data is allocated, otherwise the function does nothing.
void reserve(Vector* vector, size_t new_capacity) {
	if( vector -> capacity < new_capacity ){		
		size_t e_size = vector -> element_size;
		
		vector -> data = realloc( vector -> data, new_capacity * e_size );
		vector -> capacity = new_capacity;
	}
}

// Resizes the vector to contain new_size elements.
// If the current size is greater than new_size, the container is
// reduced to its first new_size elements.

// If the current size is less than new_size,
// additional zero-initialized elements are appended
void resize(Vector* vector, size_t new_size) {
	size_t e_size = vector -> element_size;
	size_t old_size = vector -> size;
	reserve(vector, new_size);
	
	if( old_size < new_size ){
		//for( int i = old_size ; i < new_size ; i ++ ):
		//	(vector -> data)[i] = 0;

		memset( (char*) vector -> data + old_size * e_size, 0, (new_size - old_size) * e_size );
	}


	vector -> size = new_size;

}

// Add element to the end of the vector
void push_back(Vector* vector, void* value) {
	size_t size = vector -> size;
	size_t capacity = vector -> capacity;
	size_t e_size = vector -> element_size;
	
	if( size == capacity ){
		reserve(vector, capacity * 2);
	}

	memcpy( (char*) vector -> data + size * e_size, value, e_size );
	(vector -> size) ++;

}

// Remove all elements from the vector
void clear(Vector* vector) {
	free( vector -> data );
	init_vector( vector, 1, vector -> element_size );
}

// Insert new element at index (0 <= index <= size) position
void insert(Vector* vector, size_t index, void *value) {
	size_t size = vector -> size;
	size_t capacity = vector -> capacity;
	size_t e_size = vector -> element_size;

	if( size == capacity ){
		reserve( vector, capacity * 2 );
		capacity *= 2;
	}

	// make space
	memmove( (char*) vector -> data + (index + 1) * e_size, (char*) vector -> data + index * e_size, (size - index) * e_size );
	// cpy new val
	memcpy( (char*) vector -> data + index * e_size, value, e_size );
	(vector -> size) ++;

}

// Erase element at position index
// 0 <= index < size
void erase(Vector* vector, size_t index) {
	size_t size = vector -> size;
	size_t e_size = vector -> element_size;

	memmove( (char*) vector -> data + index * e_size, (char*) vector -> data + (index + 1) * e_size, (size - index - 1) * e_size );
	(vector -> size) --;
}

// Erase all elements that compare equal to value from the container
void erase_value(Vector* vector, void* value, cmp_ptr cmp) {
	size_t e_size = vector -> element_size;
	size_t size = vector -> size;

	for( size_t i = 0 ; i < size ; i++ ){
		void* ptr = (char*) vector -> data + i * e_size;
		
		if( cmp(ptr, value) == 0 ){
			erase(vector, i);
			i --;
		}
	}

}

// Erase all elements that satisfy the predicate from the vector
void erase_if(Vector* vector, int (* predicate)(void *)) {
	size_t e_size = vector -> element_size;
	size_t size = vector -> size;

	for( size_t i = 0 ; i < size ; i++ ){
		void* ptr = (char*) vector -> data + i * e_size;
		
		if( predicate(ptr) ){
			erase(vector, i);
			i --;
		}
	}
}

// Request the removal of unused capacity
void shrink_to_fit(Vector* vector) {
	size_t size = vector -> size;
	size_t e_size = vector -> element_size;
	if( vector -> capacity != size ){
		vector -> data = realloc( vector -> data, size * e_size );
		vector -> capacity = size;
	}
}

// ---------------------------------------------------

typedef struct {
	int age;
	char first_name[MAX_STR_LEN];
	char last_name[MAX_STR_LEN];
} Person;

void print_int(const void* v) {
	printf("%d ", *( (int*)v ));
}

void print_char(const void* v) {
	printf("%c ", *( (char*)v ));
}

void print_person(const void* v) {
	Person p = *( (Person*)v );
	printf("%d %s %s\n", p.age, p.first_name, p.last_name);
}

void print_vector(Vector* vector, print_ptr print) {
	size_t capacity = vector -> capacity;
	size_t size = vector -> size;
	size_t e_size = vector -> element_size;

	printf("%d\n", capacity);

	for( int i=0 ; i < size ; i++ ){
		void* ptr = (char*) vector -> data + i * e_size;
		print(ptr);
	}

}

int int_cmp(const void* v1, const void* v2) {
	int a = *( (int*)v1 );
	int b = *( (int*)v2 );

	if( a < b )
		return -1;
	if( b < a )
		return 1;
	else
		return 0;
	

}

int char_cmp(const void* v1, const void* v2) {
	char a = *( (char*)v1 );
	char b = *( (char*)v2 );

	if( a < b )
		return -1;
	if( b < a )
		return 1;
	else 
		return 0;

}

int person_cmp(const void* p1, const void* p2) {
	Person a = *( (Person*)p1 );
	Person b = *( (Person*)p2 );
	int res;

	if( a.age > b.age )
		return -1;
	if( b.age > a.age )
		return 1;
	
	res = strcmp(a.first_name, b.first_name);
	if( res < 0 )
		return -1;
	if( res > 0 )
		return 1;

	res = strcmp(a.last_name, b.last_name);
	if( res < 0 )
		return -1;
	if( res > 0 )
		return 1;
	else
		return 0;

}

int is_even(void* value) {
	int v = *( (int*)value );
	return v % 2 == 0;
}

int is_vowel(void* value) {
	char c = *( (char*)value );
	c = toupper(c);
	return c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U' || c == 'Y'; 
}

int is_older_than_25(void* person) {
	Person p = *( (Person*)person );
	return p.age > 25;
}

void read_int(void* value) {
	scanf("%d",value);
}

void read_char(void* value) {
	scanf(" %c",value);
}

void read_person(void* value) {
	Person* p = (Person*)value;
	scanf("%d %s %s", &p -> age, &p -> first_name, &p -> last_name);
}

void vector_test(Vector* vector, size_t block_size, size_t elem_size, int n, read_ptr read, cmp_ptr cmp, predicate_ptr predicate, print_ptr print) {
	init_vector(vector, block_size, elem_size);
	void* v = malloc(vector->element_size);
	size_t index, size;
	for (int i = 0; i < n; ++i) {
		char op;
		scanf(" %c", &op);
		switch (op) {
			case 'p': // push_back
				read(v);
				push_back(vector, v);
				break;
			case 'i': // insert
				scanf("%zu", &index);
				read(v);
				insert(vector, index, v);
				break;
			case 'e': // erase
				scanf("%zu", &index);
				erase(vector, index);
				break;
			case 'v': // erase
				read(v);
				erase_value(vector, v, cmp);
				break;
			case 'd': // erase (predicate)
				erase_if(vector, predicate);
				break;
			case 'r': // resize
				scanf("%zu", &size);
				resize(vector, size);
				break;
			case 'c': // clear
				clear(vector);
				break;
			case 'f': // shrink
				shrink_to_fit(vector);
				break;
			case 's': // sort
				qsort(vector->data, vector->size,
				      vector->element_size, cmp);
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
	print_vector(vector, print);
	free(vector->data);
	free(v);
}

int main(void) {
	int to_do, n;
	Vector vector_int, vector_char, vector_person;

	scanf("%d%d", &to_do, &n);

	switch (to_do) {
		case 1:
			vector_test(&vector_int, 4, sizeof(int), n, read_int, int_cmp, is_even, print_int);
			break;
		case 2:
			vector_test(&vector_char, 2, sizeof(char), n, read_char, char_cmp, is_vowel, print_char);
			break;
		case 3:
			vector_test(&vector_person, 2, sizeof(Person), n, read_person, person_cmp, is_older_than_25, print_person);
			break;
		default:
			printf("Nothing to do for %d\n", to_do);
			break;
	}

	return 0;
}

