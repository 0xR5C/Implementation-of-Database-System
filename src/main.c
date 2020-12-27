#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "records.h"
#include "HP.h"


int main(int argc, char* argv[]) {
    HP_info* i;
    char attr[]= "id"; 
    char temp[200];
    char temp2[150];
    int id;
    char delim[]= "\",\"";

    Record rec;
    int ret;
    FILE* fp;

    HP_CreateFile("test", 'c', attr, strlen(attr)+1, 5);
    i= HP_OpenFile("test");
    fp= fopen(argv[1], "r");
    while(fscanf(fp, "%s\n", temp) != EOF) {
        sscanf(temp, "{%d,\"%s\"}", &rec.id, temp2);
        strcpy(rec.name, strtok(temp2, delim));
        strcpy(rec.surname, strtok(NULL, delim));    
        strcpy(rec.address, strtok(NULL, delim));    
        ret= HP_InsertEntry(*i, rec);
        if (ret < 0) {
            printf("wrong\n");
            return -1;
        }

        
    }
    id= 0;
    ret= HP_GetAllEntries(*i, &id);
    printf("--%d\n", ret);

    id= 6;
    ret= HP_GetAllEntries(*i, &id);
    printf("--%d\n", ret);    

    id= 66;
    ret= HP_GetAllEntries(*i, &id);
    printf("--%d\n", ret);   

    id= 6;
    ret= HP_DeleteEntry(*i, &id);
    printf("===%d\n", ret);    

    ret= HP_GetAllEntries(*i, &id);
    printf("--%d\n", ret);

    id= 0;
    ret= HP_DeleteEntry(*i, &id);
    printf("===%d\n", ret);    

    ret= HP_GetAllEntries(*i, &id);
    printf("--%d\n", ret);    

    id= 66;
    ret= HP_GetAllEntries(*i, &id);
    printf("--%d\n", ret);       

    rec.id= 11111; strcpy(rec.name, "iiiii"); strcpy(rec.surname, "IIIII"); strcpy(rec.address, "PPPPPP");

    ret= HP_InsertEntry(*i, rec);
    printf("===%d\n", ret);

    ret= HP_GetAllEntries(*i, &rec.id);
    printf("--%d\n", ret);     

    fclose(fp);
    HP_CloseFile(i);

    return 0;
}
