#include <stdio.h>
#include <math.h>

double silnia(int n) {
    double res = 1;
    for (int i = 2; i <= n; i++) {
        res *= i;
    }
    return res;
}

double taylor_cos(float x) {
    double res = 0;
    for (int i = 0; i <= 6 ; i++) {
        res += pow(-1, i) * pow(x, 2 * i) / silnia(2 * i);
    }
    return res;
}

int main(){
    float a,b,x;

    printf("Podaj a,b,krok: ");
    scanf("%f %f %f",&a,&b,&x);
    printf("\n\nx|cos(x)|~cos(x)\n");
    
    for( &a ; a<=b; a+=x ){
        printf("%f, %f, %f\n",a,cos(a),taylor_cos(a));
    }

    return 0;
}