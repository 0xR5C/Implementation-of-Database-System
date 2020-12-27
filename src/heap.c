#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "records.h"
#include "BF.h"
#include "HP.h"

int getMaxRecords(void) {
  return (BLOCK_SIZE - sizeof(int)*2)/sizeof(Record);
}

int HP_CreateFile(char *fileName, char attrType, char* attrName, int attrLength, int buckets) {
    int retval;
    //int first;
    int fileDesc;
    void* block;
    void* i;
    int pointer = -1;

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
    memcpy(i, (int*)&pointer, sizeof(int));
    i+= sizeof(int);
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

    block+= sizeof(int);
    memcpy(&(i->fileDesc), (int*)block, sizeof(int));
    block+= sizeof(int);
    memcpy(&(i->attrType), (char*)block, sizeof(char));
    block+= sizeof(char);
    memcpy(temp, (char*)block, 40*sizeof(char));
    strcpy(i->attrName, temp);
    block+= 40*sizeof(char);
    memcpy(&(i->attrLength), (int*)block, sizeof(int));
    return i;
}

int HP_CloseFile(HP_info* header_info){

    BF_CloseFile(header_info->fileDesc);
    free(header_info->attrName);
    free(header_info);

    return 0;
}

int HP_InsertEntry(HP_info header_info, Record record) {
  void *block;
  int blockCounter = BF_GetBlockCounter(header_info.fileDesc)-1;
  int retval;
  int pointer, counter;
  int i, j;
  Record *x;

  // Check if entry already exists
  if (blockCounter != 0) {
    for (i=1; i<=blockCounter; i++) {
      retval = BF_ReadBlock(header_info.fileDesc, i, &block);
      if (retval < 0) return -1;

      block += sizeof(int);
      memcpy(&counter, (int*)block, sizeof(int));
      block += sizeof(int);

      for (j=0; j<counter; j++) {
        x = (Record *)block;
        if (x->id == record.id) return -1;
        block += sizeof(Record);
      }
    }
  }

  // If file contains only one block
  if (blockCounter == 0) {
    BF_AllocateBlock(header_info.fileDesc);
    BF_ReadBlock(header_info.fileDesc, blockCounter, &block);
    pointer = blockCounter+1;
    memcpy(block, (int*)&pointer, sizeof(int));
    BF_WriteBlock(header_info.fileDesc, blockCounter);
    blockCounter++;
    BF_ReadBlock(header_info.fileDesc, blockCounter, &block);
    pointer = -1;
    counter = 0;
    memcpy(block, (int*)&pointer, sizeof(int));
    block += sizeof(int);
    memcpy(block, (int*)&counter, sizeof(int));
    BF_WriteBlock(header_info.fileDesc, blockCounter);
  }

  // Go through every block to find a spot to fill
  for (i=1; i<=blockCounter; i++) {
    retval = BF_ReadBlock(header_info.fileDesc, i, &block);
    if (retval < 0) return -1;

    block += sizeof(int);
    memcpy(&counter, (int*)block, sizeof(int));
    if (counter < getMaxRecords()) {
      counter++;
      memcpy(block, (int*)&counter, sizeof(int));
      block += sizeof(int);
      for (j=0; j<counter-1; j++) block += sizeof(Record);
      memcpy(block, &record, sizeof(Record));
      retval = BF_WriteBlock(header_info.fileDesc, i);
      if (retval < 0) return -1;
      return i;
    }
    BF_WriteBlock(header_info.fileDesc, blockCounter);
  }

  // If all allocated blocks full, allocate a new one
  retval = BF_ReadBlock(header_info.fileDesc, blockCounter, &block);
  if (retval < 0) return -1;

  BF_AllocateBlock(header_info.fileDesc);
  pointer = blockCounter+1;
  memcpy(block, (int*)&pointer, sizeof(int));
  BF_WriteBlock(header_info.fileDesc, blockCounter);
  blockCounter++;
  BF_ReadBlock(header_info.fileDesc, blockCounter, &block);
  pointer = -1;
  counter = 1;
  memcpy(block, (int*)&pointer, sizeof(int));
  block += sizeof(int);
  memcpy(block, (int*)&counter, sizeof(int));
  block += sizeof(int);
  memcpy(block, &record, sizeof(Record));
  BF_WriteBlock(header_info.fileDesc, blockCounter);
  return blockCounter;

}

