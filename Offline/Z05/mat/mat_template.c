#include <stdio.h>
#include <math.h>

#define SIZE 40

void read_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		scanf("%lf", x++);
	}
}

void print_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		printf("%.4f ", x[i]);
	}
	printf("\n");
}

void read_mat(double A[][SIZE], int w, int k) {
	for(int i = 0; i < w; ++i) {
		for(int j = 0; j < k; ++j) {
			scanf("%lf", &A[i][j]);
		}
	}
}

void print_mat(double A[][SIZE], int w, int k) {
	for(int i = 0; i < w; ++i) {
		for(int j = 0; j < k; ++j) {
			printf("%.4f ", A[i][j]);
		}
		printf("\n");
	}
}

void swap(int *a, int *b){
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

// 1. Calculate matrix product, C = A X B
// A[w][p], B[p][k], C[w][k]
void mat_product(const double A[][SIZE], const double B[][SIZE], double C[][SIZE], int w, int k, int p) {
	// iterate through C MATRIX
	double sum;
	for (int i=0 ; i<w ; i++){
		for (int j=0 ; j<k ; j++){
			
			sum=0;
			for (int x=0 ; x<p ; x++){
				sum += A[i][x]*B[x][j];
			}
			C[i][j] = sum;

		}
	}

}


// 2. Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n) {
	
	double e,factor,det=1;

	for (int i=0 ; i<n ; i++){
		e=A[i][i];

		if (e == 0)
			return NAN;

		det *= e;

		for (int w=i+1 ; w<n ; w++){
			factor = A[w][i] / e;
			for (int k=i ; k<n ; k++){
				A[w][k] -= A[i][k]*factor;
			}
		}

	}

	return det;

}


// 3. Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.

void backward_substitution_index(double A[][SIZE], double b[], double x[], const int perm[], int n) {
	
	for (int w=n-1 ; w>=0 ; w--){

			double sum = b[perm[w]];
			for (int j=n-1 ; j>w ; j--){
				sum -= x[j]*A[ perm[w] ][j];
			}
			x[w] = sum/A[ perm[w] ][w];
		
		}

}

double gauss(double A[][SIZE], double b[], double x[], const int n, const double eps) {
	
	double e,factor,det=1;
	int perm[n];

	// fill permutations array
	for (int i=0 ; i<n ; i++){
		perm[i]=i;
	}

	for (int i=0 ; i<n ; i++){
		
		// find max value
		for (int w=i+1 ; w<n ; w++){
			if ( fabs( A[ perm[w] ][i] ) > fabs( A[ perm[i] ][i] ) ){
				swap(&perm[w],&perm[i]);
				det *= -1;
			}
		}
		
		e=A[perm[i]][i];

		if ( fabs(e) < eps )
			return 0;

		det *= e;

		// gauss
		for (int w=i+1 ; w<n ; w++){
			factor = A[ perm[w] ][i] / e;
			for (int k=i ; k<n ; k++){
				A[ perm[w] ][k] -= A[ perm[i] ][k]*factor;
			}
			b[ perm[w] ] -= b[ perm[i] ]*factor;
		}

	}

	if ( det != 0){
		backward_substitution_index(A,b,x,perm,n);
	}

	return det;

}

// 4. Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps) {
		
	double e,factor,det=1;
	int perm[n];
	double C[n][n];

	// fill unitary matrix
	for (int w=0 ; w<n ; w++){
		for (int k=0 ; k<n ; k++){
			if (w==k)
				C[w][k]=1;
			else 
				C[w][k]=0;
		}
	}

	// fill permutations array
	for (int i=0 ; i<n ; i++){
		perm[i]=i;
	}

	// 1. Triangulate matrix
	for (int i=0 ; i<n ; i++){
		
		// find max value
		for (int w=i+1 ; w<n ; w++){
			if ( fabs( A[ perm[w] ][i] ) > fabs( A[ perm[i] ][i] ) ){
				swap(&perm[w],&perm[i]);
				det *= -1;
			}
		}
		
		e=A[perm[i]][i];

		if ( fabs(e) < eps )
			return 0;

		det *= e;

		// gauss
		for (int w=i+1 ; w<n ; w++){
			factor = A[ perm[w] ][i] / e;
			for (int k=i ; k<n ; k++){
				A[ perm[w] ][k] -= A[ perm[i] ][k]*factor;
			}
			for (int k=0 ; k<n ; k++){
				C[ perm[w] ][k] -= C[ perm[i] ][k]*factor;
			}
		}

		//A to unitary progress
		factor=e;
		for (int k=i ; k<n ; k++){
			A[ perm[i] ][k] /= factor;
		}
		for (int k=0 ; k<n ; k++){
			C[ perm[i] ][k] /= factor;
		}

	}


	// 2. Reverse gauss
	for (int i=n-1 ; i>=0 ; i--){		
		e=A[perm[i]][i]; 


		// gauss
		for (int w=i-1 ; w>=0 ; w--){
			factor = A[ perm[w] ][i] / e; 
			for (int k=i ; k>=0 ; k--){
				A[ perm[w] ][k] -= A[ perm[i] ][k]*factor;
			}
			for (int k=n-1 ; k>=0 ; k--){
				C[ perm[w] ][k] -= C[ perm[i] ][k]*factor;
			}
		}

	}

	// 3. Move to B
	for (int w=0 ; w<n ; w++){
		for (int k=0 ; k<n ; k++){
			B[w][k] = C[perm[w]][k];
		}
	}

	return det;

}


int main(void) {

	double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
	double b[SIZE], x[SIZE], det, eps = 1.e-13;

	int to_do;
	int m, n, p;

	scanf ("%d", &to_do);

	switch (to_do) {
		case 1:
			scanf("%d %d %d", &m, &p, &n);
			read_mat(A, m, p);
			read_mat(B, p, n);
			mat_product(A, B, C, m, n, p);
			print_mat(C, m, n);
			break;
		case 2:
			scanf("%d", &n);
			read_mat(A, n, n);
			printf("%.4f\n", gauss_simplified(A, n));
			break;
		case 3:
			scanf("%d", &n);
			read_mat(A,n, n);
			read_vector(b, n);
			det = gauss(A, b, x, n, eps);
			printf("%.4f\n", det);
			if (det) print_vector(x, n);
			break;
		case 4:
			scanf("%d", &n);
			read_mat(A, n, n);
			det = matrix_inv(A, B, n, eps);
			printf("%.4f\n", det);
			if (det) print_mat(B, n, n);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

