
typedef struct {
    int fileDesc;
    char attrType;
    char* attrName;
    int attrLength;
    int numBuckets;
} HT_info;

int HT_CreateIndex(char* fileName, char attrType, char* attrName, int attrLength, int buckets);

HT_info* HT_OpenIndex(char* fileName);

int HT_CloseIndex(HT_info* header_info);