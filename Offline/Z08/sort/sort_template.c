#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
	int day; 
	int month; 
	int year;
} Date;

/////////////////////////////////////////////////////////////////
// 1

#define NAME_MAX  20   // max. length of name + 1
#define FOOD_MAX  30   // max. number of goods
#define RECORD_MAX 64  // max. line length

typedef struct {
	char name[NAME_MAX];
	float price;
	int amount;
	Date valid_date;
} Food;

// pointer to function
typedef int (*ComparFp)(const void*, const void*);

int cmp_date(const void* d1, const void* d2) {
	Date a = *( (Date*)d1 );
	Date b = *( (Date*)d2 );

	if ( a.year < b.year )
		return -1;
	if ( b.year < a.year )
		return 1;

	if ( a.month < b.month ) 
		return -1;
	if ( b.month < a.month )
		return 1;

	if ( a.day < b.day )
		return -1;
	if ( b.day < a.day )
		return 1;
	else
		return 0;

}

int cmp_price(const void* p1, const void* p2) {
	float a = *( (float*)p1 );
	float b = *( (float*)p2 );

	if( a < b )
		return -1;
	if( b < a )
		return 1;
	else
		return 0;

}

int cmp(const void* fa, const void* fb) {
	Food a = *( (Food*)fa );
	Food b = *( (Food*)fb );
	int res;

	res = strcmp(a.name, b.name);
	if( res < 0 )
		return -1;
	if( res > 0 )
		return 1;

	res = cmp_price(&a.price, &b.price);
	if( res < 0 )
		return -1;
	if( res > 0 )
		return 1;
	
	res = cmp_date(&a.valid_date, &b.valid_date);
	if( res < 0 )
		return -1;
	if( res > 0 )
		return 1;
	else
		return 0;

}

int cmp_foodbydate(const void* fa, const void* fb) {
	Food a = *( (Food*)fa );
	Food b = *( (Food*)fb );

	return cmp_date( &a.valid_date, &b.valid_date );
}

void* bsearch2 (const void* key, const void* base, const size_t nitems, const size_t size, const ComparFp compar, char* result) {
	
	int left=0, right=nitems-1;
	
	if (nitems == 0) {
		*result = 0;
		return (char*)base + left*size; //?
    }

	while (left <= right){
			
		int mid = (right-left)/2 + left;
		void* midptr = (char*)base + mid*size;

		int comres = compar(key, midptr);
		if (comres == 0){
			*result = 1;
			return midptr;
		}
		if (comres < 0){
			right = mid - 1;
		}
		else{ // comres > 0
			left = mid + 1;
		}

	}


	
	* result = 0;
	return (char*)base + left*size;

}

void print_art(Food* p, const int n, const char* art) {
	
	for( int i=0 ; i<n ; i++){
		if( strcmp(p[i].name,art) == 0 )
			printf("%.2f %d %02d.%02d.%04d\n", p[i].price, p[i].amount, p[i].valid_date.day, p[i].valid_date.month,
                   p[i].valid_date.year);
	}

}

Food* add_record(Food* tab, int* nitems, const ComparFp compar, const Food* new) {

	char res;
	Food* location = bsearch2( new, tab, *nitems, sizeof(Food), compar, &res );

	if(res == 1){ //article already exists
		location -> amount += new -> amount;
	}
	if(res == 0){ //article not found
		int new_index = location - tab;
		(*nitems)++;
		for( int i = *nitems ; i > new_index ; i-- ){
			tab[i] = tab[i-1];
		}

		tab[new_index] = *new;
	}

	return location;

}

int read_goods(Food* tab, const int no, FILE* stream, const int sorted) {
	int nitems = 0;
	
	for( int i=0 ; i<no ; i++ ){
		Food new;

		fscanf(stream,"%s",new.name);
		fscanf(stream,"%f",&new.price);
		fscanf(stream,"%d",&new.amount);
		fscanf(stream,"%d.%d.%d",&new.valid_date.day,&new.valid_date.month,&new.valid_date.year);

		if (sorted == 1)
			add_record(tab, &nitems, cmp, &new);
		if (sorted == 0){
			tab[nitems]=new;
			nitems++;
		}

	}

	return nitems;

}

