#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "records.h"

#include "BF.h"
#include "HT.h"

int getMaxRecords(void) {
  return (BLOCK_SIZE - sizeof(int)*2)/sizeof(Record);
}

long hash(long n, long buckets) {
    return n%buckets + 1;
}

int HT_CreateIndex(char* fileName, char attrType, char* attrName, int attrLength, int buckets) {
    int ret;
    int fileDesc;
    int counter =0;
    void* block;
    
    BF_Init();
    ret= BF_CreateFile(fileName);
    if (ret < 0) {
        return -1;
    }

    fileDesc= BF_OpenFile(fileName);

    ret= BF_AllocateBlock(fileDesc);
    if (ret < 0) {
        return -1;
    }    

    
    fileDesc= BF_OpenFile(fileName);
    if (fileDesc < 0) {
        return -1;
    }

    ret= BF_AllocateBlock(fileDesc);
    if (ret < 0) {
        return -1;
    }

    ret= BF_ReadBlock(fileDesc, 0, &block);
    if (ret < 0) {
        return -1;
    }

    int pointer= 1;
    memcpy(block, (int*)&pointer, sizeof(int));
    block+= sizeof(int);
    memcpy(block, (int*)&fileDesc, sizeof(int));
    block+= sizeof(int);
    memcpy(block, (char*)&attrType, sizeof(char));
    block+= sizeof(char);
    memcpy(block, (char*)attrName, 40*sizeof(char));
    block+= 40*sizeof(char);
    memcpy(block, (int*)&attrLength, sizeof(int));
    block+= sizeof(int);
    memcpy(block, (int*)&buckets, sizeof(int));
    ret= BF_WriteBlock(fileDesc, 0);
    if (ret < 0) {
        return -1;
    }

    for (int i=1; i<=buckets; i++) {
        ret= BF_AllocateBlock(fileDesc);
        if (ret < 0) {
            return -1;
        }    
        pointer= -1;
        ret= BF_ReadBlock(fileDesc, i, &block);
        if (ret < 0) {
            return -1;
        }            

        memcpy(block, (int*)&pointer, sizeof(int));
        block+= sizeof(int);
        memcpy(block, (int*)&counter, sizeof(int));
        ret= BF_WriteBlock(fileDesc, i);
        if (ret < 0) {
            return -1;
        }            
    }




    ret= BF_CloseFile(fileDesc);
    if (ret < 0) {
        return -1;
    }
    return 0;
}


HT_info* HT_OpenIndex(char* fileName) {
    void* block;
    int retval;
    char temp[40];

    HT_info* info= malloc(sizeof(HT_info));
    info->attrName= malloc(sizeof(char)*40);

    info->fileDesc= BF_OpenFile(fileName);
    if (info->fileDesc < 0) {
        printf("yyyy\n");
        return NULL;
    }


    retval= BF_ReadBlock(info->fileDesc, 0, &block);
    if (retval < 0) {
        printf("uuuu\n");
        return NULL;
    }

    // skip pointer
    block+= sizeof(int);
    memcpy(&(info->fileDesc), (int*)block, sizeof(int));
    block+= sizeof(int);
    memcpy(&(info->attrType), (char*)block, sizeof(char));
    block+= sizeof(char);
    memcpy(temp, (char*)block, 40*sizeof(char));
    strcpy(info->attrName, temp);
    block+= 40*sizeof(char);
    memcpy(&(info->attrLength), (int*)block, sizeof(int));
    block+= sizeof(int);
    memcpy(&(info->numBuckets), (int*)block, sizeof(int));
    return info;

}


int HT_CloseIndex(HT_info* header_info) {

    BF_CloseFile(header_info->fileDesc);
    free(header_info->attrName);
    free(header_info);
    return 0;
}