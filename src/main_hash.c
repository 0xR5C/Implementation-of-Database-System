#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "records.h"
#include "HT.h"


int main(int argc, char* argv[]) {
    HT_info* i;
    int ret;
    ret= HT_CreateIndex("hash_test", 'c', "id", sizeof(int), 69);
    printf("--%d\n", ret);

    i= HT_OpenIndex("hash_test");
    printf("---%d\n", i->fileDesc);

    HT_CloseIndex(i);

    
}