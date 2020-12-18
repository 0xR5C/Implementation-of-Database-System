#include <stdio.h>
#include <stdlib.h>
#include "HP.h"


int main() {
    char attr[]= "testtt"; 
    HP_CreateFile("test", 'c', attr, strlen(attr), 5);
    return 0;
}
