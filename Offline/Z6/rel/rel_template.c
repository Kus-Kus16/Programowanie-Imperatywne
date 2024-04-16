#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100
#define MAX_RANGE 100

typedef struct {
	int first;
	int second;
} pair;

// Add pair to existing relation if not already there
int add_relation(pair tab[], int n, pair new_pair) {

	int i;
	for( i=0 ; i<n ; i++ ){
		if( tab[i].first == new_pair.first && tab[i].second == new_pair.second )
			return 0;
	}
	
	tab[i] = new_pair;

	return 1;

}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair relation[]) {
	int n,cnt=0;
	pair new;
	scanf("%d",&n);

	for( int i=0 ; i<n ; i++ ){
		scanf("%d %d",&new.first,&new.second);
		cnt += add_relation(relation,cnt,new);
	}

	return cnt;

}

int addsort_array(int array[], int n, int x){
	int e,tmp,i=0;
	while( i<n && array[i] < x ){
		i++;
	}

	if (i==n){
		array[i]=x;
		return 1;
	}

	if( array[i] == x )
		return 0;

	e = array[i];
	array[i] = x;
	
	while( i<n ){
		i++;
		tmp=array[i];
		array[i]=e;
		e=tmp;	
	} 

	return 1;

}

void print_int_array(int array[], int n) {
	printf("%d\n", n);
	for( int i=0 ; i < n ; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");
}

//------------------------------------------------

// Case 1:

// The relation R is reflexive if:
// xRx for every x in X
int is_reflexive(pair tab[], int n) {
	int e,Flag;
	
	for( int i=0 ; i<n ; i++ ){
		e=tab[i].first;
		Flag=0;
		for( int j=0 ; j<n ; j++ ){
			if( tab[j].first == e && tab[j].second == e){
				Flag=1;
				break;
			}
		}

		if( Flag == 0)
			return 0;

	}

	return 1;

}

// The relation R on the set X is called irreflexive if:
// xRx is false for every x in X
int is_irreflexive(pair tab[], int n) {
	int e;
	
	for( int i=0 ; i<n ; i++ ){
		if( tab[i].first == tab[i].second )
			return 0;
	}

	return 1;

}

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair tab[], int n) {
	int a,b,Flag;
	
	for( int i=0 ; i<n ; i++ ){
		a=tab[i].first;
		b=tab[i].second;
		Flag=0;

		for( int j=0 ; j<n ; j++ ){
			if( tab[j].first == b && tab[j].second == a){
				Flag=1;
				break;
			}
		}

		if( Flag == 0)
			return 0;

	}

	return 1;

}

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair tab[], int n) {
	int a,b,Flag=1;
	
	for( int i=0 ; i<n ; i++ ){
		a=tab[i].first;
		b=tab[i].second;

		for( int j=0 ; j<n ; j++ ){
			if( tab[j].first == b && tab[j].second == a){
				if (a != b)
					Flag=0;
				
				break;
			}
		}

		if( Flag == 0)
			return 0;

	}

	return 1;

}

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
//// A relation is asymmetric if and only if it is both antisymmetric and irreflexive
int is_asymmetric(pair tab[], int n) {
	int a,b,Flag=1;
	
	for( int i=0 ; i<n ; i++ ){
		a=tab[i].first;
		b=tab[i].second;

		if( a==b )
			return 0;

		for( int j=0 ; j<n ; j++ ){
			if( tab[j].first == b && tab[j].second == a){
				if (a != b)
					Flag=0;
				
				break;
			}
		}

		if( Flag == 0)
			return 0;

	}

	return 1;

}

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair tab[], int n) {
	int x,y,z,Flag;

	for( int i=0 ; i<n ; i++ ){
		x=tab[i].first;
		y=tab[i].second;
		
		for( int j=0 ; j<n ; j++ ){

			if( tab[j].first == y){
				z=tab[j].second;
				Flag=0;

				for( int k=0 ; k<n ; k++ ){
					
					if( tab[k].first == x && tab[k].second == z ){
						Flag=1;
						break;
					}

				}

				if ( Flag == 0)
					return 0;

			}



		}

	}

	return 1;

}

//------------------------------------------------

// Case 2:

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair tab[], int n) {
	int x,y,a,b,Flag;
	
	for( int i=0 ; i<n ; i++ ){
		x=tab[i].first;
		
		for( int j=i ; j<n ; j++ ){
			y=tab[j].first;
			if( x != y){
				Flag=0;
				for( int k=0 ; k<n ; k++){
					a=tab[i].first;
					b=tab[i].second;
					if( (a==x && b==y) || (b==x && a==y) ){
						Flag=1;
						break;
					}

				}

				if( Flag == 0 )
					return 0;

			}

		}

	}

	return 1;

}

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair tab[], int n) {
	int tests=0;

	tests += is_reflexive(tab,n);
	tests += is_antisymmetric(tab,n);
	tests += is_transitive(tab,n);

	if ( tests == 3 )
		return 1;
	
	return 0;

}

// A total order relation is a partial order relation that is connected
int is_total_order(pair tab[], int n) {
	int tests=0;

	tests += is_reflexive(tab,n);
	tests += is_antisymmetric(tab,n);
	tests += is_transitive(tab,n);
	tests += is_connected(tab,n);

	if ( tests == 4 )
		return 1;
	
	return 0;

}

int find_max_elements(pair tab[], int n, int max_elements[]) { // tab - partial order
	int x,Flag,cnt=0;

	for( int i=0 ; i<n ; i++ ){
		x=tab[i].first;
		Flag=1;
		for( int j=0 ; j<n ; j++ ){
			if( tab[j].first == x && tab[j].second != x ){
				Flag=0;
				break;
			}

		}

		if( Flag == 1 )
			cnt += addsort_array(max_elements,cnt,x);
			
	}

	return cnt;

}

int find_min_elements(pair tab[], int n, int min_elements[]) { // tab - strong partial order
	int y,Flag,cnt=0;

	for( int i=0 ; i<n ; i++ ){
		y=tab[i].second;
		Flag=1;
		for( int j=0 ; j<n ; j++ ){
			if( tab[j].second == y && tab[j].first != y ){
				Flag=0;
				break;
			}

		}

		if( Flag == 1 )
			cnt += addsort_array(min_elements,cnt,y);
			
	}

	return cnt;

}

int get_domain(pair tab[], int n, int domain[]) {
	int x,y,cnt=0;

	for( int i=0 ; i<n ; i++ ){
		x=tab[i].first;
		y=tab[i].second;

		cnt += addsort_array(domain,cnt,x);
		cnt += addsort_array(domain,cnt,y);
	}

	return cnt;

}

//------------------------------------------------

// Case 3:

// x(S o R)z iff exists y: xRy and ySz
// x(R o S)z iff exists y: xSy and yRz
int composition (pair rel_R[], int nR, pair rel_S[], int nS, pair rel_SoR[]) {
	int x,y,z,cnt=0;
	pair new;

	for( int i=0 ; i<nR ; i++ ){
		x=rel_R[i].first;
		y=rel_R[i].second;

		for( int j=0 ; j<nS ; j++ ){
			if( rel_S[j].first == y ){
				z=rel_S[j].second;

				new.first=x;
				new.second=z;
				cnt +=  add_relation(rel_SoR,cnt,new);
			}
		}

	}

	return cnt;

}

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];
	int max_elements[MAX_REL_SIZE];
	int min_elements[MAX_REL_SIZE];

	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size));
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
			print_int_array(max_elements, no_max_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	
	return 0;
}

