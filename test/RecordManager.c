#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef enum {
    EN_RECORD_CALL = 0,
    EN_RECORD_AUDIO,
    EN_RECORD_VIDEO,
    EN_RECORD_AUX,
    EN_RECORD_EVENT,
    EN_RECORD_BUTT
} EN_RECORD_TYPE;

typedef struct {
    char        aucFilename[256];   // 文件名
    FILE        *pFile;             // 文件指针
    uint32_t    uiType;             // 文件类型
    uint32_t    uiOwner;            // 所属
} RecordInfo;

typedef struct _RecordInfoNode{
    RecordInfo              stRecord;
    struct _RecordInfoNode *next;
} RecordInfoNode;

typedef RecordInfoNode RecordInfoList;

typedef struct {
    RecordInfoNode  *pstWorkListHead;
    RecordInfoNode  *pstBackListHead;
} FileManager;

FileManager gstFileManager = {0};

void AddRecordToList(RecordInfoNode** li, RecordInfoNode* n) {
    RecordInfoNode* pstTmp = 0;
    if(!li || !n) return;
    if(!*li) {
        *li = n;
        return;
    }
    pstTmp = *li;
    while(pstTmp->next) {
        pstTmp = pstTmp->next;
    }
    n->next = 0;
    pstTmp->next = n;
}

static int g_cnt = 0;
int GetNewRecordFileName(char* buf, size_t size) {
    snprintf(buf, size, "b%010u.wtf", g_cnt++);
    return 0;
}

void InitFileManager() {
    memset_s(&gstFileManager, sizeof(FileManager), 0, sizeof(FileManager));
}

int ProcessRecord(uint32_t uiRecordType, uint32_t uiOwner, void* pData, uint32_t uiSize) {
    RecordInfoNode *pstRecord = 0, *pstLastRecord = 0, *pstTmp;
    if(uiRecordType >= EN_RECORD_BUTT) {
        return 1;
    }
    // 查找对应的record
    pstTmp = gstFileManager.pstWorkListHead;
    while(pstTmp) {
        if(pstTmp->stRecord.uiType == uiRecordType && pstTmp->stRecord.uiOwner == uiOwner) {
            pstRecord = pstTmp;
        }
        pstLastRecord = pstTmp;
        pstTmp = pstTmp->next;
    }
    if(!pstRecord) {
        // 没有对应的记录就创建
        pstRecord = (RecordInfoNode*)malloc(sizeof(RecordInfoNode));
        if(!pstRecord) {
            return 1;
        }
        memset_s(pstRecord, sizeof(RecordInfoNode), 0, sizeof(RecordInfoNode));
        if(!pstLastRecord) {
            gstFileManager.pstWorkListHead = pstRecord;
        } else {
            pstLastRecord->next = pstRecord;
        }

        memset_s(pstRecord, sizeof(RecordInfoNode), 0, sizeof(RecordInfoNode));
        pstRecord->stRecord.uiType = uiRecordType;
        pstRecord->stRecord.uiOwner = uiOwner;
        GetNewRecordFileName(pstRecord->stRecord.aucFilename, sizeof(pstRecord->stRecord.aucFilename));
        printf("create file\n");

        pstRecord->stRecord.pFile = fopen(pstRecord->stRecord.aucFilename, "wb+");
        if(!pstRecord->stRecord.pFile) {
            // keep null
            printf("can't open file\n");
            return 1;
        }
    }
   
    fwrite(pData, uiSize, 1, pstRecord->stRecord.pFile);
    printf("proc record for type %u owner %u file %s\n", uiRecordType, uiOwner, pstRecord->stRecord.aucFilename);
    return 0;
}

int ReportRecord(uint32_t uiOwner) {
    RecordInfoNode *pstRecord = 0, *pstPreRecord = 0, *pstTmp = 0;
    pstRecord = gstFileManager.pstWorkListHead;
    if(!pstRecord) return 0;

    while(pstRecord) {
        if(pstRecord->stRecord.uiOwner == uiOwner) {
            // process and remove file
            if(pstRecord->stRecord.pFile) {
                fclose(pstRecord->stRecord.pFile);
                pstRecord->stRecord.pFile = 0;
                remove(pstRecord->stRecord.aucFilename);
                printf("remove file %s!\n", pstRecord->stRecord.aucFilename);
                pstTmp = pstRecord->next;
                AddRecordToList(&gstFileManager.pstBackListHead, pstRecord);
                // add to back list
                printf("zip file(%s) to %u\n", pstRecord->stRecord.aucFilename, pstRecord->stRecord.uiOwner);
            } else {
                printf("file %s is not open!\n", pstRecord->stRecord.aucFilename);
            }
            if(pstPreRecord) {
                pstPreRecord->next = pstTmp;
            } else {
                gstFileManager.pstWorkListHead = pstTmp;
                pstRecord = pstTmp;
                continue;
            }
        }
        pstPreRecord = pstRecord;
        pstRecord = pstTmp;
    }

    return 0;
}

int main() {
    char ss[] = {0xAA, 0xBB, 0xCC, 0xDD};
    InitFileManager();
    ProcessRecord(0, 1, ss, sizeof(ss));
    ProcessRecord(1, 1, ss, sizeof(ss));
    ProcessRecord(2, 1, ss, sizeof(ss));
    
    ProcessRecord(0, 2, ss, sizeof(ss));
    ProcessRecord(1, 2, ss, sizeof(ss));
    ProcessRecord(2, 2, ss, sizeof(ss));

    ProcessRecord(1, 2, ss, sizeof(ss));

    //ReportRecord(0);
    ReportRecord(1);
    return 0;
}