/////////////////////////////////////////////////////////////////
// 2 value

int cmp_qs(const void* a, const void* b) {
	const Food *fa = (Food*)a, *fb = (Food*)b;
	return cmp_date(&fa->valid_date, &fb->valid_date);
}

int cmp_bs(const void* a, const void* b) {
	const Date *pd = (Date*)a;
	const Food *fb = (Food*)b;
	return cmp_date(pd, &fb->valid_date);
}

void DateToTime(const Date* date, time_t* time) {
	struct tm* tmp = localtime(time);
	tmp->tm_year = date->year - 1900; //[1900-]
	tmp->tm_mon = date->month - 1; //[0,11]
	tmp->tm_mday = date->day;

	*time = mktime(tmp);
}
void TimeToDate(const time_t* time, Date* date){
	struct tm* tmp = localtime(time);
	date->year = tmp -> tm_year + 1900;
	date->month = tmp -> tm_mon + 1;
	date->day = tmp -> tm_mday;
}

float value(Food* food_tab, const size_t n, const Date curr_date, const int days) {
	
	//printf("\n%d,%d,%d\n",curr_date.day,curr_date.month,curr_date.year);
	// 1. Determine date
	time_t tmp;
	time(&tmp);
	Date searched_date;
	
	DateToTime(&curr_date,&tmp);

	tmp += days*24*3600;
	if (tmp == -1)
		printf("Error!");



	TimeToDate(&tmp,&searched_date);

	//printf("%d,%d,%d\n\n",searched_date.day,searched_date.month,searched_date.year);
		

	// 2. sort table
	qsort((void*)food_tab, n, sizeof(Food), cmp_foodbydate);

	/* sorted:
	for ( int i = 0 ; i<n ; i++ ){
		Date balls = food_tab[i].valid_date; 
		printf("%d,%d,%d\n",balls.day,balls.month,balls.year);
	}
	*/

	// 3. Check for date
	Food checker;
	checker.valid_date = searched_date;
	char res;
	
	Food* found = bsearch2(&checker, food_tab, n, sizeof(Food),cmp_foodbydate,&res);
	if( res == 0 )
		return 0.0;

	float total = 0;
	//total += found->price * found->amount;

	int index = found - food_tab;

	for( int i=index ; i >= 0 && cmp_date(&searched_date,&food_tab[i].valid_date) == 0 ; i--){
		total += food_tab[i].price * food_tab[i].amount;
	}
	for( int i=index+1 ; i < n && cmp_date(&searched_date,&food_tab[i].valid_date) == 0 ; i++){
		total += food_tab[i].price * food_tab[i].amount;
	}

	return total;	

}

/////////////////////////////////////////////////////////////////
// 3 Succession

#define CHILD_MAX 20

enum Sex {F, M};
enum BOOL {no, yes};

struct Bit_data {
	enum Sex sex:1;
	enum BOOL in_line:1;
};

typedef struct {
	char *name;
	struct Bit_data bits;
	Date born;
	char *parent;
} Person;

typedef struct {
	char *name;
	int first_index;
	int children_cnt;
} Parent;

const Date primo_date = { 28, 10, 2011 }; // new succession act

// sex compare
int cmp_sex(const Person* person1, const Person* person2) {
	// same sex = sexually equal
	if (person1->bits.sex == person2->bits.sex)
		return 0;
	
	// Woman date determinant:
	// If born after Date -> She is a better woman -> She is sexually equal to man
	if (person1->bits.sex == F && cmp_date(&person1->born, &primo_date) > 0)
		return 0;
	if (person2->bits.sex == F && cmp_date(&person2->born, &primo_date) > 0)
		return 0;

	// Here means woman born before Date -> she is a worse woman -> Man wins	
	return person2->bits.sex - person1->bits.sex;
}

