#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

// 1
////////////////////////////////////////////////////////////

int get(int cols, int row, int col, const int* A) {
		
	// all count from 0
	int i = cols*row + col;
	return A[i];

}

void set(int cols, int row, int col, int* A, int value) {
	
	// all count from 0
	int i = cols*row + col;
	A[i] = value;

}

void prod_mat(int rowsA, int colsA, int colsB, int* A, int* B, int* AB) {
	//					 == rowsB
	// final matrix: rowsA x colsB
	for(int w = 0 ; w < rowsA ; w++){
		for(int k = 0 ; k < colsB ; k++){
			
			int value = 0;
			for(int i = 0 ; i < colsA ; i++){
				value += get(colsA, w, i, A) * get(colsB, i, k, B);
			}

			set(colsB, w, k, AB, value);
		}
	}

}

void read_mat(int rows, int cols, int* M) {
	
	for(int w = 0 ; w < rows ; w++){
		for(int k = 0 ; k < cols ; k++){
			int value;
			scanf("%d", &value);
			set(cols, w, k, M, value);
		}
	}

}

void print_mat(int rows, int cols, int* M) {
	
	for(int w = 0 ; w < rows ; w++){
		for(int k = 0 ; k < cols ; k++){
			int value = get(cols, w, k, M);
			printf("%d ",value);
		}
		printf("\n");
	}

}

// 2
/////////////////////////////////////////////////////////////

int* read_line_cont(int* elements_cnt) {
	int* line = NULL;
	*elements_cnt = 0;

	char buff[BUF_SIZE] = {0};
	int buff_cnt = 0;

	char c = fgetc(stdin);
	while( !feof(stdin) && c != '\n' ){
		if( c == ' ' ){
			int num = atoi(buff);

			line = realloc(line, (*elements_cnt + 1)*sizeof(int) );
			line[*elements_cnt] = num;
			(*elements_cnt)++;

			memset(buff,0,BUF_SIZE);
			buff_cnt = 0;
		}
		else {
			buff[buff_cnt] = c;
			buff_cnt ++;
		}

		c = fgetc(stdin);

	}

	if(buff_cnt == 0)
		return NULL;

	int num = atoi(buff);
	line = realloc(line, (*elements_cnt + 1)*sizeof(int) );
	line[*elements_cnt] = num;
	(*elements_cnt)++;


	return line;
}

int read_int_lines_cont(int* ptr_array[]) {
	int lines_cnt = 0;
	int elements_cnt = 0;

	int* tab = ptr_array[0];

	while( !feof(stdin) ){
		int line_elements_cnt = 0;
		
		int* line = read_line_cont(&line_elements_cnt);
		if(line == NULL)
			break;

		memcpy(&tab[elements_cnt], line, sizeof(int)*line_elements_cnt );
		free(line);
		
		ptr_array[lines_cnt] = &tab[elements_cnt];

		elements_cnt += line_elements_cnt;
		lines_cnt++;
	}

	ptr_array[lines_cnt] = ptr_array[0] + elements_cnt;
	return lines_cnt;

}

void write_int_line_cont(int *ptr_array[], int n) {
	for( int* ptr = ptr_array[n] ; ptr < ptr_array[n+1] ; ptr++ ){
		printf("%d ",*ptr);
	}
}

// 3
///////////////////////////////////////////////////////////

int read_char_lines(char *array[]) {
	int lines_cnt = 0;
	char buff[BUF_SIZE] = {0};

	while( !feof(stdin) ){
		fgets(buff, BUF_SIZE, stdin);
		int buff_cnt = strlen(buff);

		array[lines_cnt] = malloc( (buff_cnt + 1)*sizeof(char) );
		memcpy(array[lines_cnt], buff, buff_cnt + 1);


		lines_cnt++;
	}

	array[lines_cnt] = NULL;
	return lines_cnt;

}

void write_char_line(char *array[], int n) {
	printf("%s",array[n]);
}

void delete_lines(char *array[]) {
	
	for(int i = 0 ; array[i] ; i++){
		free( array[i] );
	}

}


// 4
////////////////////////////////////////////////

typedef struct {
	int* values;
	int len;
	double average;
} line_type;

int read_int_lines(line_type lines_array[]) {

	int lines_cnt = 0;

	while( !feof(stdin) ){
		int line_elements_cnt = 0;
		
		int* line = read_line_cont(&line_elements_cnt);
		if(line == NULL)
			break;

		lines_array[lines_cnt].values = malloc( line_elements_cnt*sizeof(int) );
		lines_array[lines_cnt].len = line_elements_cnt;
		
		double average_tmp = 0;
		for (int i = 0 ; i < line_elements_cnt ; i++){
			average_tmp += line[i];
			lines_array[lines_cnt].values[i] = line[i];
		}
		lines_array[lines_cnt].average = average_tmp / line_elements_cnt;
		
				
		free(line);
		lines_cnt++;
	}

	return lines_cnt;

}

void write_int_line(line_type lines_array[], int n) {
	// print whole line, then average
	for (int i = 0; i < lines_array[n].len ; i++){
		printf("%d ", lines_array[n].values[i]);
	}
	printf("\n%.2f",lines_array[n].average);
}

