#include <stdio.h>

int main(){

    char tekst[100], x;
    int k, y;

    printf("Podaj stala szyfru: ");
    scanf("%d",&k);
    printf("Podaj ciag znakow: ");
    scanf("%99s",&tekst);
    
    
    for ( int i=0 ; tekst[i] != '\0' ; i++){
        x=tekst[i];
        if ( (x>='a' && x<='z') || (x>='A' && x<='Z') ){
            if  (x>='a' && x<='z')
                tekst[i]=( tekst[i]-'a'+k)%26 + 'a';
            else
                tekst[i]=( tekst[i]-'A'+k)%26 + 'A';

        }

    }

    printf("%s",tekst);

    return 0;
}