#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <unistd.h>

int main(int argc, const char *argv[]){
    int cont=0;
    ucontext_t context;

    getcontext(&context);
    puts("Hy\n");
    sleep(1);
    if (++cont<10)
        setcontext(&context);
    return 0;
}
