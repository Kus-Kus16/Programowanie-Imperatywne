#include <stdio.h>

int main(){

    int a=0, b=1, c, n, il=0;

    printf("Podaj N:");
    scanf("%d",&n);

    while (il<n){  
        c=a+b;
        a=b;
        b=c;
        il=a*b;
    }

    if (il==n)
        printf("True");
    else
        printf("False");


    return 0;
}