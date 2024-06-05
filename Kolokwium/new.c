typedef struct tagListElement {
    struct tagListElement* next;
    void* data;
}ListElement;

typedef void (*ConstDataFp)(const void*);
typedef void (*DataFp)(void*);
typedef int (*CompareDataFp)(const void*,const void*);

typedef struct tagList {
    ListElement* head;
    ListElement* tail;
    int size;
    ConstDataFp dumpData;
    DataFp freeData;
    CompareDataFp compareData;
} List;

typedef struct{
    double* array;
    size_t size;
} vector;

vector* vector_copy(const vector* src) {
    vector* new = (vector*) malloc(sizeof(vector));

    new -> size = src -> size;
    new -> array = (double*) malloc( (src -> size)*sizeof(double) );
    
    memcpy(new -> array, src -> array, (src -> size)*sizeof(double) );
    return new;
}

void add_vector_to_list(List* list, vector* v) {
    ListElement* new = (ListElement*) malloc(sizeof(ListElement));

    new -> data = v;
    new -> next = NULL;

    if (list -> tail) {
        list -> tail -> next = new;
    }
    else {
        list -> head = new;
    }

    list -> tail = new;
    list -> size++;

}

int copy_vector_list(List* target, const List* src, int (*fp)(const vector* v)) {
    int cnt = 0;
    ListElement* current = src -> head;

    while (current) {
        vector* vec = (vector*) current -> data;

        if ( fp(vec) ) {
            vector* copied = copy_vector(vec);
            add_vector_to_list(target, copied)
            cnt++;
        }
        
        current = current -> next;
    }

    return cnt;
}

// Prykladowa funkcja predykatowa
int pfunction(const vector* v) {
    return v -> size < 10;
}





int count_even(vector* v) {
    int count = 0;

    for (size_t i = 0 ; i < v -> size ; ++i) {
        if ((int)v -> array[i] % 2 == 0) {
            count++;
        }
    }

    return count;
}

int vectors_even_cmp(void* a, void* b) {
    vector* v1 = *(vector**)a;
    vector* v2 = *(vector**)b;

    int c1 = count_even(v1);
    int c2 = count_even(v2);

    return c1 - c2;
}

void sort_vector_list(List* target, const List* src) {
    vector** vectors = (vector**) malloc( (src -> size)*sizeof(vector*) );

    ListElement* current = src -> head;
    int i = 0;
    while (current) {
        vector* copied_vector = vector_copy( (vector*)current -> data );
        vectors[i++] = copied_vector;

        current = current -> next;
    }

    qsort(vectors, src -> size, sizeof(vector*), vectors_even_cmp);

    for (int j = 0 ; j < src -> size ; ++j) {
        add_vector_to_list(target, vectors[j]);
    }

    free(vectors);
}




void absolute_values(List* list) {
    ListElement* current = list -> head;

    while (current) {
        vector* vec = (vector*) current -> data;

        for (size_t i = 0 ; i < vec -> size ; ++i) {
            vec -> array[i] = fabs(vec -> array[i]);
        }

        current = current -> next;
    }
}






void add_vector(List* list, const double* array, size_t size) {

    vector* new = (vector*) malloc( sizeof(vector) );
    new -> array = (double*) malloc( size*sizeof(double) );

    memcpy( new -> array, array, size*sizeof(double) );
    new -> size = size;

    add_vector_to_list(list, new);

}






double find_min_value(const vector* v) {
    double min_value = DBL_MAX;

    for (size_t i = 0 ; i < v -> size ; ++i) {
        if (v -> array[i] < min_value) {
            min_value = v -> array[i];
        }
    }
    return min_value;
}

vector min_values(const List* list) {
    vector min_vector;
    min_vector.size = list -> size;
    min_vector.array = (double*) malloc( (min_vector.size)*sizeof(double) );

    ListElement* current = list -> head;
    size_t i = 0;

    while (current) {
        vector* v = (vector*) current -> data;

        double min_value = find_min_value(v);
        min_vector.array[i++] = min_value;

        current = current -> next;
    }

    return min_vector;
}