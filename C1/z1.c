#include <stdio.h>
#include <limits.h>

int main(){

    int n;
    unsigned long long int res = 1;
    
    printf("Podaj N: ");
    scanf("%d",&n);

    if(n<0){
        printf("Niepoprawne N");
        return 0;
    }

    for(int i=2 ; i<=n ; i++){

        if( res > (ULLONG_MAX/i) ){
            printf("Przekroczenie zakresu!");
            return 0;
        }

        res *= i;
    }

    printf("N! wynosi: %llu",res);    

    return 0; 
}