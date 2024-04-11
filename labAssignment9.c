#include <stdio.h>
#include <stdlib.h> // for malloc, free

// RecordType
struct RecordType
{
    int     id;
    char    name;
    int     order; 
};

// HashNode
struct HashNode
{
    struct RecordType data;
    struct HashNode* next;
};

// HashType
struct HashType
{
    struct HashNode* head;
};

// Compute the hash function
int hash(int x, int hashSz)
{
    return x % hashSz;
}

// Parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// Prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// Initializes the hash table
struct HashType* createHashTable(int hashSz)
{
    struct HashType* hashTable = (struct HashType*)malloc(sizeof(struct HashType) * hashSz);
    if (hashTable == NULL)
    {
        printf("Cannot allocate memory\n");
        exit(-1);
    }
    int i;
    for (i = 0; i < hashSz; ++i)
    {
        hashTable[i].head = NULL;
    }
    return hashTable;
}

// Inserts a record into the hash table
void insertRecord(struct HashType* hashTable, int hashSz, struct RecordType record)
{
    int index = hash(record.id, hashSz);
    struct HashNode* newNode = (struct HashNode*)malloc(sizeof(struct HashNode));
    if (newNode == NULL)
    {
        printf("Cannot allocate memory\n");
        exit(-1);
    }
    newNode->data = record;
    newNode->next = hashTable[index].head;
    hashTable[index].head = newNode;
}

// Displays records in the hash structure
void displayRecordsInHash(struct HashType* hashTable, int hashSz)
{
    int i;
    printf("\nRecords in Hash Table:\n");
    for (i = 0; i < hashSz; ++i)
    {
        printf("Index %d -> ", i);
        struct HashNode* current = hashTable[i].head;
        while (current != NULL)
        {
            printf("%d, %c, %d -> ", current->data.id, current->data.name, current->data.order);
            current = current->next;
        }
        printf("\n");
    }
    printf("\n\n");
}

int main(void)
{
    struct RecordType* pRecords;
    int recordSz = 0;
    int hashSize = 10; // Choose your hash size

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    struct HashType* hashTable = createHashTable(hashSize);
    int i;
    for (i = 0; i < recordSz; ++i)
    {
        insertRecord(hashTable, hashSize, pRecords[i]);
    }
    displayRecordsInHash(hashTable, hashSize);

    // Free allocated memory
    free(pRecords);
    free(hashTable);

    return 0;
}
