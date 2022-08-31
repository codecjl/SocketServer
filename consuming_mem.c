#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#define GSize 1073741824

int main(int argc, char * argv[]){
    (void)argc;
    (void)argv;
    char * mm[100];
    int n;
    printf("how many Gigabyte of memory you want to lock? ");
    scanf("%d", &n);
    if( n>=100 || n<=0){
        printf("Too big (<100), exit\n");
        return -1;
    }
    printf("n=%d\n", n);
    int a=0;
    for(; a<100; a++){
        if( a == n)
            break;
        mm[a] = malloc( GSize ); // alloc 1G memory
        memset(mm[a], 0, GSize);
        mlock((const void *)mm[a], GSize ); // avoid paging
    }
    char car;
    car = (char)getchar();
    car = (char)getchar();
    (void)car;
    munlockall();
    return 0;
}
