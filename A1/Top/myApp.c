#include <stdio.h>

extern mySquare();
extern myPythonInC();

int main(int argc, char *argv[]){
    int a=3,b;
    b= mySquare(a);
    printf("%d\n",b);
    myPythonInC(1,argv);
}
