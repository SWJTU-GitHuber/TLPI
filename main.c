#include <stdio.h>

void f(int a);

int main(int argc, char *argv[]){
    f(2);
    return 0;
}

void f(int a){
    printf("Hello --- %d",a);
}
