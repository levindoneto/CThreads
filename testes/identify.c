#include <stdio.h>
#include <stdlib.h>

#include "cthread.h"

/* Tests cidentify*/
#include "cdata.h"
#include "able_threads.h"


int main(){
    char test[200];
    int num;
    num = 0;
    cidentify(test,num);
    printf("\nPrinting with %d characters ->\n %s",num,test);
    num = 10;
    cidentify(test,num);
    printf("\nPrinting with %d characters ->\n %s",num,test);
    num = 20;
    cidentify(test,num);
    printf("\nPrinting with %d characters ->\n %s",num,test);
    num = 50;
    cidentify(test,num);
    printf("\nPrinting with %d characters ->\n %s",num,test);
    num = 100;
    cidentify(test,num);
    printf("\nPrinting with %d characters ->\n %s",num,test);
    num = 200;
    cidentify(test,num);
    printf("\nPrinting with %d characters ->\n %s",num,test);
    return 0;
}