//int cmp_date(const void* d1, const void* d2)

int cmp_initial(const void* p1, const void* p2) {
	Person a = *( (Person*)p1 );
	Person b = *( (Person*)p2 );
	int result;

	// A. Check for King
	if( a.parent == NULL )
		return -1;
	if( b.parent == NULL )
		return 1;

	// B. Alfabetical by parents
	result = strcmp(a.parent,b.parent);
	if( result < 0 )
		return -1;
	if( result > 0 )
		return 1;

	// C. By sex value
	result = cmp_sex(&a,&b);
	if( result < 0 )
		return -1;
	if( result > 0 )
		return 1;

	// D. By born date
	result=cmp_date(&a.born,&b.born);
	if( result < 0 )
		return -1;
	if( result > 0 )
		return 1;

	return 0;

}

int cmp_parent(const void* p1, const void* p2) {
	Person a = *( (Person*)p1 );
	Person b = *( (Person*)p2 );

	if(a.parent == NULL)
		return -1;
	if(b.parent == NULL)
		return 1;

	return strcmp(a.parent,b.parent);

}


int fill_indices_tab(Parent *idx_tab, const Person *pers_tab, const int size) {
}

void persons_shifts(Person *person_tab, const int size, Parent *idx_tab, const int no_parents) {
}

int cleaning(Person *person_tab, int n) {
}

void print_person(const Person* p) {
	printf("%s\n", p->name);
}

void printtab_persons(Person* person_tab, int n){
	for( int i=0 ; i<n ; i++ ){
		printf("%d,self:%s,parent:%s\n",i,person_tab[i].name,person_tab[i].parent);
	}
}

void printtab_parents(Parent* parent_tab, int n){
	for( int i=0 ; i<n ; i++ ){
		printf("%d,iam:%s,parentof:%d\n",i,parent_tab[i].name,parent_tab[i].children_cnt);
	}
}

void create_lists(Person* person_tab, Parent* parent_tab, int n) {
	// initial sort, siblings near each other, siblings sorted, king at [0]
	// between siblings as follows:	sexsort, bornsort
	qsort(person_tab, n, sizeof(Person), cmp_initial);
	
	for( int i=0 ; i<n ; i++ ){

		parent_tab[i].name = person_tab[i].name;

		Person key = { .parent = person_tab[i].name };
		Person* location = bsearch(&key, person_tab, n, sizeof(Person), cmp_parent);

		if(location != NULL){
			// has children

			//find first kid
			int j = location - person_tab;
			while( j > 0 && strcmp(person_tab[j].parent, parent_tab[i].name) == 0 )
				j --;
			j++;
			parent_tab[i].first_index = j;

			//count children amount
			while( j < n && strcmp(person_tab[j].parent, parent_tab[i].name) == 0 )
				j ++;
			parent_tab[i].children_cnt = j - parent_tab[i].first_index;

		}
		else {
			// no children

			parent_tab[i].first_index = -1;
			parent_tab[i].children_cnt = 0;
		}

	} 
	
	//printtab_persons(person_tab,n);
	//printf("\n\n");
	//printtab_parents(parent_tab,n);
	
}

int find_successor(const Person* person_tab, const Parent* parent_tab, int index, int heir, int* result ){
	if( heir == 0 ){
		*result = index;
		return -1;
	}

	
	if( heir < 0 || parent_tab[index].children_cnt == 0 )
		return heir;

	int first_child = parent_tab[index].first_index;
	for ( int i=0 ; i< parent_tab[index].children_cnt ; i++ ){
		
		if( person_tab[first_child+i].bits.in_line )
			heir--;
		
		heir = find_successor(person_tab, parent_tab, first_child + i, heir, result);

	}

	return heir;

}

void succession(Person* person_tab, int n, int heir){
	Parent parent_tab[n];
	create_lists(person_tab, parent_tab, n);
	
	int result;
	find_successor(person_tab, parent_tab, 0, heir, &result);
	print_person( &person_tab[result] );
}

