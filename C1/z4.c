#include <stdio.h>
#include <math.h>

int Ndescending(int x){
    int last;

    while ( x>0 ){
        last = x%10;
        x /= 10;

        if ( !( last >= x%10 ) )
            return 0;
        
    }

    return 1;
}


int prime(int x){
    if ( x==2 || x==3 )
        return 1;
    if ( x<=1 || x%2==0 || x%3==0 )
        return 0;

    float sq=sqrt(x);
    int n=5;
    while ( n<=sq ){
        if ( x%n==0 )
            return 0;
        n += 2;
        if ( x%n==0 )
            return 0;
        n += 4;

    }

    return 1;

}

int main(){
    int n, cnt=0;

    printf("Podaj N: ");
    scanf("%d",&n);

    for ( int i=2 ; i<n ; i++ ){
        if ( Ndescending(i) && prime(i) ){
            printf("%d, ",i);
            cnt++ ;
        }
    }

    printf("\nLiczba wlasciwych liczb: %d",cnt);
    
    return 0;

}