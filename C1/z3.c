#include <stdio.h>
/*
#include <math.h>

int doskonala(int x){
    float sq=sqrt(x);
    int i=2, sum=1;

    while ( i<=sq ){
        if ( x%i==0 )
            sum += ( i + x/i );
        
        i++;
    }

    return ( sum==x )? 1:0 ;
}*/

int doskonala(int x){
    int sum=0;
    for ( int i=1 ; i<x ; i++){
        if ( x%i==0 )
            sum += i;
    }

    return (sum==x);
}

int main(){

    int z;

    printf("Podaj zasieg: ");
    scanf("%d",&z);

    
    for ( int i=1 ; i<=z ; i++){
        if ( doskonala(i) == 1 )
            printf("%d, ",i);
    }

    return 0;
    
}

