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
    void* i;

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

    i= block;
    memcpy(i, (int*)&fileDesc, sizeof(int));
    i+= sizeof(int);
    memcpy(i, (char*)&attrType, sizeof(char));
    i+= sizeof(char);
    memcpy(i, (char*)attrName, strlen(attrName)+1);
    i+= strlen(attrName)+1;
    memcpy(i, (int*)&attrLength, sizeof(int));
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




HP_info* HP_OpenFile(char* fileName) {
    HP_info* i= malloc(sizeof(HP_info));
    i->attrName= malloc(sizeof(char)*40);
    char temp[40];
    int fileDesc, retval;
    void* block;

    fileDesc= BF_OpenFile(fileName);
    if (fileDesc < 0) {
        return NULL;
    }

    retval= BF_ReadBlock(fileDesc, 0, &block);
    if (retval < 0) {
        return NULL;
    }  

    memcpy(&(i->fileDesc), (int*)block, sizeof(int));
    block+= sizeof(int);
    memcpy(&(i->attrType), (char*)block, sizeof(char));
    block+= sizeof(char);
    memcpy(temp, (char*)block, 40*sizeof(char));
    strcpy(i->attrName, temp);
    block+= 40*sizeof(char);   
    memcpy(&(i->attrLength), (int*)block, sizeof(int));
    //BF_CloseFile(fileDesc);
    return i;
}

int HP_CloseFile(HP_info* header_info){


    BF_CloseFile(header_info->fileDesc);
    return 0;
}




int HP_InsertEntry(HP_info header_info, Record record) {
  void *block;
  BF_AllocateBlock(header_info.fileDesc);
  int blockCounter = BF_GetBlockCounter(header_info.fileDesc)-1;
  int retval;

  retval = BF_ReadBlock(header_info.fileDesc, blockCounter, &block);
  if (retval < 0) {
    return -1;
  }
  if (strcmp(record.name, "Kroba")) {
    block+=sizeof(Record);
  }
  
  block = memcpy(block, &record, sizeof(Record));
  retval = BF_WriteBlock(header_info.fileDesc, blockCounter);
  if (retval == 0) {
    BF_ReadBlock(header_info.fileDesc, blockCounter, &block);
    Record temp;
    memcpy(&temp, (Record *)block, sizeof(Record));
    printf("%s, %s\n", temp.name, temp.surname);

    return blockCounter;
  }

  else {
    retval = BF_AllocateBlock(header_info.fileDesc);
    if (retval < 0) return -1;

    blockCounter++;
    retval = BF_ReadBlock(header_info.fileDesc, blockCounter, &block);
    if (retval < 0) return -1;

    block = memcpy(block, &record, sizeof(Record));
    retval = BF_WriteBlock(header_info.fileDesc, blockCounter);
    if (retval < 0) return -1;
  }

  return blockCounter;

}