int HP_DeleteEntry(HP_info header_info, void *value) {
  void* block;
  int blockCounter = BF_GetBlockCounter(header_info.fileDesc)-1;
  int retval;
  int pointer, counter, prev;
  Record rec;

  // no blocks allocated
  if (blockCounter == 0) {
      return -1;
  }

  pointer=1;

  while (pointer != -1) {
      retval= BF_ReadBlock(header_info.fileDesc, pointer, &block);
      if (retval < 0) {
          return -55;
      }

      prev = pointer;
      memcpy(&pointer, block, sizeof(int));
      block+= sizeof(int);
      memcpy(&counter, block, sizeof(int));
      block+= sizeof(int);
      int flag= 0;
      int i;
      for(i= 1; i<=counter; i++) {
          memcpy(&rec, block, sizeof(Record));
          if (strcmp(header_info.attrName, "id") == 0) {
              if (rec.id == *(int*)value) {
                  flag= 1;
                  break;
              }
          }
          else if (strcmp(header_info.attrName, "name") == 0) {
              if (strcmp(rec.name, (char*)value)) {
                  flag= 1;
                  break;
              }
          }
          else if (strcmp(header_info.attrName, "surname") == 0) {
              if (strcmp(rec.surname, (char*)value)) {
                  flag= 1;
                  break;
              }
          }
          else if (strcmp(header_info.attrName, "address") == 0) {
              if (strcmp(rec.address, (char*)value)) {
                  flag= 1;
                  break;
              }
          }
          block+= sizeof(Record);

      }
      // if the record is found
      if (flag == 1) {
          // if record isn't last in block
          if (i < counter) {
              Record tempRec;
              void* block2= block;
              block2+= sizeof(Record)*(counter-i);
              memcpy(&tempRec, block2, sizeof(Record));

              memcpy(block, &tempRec, sizeof(Record));
              retval= BF_WriteBlock(header_info.fileDesc, prev);
              if (retval < 0) {
                  return -1;
              }
          }

          retval= BF_ReadBlock(header_info.fileDesc, prev, &block);
          if (retval < 0) {
              return -1;
          }
          block+= sizeof(int);
          counter--;
          memcpy(block, &counter, sizeof(int));
          retval= BF_WriteBlock(header_info.fileDesc, prev);
          if (retval < 0) {
              return -1;
          }
          return 0;
      }
  }
  return -1;

}

int HP_GetAllEntries(HP_info header_info, void *value) {
  int blockCounter = BF_GetBlockCounter(header_info.fileDesc) - 1;
  int retval, i, j;
  int numOfBlocks = 0;
  void *block;
  int counter = 0;
  int found = 0;
  Record *x;

  if (blockCounter == 0) return -1;

  for (i=1; i<=blockCounter; i++) {
    numOfBlocks++;

    retval = BF_ReadBlock(header_info.fileDesc, i, &block);
    if (retval < 0) return -1;

    block += sizeof(int);
    memcpy(&counter, (int*)block, sizeof(int));
    block += sizeof(int);

    for (j=0; j<counter; j++) {
      x = (Record *)block;
      if (x->id == *(int *)value) {
        found++;
        printf("Found entry in block %d!\n", i);
        printf("ID: %d, Name: %s, Surname: %s, Address: %s\n"
        , x->id, x->name, x->surname, x->address);
      }
      block += sizeof(Record);
    }
  }
  if (found > 0) {
    printf("Blocks read: %d\n", numOfBlocks);
    return numOfBlocks;
  }
  else {
    printf("Entry not found!\n");
    printf("Blocks read: %d\n", numOfBlocks);
    return -1;
  }

}
