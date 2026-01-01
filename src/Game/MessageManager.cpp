#include "MessageManager.hpp"

#include <revolution/SC.h>

#include "FileManager.hpp"

struct BMGBlockHeader {
    u32 type;
    u32 size;

    BMGBlockHeader *getNext(void) {
        return reinterpret_cast<BMGBlockHeader *>(
            reinterpret_cast<u8 *>(this) + size
        );
    }
};

#define BMG_MAKE_ID(char1, char2, char3, char4) \
    (((u32)(char1) << 24) | ((u32)(char2) << 16) | ((u32)(char3) << 8) | (u32)(char4))

enum {
    BMG_BLOCK_INF1 = BMG_MAKE_ID('I','N','F','1'),
    BMG_BLOCK_DAT1 = BMG_MAKE_ID('D','A','T','1'),
    BMG_BLOCK_STR1 = BMG_MAKE_ID('S','T','R','1'),
    BMG_BLOCK_MID1 = BMG_MAKE_ID('M','I','D','1'),
};

struct BMGFileHeader {
    enum EEncoding {
        ENCODING_1BYTE,
        ENCODING_2BYTE,
        ENCODING_SJIS,
        ENCODING_UTF8,
    };

    u32 signature;
    u32 type;
    u32 dataSize;
    u32 numBlocks;

    u8 mEncoding; // EEncoding

    u8 mUnk11[0x20 - 0x11];

    BMGBlockHeader mFirstBlock[1];
};

struct BMGMessageInfo {
    struct Entry {
        u32 dataOffset;
        u32 attribute;
    };

    u16 numEntries;
    u16 entrySize;
    u16 groupID;
    Entry entries[1];
};

struct BMGMessageData {
    wchar_t data[1];
};

struct BMGStringAttributeInfo {};

struct BMGMessageIDInfo {
    u16 numEntries;
    u8 form;
    u8 formSupplement;
    u8 reserved[4];
    u32 entries[];
};

CBMGRes::CBMGRes(void) {
    mData = NULL;

    mFileHeader = NULL;
    mMessageInfo = NULL;
    mMessageData = NULL;
    mStrAttributeInfo = NULL;
    mMessageIDInfo = NULL;
}

CBMGRes::~CBMGRes(void) {}

void CBMGRes::init(u32 arcIndex, const char *pathInArc) {
    // TODO: does fn_801D4270 actually accept a u16 instead of a s32?
    mData = gFileManager->fn_801D4270((u16)arcIndex, pathInArc);

    mFileHeader = static_cast<BMGFileHeader *>(mData);

    BMGBlockHeader *currentBlock = mFileHeader->mFirstBlock;
    for (u32 i = 0; i < mFileHeader->numBlocks; i++) {
        void *blockContent = reinterpret_cast<u8 *>(currentBlock) + sizeof(BMGBlockHeader);

        switch (currentBlock->type) {
        case BMG_BLOCK_INF1:
            mMessageInfo = static_cast<BMGMessageInfo *>(blockContent);
            break;
        case BMG_BLOCK_DAT1:
            mMessageData = static_cast<BMGMessageData *>(blockContent);
            break;
        case BMG_BLOCK_STR1:
            mStrAttributeInfo = static_cast<BMGStringAttributeInfo *>(blockContent);
            break;
        case BMG_BLOCK_MID1:
            mMessageIDInfo = static_cast<BMGMessageIDInfo *>(blockContent);
            break;
        }

        currentBlock = currentBlock->getNext();
    }
}

// TODO: match (regswaps)
wchar_t *CBMGRes::getString(u32 id) {
    if (id == MESG_ID_INVALID) {
        return L"???";
    }
    else {
        u32 i;
        for (i = 0; i < mMessageIDInfo->numEntries; i++) {
            if (id == mMessageIDInfo->entries[i]) {
                break;
            }
        }

        if (i < mMessageIDInfo->numEntries) {
            wchar_t *string = mMessageData->data + (mMessageInfo->entries[i].dataOffset / sizeof(wchar_t));
            if (wcslen(string) != 0) {
                return string;
            }
            else {
                return L"???";
            }
        }
        else {
            return L"???";   
        }
    }
}

// TODO: match (logic issues)
s32 CMessageManager::fn_80087708(const char *string) {
    s32 finalValue = 0;

    if (string[0] < '0' || string[0] > '9') {
        return MESG_ID_INVALID;
    }
    finalValue = (string[0] - '0') * 1000000;

    if (string[1] < '0' || string[1] > '9') {
        return MESG_ID_INVALID;
    }
    finalValue += (string[1] - '0') * 100000;

    if (string[2] < '0' || string[2] > '9') {
        return MESG_ID_INVALID;
    }
    finalValue += (string[2] - '0') * 10000;
    
    if (string[3] < '0' || string[3] > '9') {
        return MESG_ID_INVALID;
    }
    finalValue += (string[3] - '0') * 1000;

    if (string[4] < '0' || string[4] > '9') {
        return MESG_ID_INVALID;
    }
    finalValue += (string[4] - '0') * 100;

    if (string[5] < '0' || string[5] > '9') {
        return MESG_ID_INVALID;
    }
    finalValue += (string[5] - '0') * 10;

    if (string[6] < '0' || string[6] > '9') {
        return MESG_ID_INVALID;
    }
    finalValue += (string[6] - '0');

    return finalValue;
}

CMessageManager::CMessageManager(void) {}

CMessageManager::~CMessageManager(void) {}

void CMessageManager::fn_80088030(void) {}

void CMessageManager::fn_80088034(void) {
    switch (SCGetLanguage()) {
    default:
        gFileManager->fn_801D3F94(0, "content2/message/riq_E.szs");
        gFileManager->fn_801D4364(0);
        break;
    }
}

void CMessageManager::fn_8008807C(void) {
    gFileManager->fn_801D41CC(0);
}

