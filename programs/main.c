#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HP.h"


int main() {
    HP_info* i;
    char attr[]= "testtt"; 
    HP_CreateFile("test", 'c', attr, strlen(attr), 5);
    i= HP_OpenFile("test");
    printf("%d, %s, %c, %d\n", i->fileDesc, i->attrName, i->attrType, i->attrLength);
    HP_CloseFile(i);
    free(i->attrName);
    free(i);
    return 0;
}
