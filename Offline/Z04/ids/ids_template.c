#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

#define KEY_SIZE 32

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);

char identifiers[MAX_IDS][MAX_ID_LEN];

const char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"
};

int if_found(char word [],int in_tab){
    int size=sizeof(keywords)/sizeof(keywords[0]);

    for (int i=0; i<size; i++){
        if (strcmp(word,keywords[i])==0)
			return 0;
    }
    for (int i=0; i<in_tab; i++){
        if (strcmp(word,identifiers[i])==0)
			return 0;
    }

    return 1;
}

int find_idents()
{
    int i,counter=0,ind=0;
    char d,c=getchar();

    while (c != EOF){

        if (c=='\"') {
            c=getchar();
            while (c!='\"'){
                if (c=='\\')
					getchar();
                c=getchar();
            }
        }
        else if (c=='\'') {
            c=getchar();
            while (c!='\''){
                if (c=='\\')
					getchar();
                c=getchar();
            }
        }
        else if (c=='/'){
            d=getchar();
			
            if (d=='/'){
				while (c!='\r' && c!='\n')
					c=getchar();
			}
			else if (d=='*'){
                while (!(c=='*' && d=='/')){
                    c=d;
                    d=getchar();
                }
            }
        }
        else if (c=='_' || isalpha(c)){
            i=0;
            while (c=='_' || isalnum(c)){
                identifiers[ind][i]=c;
                i++;
                c=getchar();
            }
            counter+=if_found(identifiers[ind],ind);
            ind++;
        }
        c=getchar();
    }
    return counter;
}

int cmp(const void* first_arg, const void* second_arg) {
	char *a = *(char**)first_arg;
	char *b = *(char**)second_arg;
	return strcmp(a, b);
}

int index_cmp(const void* first_arg, const void* second_arg) {
	int a = *(int*)first_arg;
	int b = *(int*)second_arg;
	return strcmp(identifiers[a], identifiers[b]);
}

int main(void) {
	printf("%d\n", find_idents());
	return 0;
}

