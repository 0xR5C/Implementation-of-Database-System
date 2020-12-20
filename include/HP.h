typedef struct {
    int fileDesc;
    char attrType;
    char* attrName;
    int attrLength;
} HP_info;

typedef struct {
    int id;
    char name[15];
    char surname[25];
    char adress[50];
} Record;

int HP_CreateFile(char *fileName, char attrType, char* attrName, int attrLength, int buckets);

HP_info* HP_OpenFile(char* fileName);

int HP_CloseFile(HP_info* header_info);

int HP_InsertEntry(HP_info header_info, Record record);



