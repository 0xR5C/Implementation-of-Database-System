#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "BF.h"
#include "HP.h"

int HP_CreateFile(char *fileName, char attrType, char* attrName, int attrLength, int buckets) {
    int retval;
    //int first;
    int fileDesc;
    void* block;

    BF_Init();
    retval= BF_CreateFile(fileName);
    if (retval < 0) {
        return -1;
    }

    fileDesc= BF_OpenFile(fileName);
    if (fileDesc < 0) {
        return -1;
    }

    retval= BF_AllocateBlock(fileDesc);
    if (retval < 0) {
        return -1;
    }    

    retval= BF_ReadBlock(fileDesc, 0, &block);
    if (retval < 0) {
        return -1;
    }  

    HP_info inf;
    inf.fileDesc= fileDesc;
    inf.attrType= attrType;
    inf.attrName= attrName;
    inf.attrLength= attrLength;
    inf.numBuckets= buckets;

    memcpy(block, (HP_info*)&inf, sizeof(HP_info));

    retval= BF_WriteBlock(fileDesc, 0);
    if (retval < 0) {
        return -1;
    }      



    retval= BF_CloseFile(fileDesc);
    if (retval < 0) {
        return -1;
    }
    return 0;
}




HP_info* HP_OpenFile(char* filename) {
    return NULL;
}

int HP_CloseFIle(HP_info* header_info){
    return 0;
}