int main(void) {
	Person person_tab[] = {
		{"Charles III", {M, no}, {14, 11, 1948},"Elizabeth II"},
		{"Anne", {F,yes}, {15, 8, 1950},"Elizabeth II"},
		{"Andrew", {M,yes}, {19, 2, 1960},"Elizabeth II"},
		{"Edward", {M,yes}, {10, 3, 1964} ,"Elizabeth II"},
		{"David", {M,yes}, {3, 11, 1961} ,"Margaret"},
		{"Sarah", {F,yes}, {1, 5, 1964}, "Margaret"},
		{"William", {M,yes}, {21, 6, 1982}, "Charles III"},
		{"Henry", {M,yes}, {15, 9, 1984}, "Charles III"},
		{"Peter", {M,yes}, {15, 11, 1977}, "Anne"},
		{"Zara", {F,yes}, {15, 5, 1981}, "Anne"},
		{"Beatrice", {F,yes}, {8, 8, 1988}, "Andrew"},
		{"Eugenie", {F,yes}, {23, 3, 1990}, "Andrew"},
		{"James", {M,yes}, {17, 12, 2007}, "Edward"},
		{"Louise", {F,yes}, {8, 11, 2003}, "Edward"},
		{"Charles", {M,yes}, {1, 7, 1999}, "David"},
		{"Margarita", {F,yes}, {14, 5, 2002}, "David"},
		{"Samuel", {M,yes}, {28, 7, 1996}, "Sarah"},
		{"Arthur", {M,yes}, {6, 5, 2019}, "Sarah"},
		{"George", {M,yes}, {22, 7, 2013}, "William"},
		{"George VI", {M,no}, {14, 12, 1895}, NULL},
		{"Charlotte", {F,yes}, {2, 5, 2015}, "William"},
		{"Louis", {M,yes}, {23, 4, 2018}, "William"},
		{"Archie", {M,yes}, {6, 5, 2019}, "Henry"},
		{"Lilibet", {F,yes}, {4, 6, 2021}, "Henry"},
		{"Savannah", {F,yes}, {29, 12, 2010}, "Peter"},
		{"Isla", {F,yes}, {29, 3, 2012}, "Peter"},
		{"Mia", {F,yes}, {17, 1, 2014}, "Zara"},
		{"Lena", {F,yes}, {18, 6, 2018}, "Zara"},
		{"Elizabeth II", {F,no}, {21, 4, 1926}, "George VI"},
		{"Margaret", {F,no}, {21, 8, 1930}, "George VI"},
		{"Lucas", {M,yes}, {21, 3, 2021}, "Zara"},
		{"Sienna", {F,yes}, {18, 9, 2021}, "Beatrice"},
		{"August", {M,yes}, {9, 2, 2021}, "Eugenie"},
		{"Ernest", {M,yes}, {30, 5, 2023}, "Eugenie"}
	};

	int to_do, no;
	int n;
	Food food_tab[FOOD_MAX];
	char buff[RECORD_MAX];
	fgets(buff, RECORD_MAX, stdin);
	sscanf(buff, "%d", &to_do);

	switch (to_do) {
		case 1:  // bsearch2
			fgets(buff, RECORD_MAX, stdin);
			sscanf(buff, "%d", &no);
			n = read_goods(food_tab, no, stdin, 1);
			scanf("%s",buff);
			print_art(food_tab, n, buff);
			break;
		case 2: // qsort
			fgets(buff, RECORD_MAX, stdin);
			sscanf(buff, "%d", &no);
			n = read_goods(food_tab, no, stdin, 0);
			Date curr_date;
			int days;
			scanf("%d %d %d", &curr_date.day, &curr_date.month, &curr_date.year);
			scanf("%d", &days);
			printf("%.2f\n", value(food_tab, (size_t)n, curr_date, days));
			break;
		case 3: // succession
			scanf("%d", &no);
			const int no_persons = sizeof(person_tab) / sizeof(Person);
			succession(person_tab,no_persons,no);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
	}
	return 0;
}

