#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

int main(int argc, char * argv[]){
    (void)argc;
    (void)argv;
    long a=0;
    while(true){
        a++;
        if( a == LONG_MAX )
            a=0;
    }
    return 0;
}