void delete_int_lines(line_type array[], int lines_cnt) {
	for (int i = 0 ; i < lines_cnt ; i++){
		free(array[i].values);
	}
}

int cmp (const void *lt1, const void *lt2) {
	
	line_type a = *( (line_type*)lt1 );
	line_type b = *( (line_type*)lt2 );

	if( a.average < b.average )
		return -1;
	if( b.average < a.average )
		return 1;
	else
		return 0;

}

void sort_by_average(line_type lines_array[], int lines_cnt) {
	qsort(lines_array, lines_cnt, sizeof(line_type), cmp);
}

// 5
///////////////////////////////////////////////////////////

typedef struct {
	int row, col, val;
} triplet;

int read_sparse(triplet* triplet_array, int n_triplets) {
	for (int i = 0 ; i < n_triplets ; i++){
		int w,k,v;
		scanf("%d %d %d", &w, &k, &v);
		triplet_array[i].row = w;
		triplet_array[i].col = k;
		triplet_array[i].val = v;
	}

	return n_triplets;
}

int cmp_triplets(const void* t1, const void* t2) {
	triplet a = *( (triplet*)t1 );
	triplet b = *( (triplet*)t2 );

	if( a.row < b.row )
		return -1;
	if( b.row < a.row )
		return 1;

	if( a.col < b.col )
		return -1;
	if( b.col < a.col )
		return 1;

	return 0;
}

void make_CSR(triplet* triplet_array, int n_triplets, int rows, int* V, int* C, int* R) {
	qsort(triplet_array, n_triplets, sizeof(triplet), cmp_triplets);
	memset(R, 0, (rows+1)*sizeof(int));

	for (int i = 0 ; i < n_triplets ; i++){
		V[i] = triplet_array[i].val;
		C[i] = triplet_array[i].col;
		R[ triplet_array[i].row ] += 1;
	}

	// fix to previous rows cnt;
	int tmp, carry = 0;
	for (int i = 0 ; i < rows+1 ; i++){
		tmp = R[i];
		R[i] = carry;
		carry += tmp;
	}

}

void multiply_by_vector(int rows, const int* V, const int* C, const int* R, const int* x, int* y) {
	for (int i = 0 ; i < rows ; i++){
		int value = 0;
		
		for (int j = R[i] ; j < R[i+1] ; j++){
			value += V[j]*x[ C[j] ];
		}

		y[i] = value;

	}
}

void read_vector(int* v, int n) {
	for (int i = 0 ; i < n ; i++){
		scanf("%d",&v[i]);	
	}
}

void write_vector(int* v, int n) {
	for (int i = 0 ; i < n ; i++){
		printf("%d ",v[i]);	
	}
	printf("\n");
}

// auxiliary
////////////////////////////////////////////////////////////

int read_int(void) {
	char c_buf[BUF_SIZE];
	fgets(c_buf, BUF_SIZE, stdin);
	return (int)strtol(c_buf, NULL, 10);
}

int main(void) {
	int to_do = read_int();

	int A[TAB_SIZE], B[TAB_SIZE], AB[TAB_SIZE];
	int n, lines_counter, rowsA, colsA, rowsB, colsB;
	int rows, cols, n_triplets;
	char* char_lines_array[TAB_SIZE] = { NULL };
	int continuous_array[TAB_SIZE];
	int* ptr_array[TAB_SIZE];
	triplet triplet_array[TAB_SIZE];
	int V[TAB_SIZE], C[TAB_SIZE], R[TAB_SIZE];
	int x[TAB_SIZE], y[TAB_SIZE];
	line_type int_lines_array[TAB_SIZE];

	switch (to_do) {
		case 1:
			scanf("%d %d", &rowsA, &colsA);
			read_mat(rowsA, colsA, A);
			scanf("%d %d", &rowsB, &colsB);
			read_mat(rowsB, colsB, B);
			prod_mat(rowsA, colsA, colsB, A, B, AB);
			print_mat(rowsA, colsB, AB);
			break;
		case 2:
			n = read_int() - 1; // we count from 1 :)
			ptr_array[0] = continuous_array;
			read_int_lines_cont(ptr_array);
			write_int_line_cont(ptr_array, n);
			break;
		case 3:
			n = read_int() - 1;
			read_char_lines(char_lines_array);
			write_char_line(char_lines_array, n);
			delete_lines(char_lines_array);
			break;
		case 4:
			n = read_int() - 1;
			lines_counter = read_int_lines(int_lines_array);
			sort_by_average(int_lines_array, lines_counter);
			write_int_line(int_lines_array, n);
			delete_int_lines(int_lines_array, lines_counter);
			break;
		case 5:
			scanf("%d %d %d", &rows, &cols, &n_triplets);
			n_triplets = read_sparse(triplet_array, n_triplets);
			read_vector(x, cols);
			make_CSR(triplet_array, n_triplets, rows, V, C, R);
			multiply_by_vector(rows, V, C, R, x, y);
			write_vector(V, n_triplets);
			write_vector(C, n_triplets);
			write_vector(R, rows + 1);
			write_vector(y, rows);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

