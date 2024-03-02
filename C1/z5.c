#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int guessing(int X){
    int a=0, b=100 ,n;
    while (1){
        n = ( rand()%b-a+1 )+ a;
        
        if ( n==X ){
            printf("Zgadlem hehe: %d",n);
            return 0;
        }
        if ( n<X )
            a=n+1;
        else
            b=n-1;

    }

}

int main(){
    srand(time(NULL));
    int X = rand()%101;

    guessing(X);
    
    return 0;

}