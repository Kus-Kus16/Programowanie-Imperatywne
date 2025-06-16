#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_BIGRAMS ((LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR))

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

int count[MAX_BIGRAMS] = { 0 };

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	if (count[va] == count[vb])
		return va - vb;
	else
		return count[vb] - count[va];
}

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp_bi (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	// sort according to second char if counts and the first char equal
	if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) 
		return va % MAX_CHARS - vb % MAX_CHARS;
	// sort according to first char if counts equal
	else if (count[va] == count[vb]) 
		return va / MAX_CHARS - vb / MAX_CHARS;
	else
		return count[vb] - count[va];
}


// -- 1 --
// count lines, words & chars in a given text file
void wc(int *nl, int *nw, int *nc) {
	*nl=*nw=*nc=0;
	int in = 0;
	char c=getchar();

	while( c != EOF ){
		(*nc)++;
		if ( isspace(c) ){
			if (in==1)
				(*nw)++;
			if ( c == '\n' )
				(*nl)++;
			in=0;
		}
		else
			in = 1;

		c=getchar();

	}

}


// -- 2 --
void char_count(int char_no, int *n_char, int *cnt) {
	
	int ids[MAX_CHARS];
	// FIRSTCHAR - 0, SECONDCHAR - 1, ... , LASTCHAR-1 - LASTCHAR-2
	for ( int i=0 ; i<MAX_CHARS ; i++){
		count[i]=0;
		ids[i]=i;
	}

	char c=getchar();
	while( c != EOF ){
		if ( c >= FIRST_CHAR && c < LAST_CHAR )
			count[c-FIRST_CHAR] += 1;

		c=getchar();
	}

	qsort(ids,MAX_CHARS,sizeof(int),cmp);

	(*n_char)=ids[char_no-1]+FIRST_CHAR;
	(*cnt)=count[*n_char-FIRST_CHAR];

}


// -- 3 --
void bigram_count(int bigram_no, int bigram[]) {
	int bigrams [MAX_BIGRAMS];
    for (int i=0; i<MAX_BIGRAMS; i++){
        bigrams[i]=i;
        count[i]=0;
    }
    char d,c=fgetc(stdin);
    while (c!=EOF){
        d=fgetc(stdin);

        if (c>=FIRST_CHAR && c<LAST_CHAR && d>=FIRST_CHAR && d<LAST_CHAR)
            count[(c-FIRST_CHAR)*MAX_CHARS+d-FIRST_CHAR]+=1;

        c=d;
    }
    
	qsort(bigrams,MAX_BIGRAMS,sizeof(int),cmp_bi);

    bigram[0]=FIRST_CHAR + bigrams[bigram_no-1] / MAX_CHARS;
    bigram[1]=FIRST_CHAR + bigrams[bigram_no-1] % MAX_CHARS;
    bigram[2]=count[(bigram[0]-FIRST_CHAR) * MAX_CHARS + bigram[1] -FIRST_CHAR];
}


// -- 4 --
void find_comments(int *line_cnt, int *block_cnt) {
	*line_cnt=*block_cnt=0;
	int CLOSED=1;

	char c=getchar();
	while( c != EOF ){
		if ( c == '/'){
			c=getchar();

			if ( c == '/'){
				(*line_cnt)++;
				c=getchar();

				while( c != EOF && c != '\n')
					c=getchar();
			}

			else if ( c == '*'){
				(*block_cnt)++;
				CLOSED = 0;

				while ( CLOSED != 1){
					c=getchar();
					if ( c == '*'){
						c=getchar();
						if ( c == '/')
							CLOSED=1;
					}
				
				}


			}

		}
		else
			c=getchar();
	}

}

#define MAX_LINE 128

int read_int() {
	char line[MAX_LINE];
	fgets(line, MAX_LINE, stdin); // to get the whole line
	return (int)strtol(line, NULL, 10);
}

int main(void) {
	int to_do;
	int nl, nw, nc, char_no, n_char, cnt;
	int line_comment_counter, block_comment_counter;
	int bigram[3];

	to_do = read_int();
	switch (to_do) {
		case 1: // wc()
			wc (&nl, &nw, &nc);
			printf("%d %d %d\n", nl, nw, nc);
			break;
		case 2: // char_count()
			char_no = read_int();
			char_count(char_no, &n_char, &cnt);
			//read_count(MAX_CHARS);
			printf("%c %d\n", n_char, cnt);
			break;
		case 3: // bigram_count()
			char_no = read_int();
			bigram_count(char_no, bigram);
			printf("%c%c %d\n", bigram[0], bigram[1], bigram[2]);
			break;
		case 4:
			find_comments(&line_comment_counter, &block_comment_counter);
			printf("%d %d\n", block_comment_counter, line_comment_counter);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

