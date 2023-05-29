#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>

void main(){
    int *p = malloc(sizeof(int));
    p = 5;
    int x = p;
    printf("%d\n", x);
    p = 3;
    printf("%d\n", x);
    printf("%d\n", p);
}