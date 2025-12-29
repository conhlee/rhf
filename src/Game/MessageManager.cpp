#include "MessageManager.hpp"

#include "FileManager.hpp"

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



