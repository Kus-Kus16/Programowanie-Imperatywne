#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct _matrix{
    int rows;
    int cols;
    double** val;
    double* storage;
} Matrix;

int create_matrix(Matrix* pmatrix, int rows, int cols){
    int size = rows*cols;
    pmatrix -> rows = rows;
    pmatrix -> cols = cols;
    pmatrix -> val = (double**) malloc( sizeof(double*)*rows );
    pmatrix -> storage = (double*) malloc( sizeof(double)*size );
    
    memset( pmatrix -> storage, 0, sizeof(double)*size );
    for( int i = 0 ; i < rows ; i++ ){
        pmatrix -> val[i] = pmatrix -> storage + i*cols;
    }


    return 1;
}

Matrix random_matrix(int rows, int cols){
    Matrix m;
    create_matrix(&m, rows, cols);

    srand(time(NULL));
    for( int w = 0 ; w < rows ; w++ ){
        for( int k = 0 ; k < cols ; k++ ){
            m.val[w][k] = (double)rand() / RAND_MAX;
        }
    }
    
    return m;
}

void copy_matrix(Matrix* ptgt, const Matrix* psrc){
    int rows = psrc -> rows;
    int cols = psrc -> cols;
    create_matrix(ptgt, rows, cols);
    
    for ( int w = 0 ; w < rows ; w++ ){
        for ( int k = 0 ; k < cols ; k++ ){
            ptgt -> val[w][k] = psrc -> val[w][k];
        }


    }
}

void multiply_by(Matrix* pm, double v){
    for( int w = 0 ; w < pm -> rows ; w++ ){
        for( int k = 0 ; k < pm -> cols ; k++ ){
            pm -> val[w][k] *= v;
        }
    }

}

int add(Matrix* presult, const Matrix* pm1, const Matrix* pm2){
    int rows = pm1 -> rows;
    int cols = pm1 -> cols;
    if( rows != pm2 -> rows || cols != pm2 -> cols )
        return 0;

    create_matrix(presult, rows, cols);
    for( int w = 0 ; w < rows ; w++ ){
        for( int k = 0 ; k < cols ; k++){
            presult -> val[w][k] = pm1 -> val[w][k] + pm2 -> val[w][k];
        }
    }

    return 1;
}

enum Vehicle_type {truck, car, v_ndef};

typedef struct{
    float load;
    short axes;
    int trailer_mass;
} Truck_data;

typedef struct{
    char vehicle_ID[15];
    unsigned short places : 3;
    unsigned short driving_wheels : 3;
    unsigned short towbar : 1; 

} Car_data;

typedef struct{
    int d;
    int m;
    int y;
} Date;

typedef struct{
    char owner[30];
    Date valid_date;
    enum Vehicle_type v_type;
    union { Car_data car; Truck_data truck} veh_union;
} Vehicle;

int date_compare(Date* d1, Date* d2){
    if(d1 -> y < d2 -> y)
        return -1;
    if(d1 -> y > d2 -> y)
        return 1;

    if(d1 -> m < d2 -> m)
        return -1;
    if(d1 -> m > d2 -> m )
        return 1;

    if(d1 -> d < d2 -> d)
        return -1;
    if(d1 -> d > d2 -> d)
        return 1;

    return 0;
}

int delayed(const Vehicle* tv, int size, const Date* base_date, int** pdelayed_owners){
    int cnt = 0;
    int* array = (int*) malloc(size*sizeof(int));

    for( int i = 0 ; i < size ; i++ ){
        if( date_compare(tv[i] -> valid_date, base_date) < 0 ){
            array[cnt] = i;
            cnt ++;
        }
    }

    if( cnt == 0 ){
        *(pdelayed_owners) = 0;
        free(array)
    }
    else{
        array = realloc(array, cnt*sizeof(int));
        *(pdelayed_owners) = array;
    }

    return cnt;

}

void print_owners(const Vehicle* tv, int* t_ind, size_t size){
    for( size_t i = 0 ; i < size ; i++ ){
        int index = t_ind[i];
        printf("%s\n", tv[index].owner);
    }
}

Vehicle* create_vehicle(void){
    Vehicle* new = (Vehicle*) malloc(sizeof(Vehicle));
    new -> valid_date.d = 1;
    new -> valid_date.m = 1;
    new -> valid_date.y = 1970;
    strcpy(new -> owner, "");
    new -> v_type = v_ndef

    return new;
}

void move_date(Vehicle* v, int months){
    v -> valid_date.m += months;
    while( v -> valid_date.m > 12 ){
        v -> valid_date.m -= 12;
        v -> valid_date.y += 1;
    }
}

void print_v_data(const Vehicle* v){
    switch(v -> vtype){
        case car:
            printf("Car: ID: %s, miejsc: %d, naped na: %d kola, hak: %s.",
            v -> veh_union.car.vehicle_ID,
            v -> veh_union.car.places,
            v -> veh_union.car.driving_wheels,
            v -> veh_union.car.towbar ? "TAK" : "NIE");
            break;
        case truck
            printf("Truck: load: %f axes: %d trailer_mass: %d",
            v -> veh_union.truck.load,
            v -> veh_union.truck.axes,
            v -> veh_union.truck.trailer_mass);
            break;
    }
    
}
