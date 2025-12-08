#include "FileManager.hpp"

#include "GameManager.hpp"

#include <string.h>
#include <stdio.h>

static char sDVDPathBuffer[64];

void CFileManager::fn_801D392C(s32 result, DVDFileInfo *fileInfo) {
    s32 index = gFileManager->getFileInfoIdx(fileInfo);
    if (result == -1) {
        return;
    }
    gFileManager->mDVDFileInfoActive[index] = false;
}

void CFileManager::fn_801D3988(s32 result, DVDFileInfo *fileInfo) {
    gFileManager->fn_801D412C(result, fileInfo);
}

CFileManager::CFileManager(void) {}
CFileManager::~CFileManager(void) {
    _08();
}

#define THREAD_STACK_SIZE (0xA00) // In words.

void CFileManager::_10(s32 maxFileCount, s32 maxArchiveCount) {
    mMaxArchiveCount = maxArchiveCount;
    mMaxFileCount = maxFileCount;

    mDVDFileInfo = new (eHeap_MEM2, 32) DVDFileInfo[mMaxFileCount];
    mUnusedFileSub = new (eHeap_MEM2, 32) UnusedFileSub[mMaxFileCount];
    mDVDFileInfoActive = new (eHeap_MEM2, 32) bool[mMaxFileCount];
    mArchiveInfo = new (eHeap_MEM2, 32) ArchiveInfo[mMaxArchiveCount];
    mThreadStack = new (eHeap_MEM2, 32) u32[THREAD_STACK_SIZE];
}

void CFileManager::_08(void) {
    delete[] mDVDFileInfo;
    delete[] mUnusedFileSub;
    delete[] mDVDFileInfoActive;
    delete[] mArchiveInfo;

    // @bug mThreadStack is never freed!
    // delete[] mThreadStack;
}

void CFileManager::_14(void) {
    DVDInit();

    for (s32 i = 0; i < mMaxFileCount; i++) {
        mDVDFileInfoActive[i] = false;
    }
    for (s32 i = 0; i < mMaxArchiveCount; i++) {
        mArchiveInfo[i].dvdFileInfo = NULL;
        mArchiveInfo[i].data = NULL;
        mArchiveInfo[i].state = eArchiveInfoState_Free;
    }

    mDVDErrorFuncF = NULL;
    mDVDErrorFuncB = NULL;
}

void CFileManager::fn_801D3C2C(const char *localeDir) {
    sprintf(mLocaleDir, "%s", localeDir);
}

const char *CFileManager::fn_801D3C44(void) {
    return mLocaleDir;
}

void *CFileManager::fn_801D3C4C(const char *path, EHeapMEM heap, s32 alignment) {
    sprintf(sDVDPathBuffer, "%s%s", gFileManager->fn_801D3C44(), path);

    s32 dfiIndex = -1;
    for (s32 i = 0; i < mMaxFileCount; i++) {
        if (mDVDFileInfoActive[i] == false) {
            dfiIndex = i;
            break;
        }
    }

    DVDOpen(sDVDPathBuffer, &mDVDFileInfo[dfiIndex]);

    u32 allocSize = ROUND_UP(mDVDFileInfo[dfiIndex].length, 32);
    void *alloc = new (heap, alignment) u8[allocSize];
    DCInvalidateRange(alloc, allocSize);

    DVDReadAsyncPrio(&mDVDFileInfo[dfiIndex], alloc, allocSize, 0, fn_801D392C, 2);
    mDVDFileInfoActive[dfiIndex] = true;

    return alloc;
}

bool CFileManager::fn_801D3D58(void) {
    for (s32 i = 0; i < mMaxFileCount; i++) {
        if (mDVDFileInfoActive[i]) {
            return true;
        }
    }
    return false;
}

void CFileManager::fn_801D3D94(void) {
    while (fn_801D3D58()) {
        waitLoadFinalizeTick();
    }
}

void CFileManager::fn_801D3E94(void) {
    while (fn_801D3D58()) {
        waitLoadFinalizeTick();
    }
}

void CFileManager::fn_801D3F94(s32 arcIndex, const char *path, EHeapMEM heap, s32 alignment) {
    sprintf(sDVDPathBuffer, "%s%s", gFileManager->fn_801D3C44(), path);

    s32 dfiIndex = -1;
    for (s32 i = 0; i < mMaxFileCount; i++) {
        if (mDVDFileInfoActive[i] == false) {
            dfiIndex = i;
            break;
        }
    }

    mArchiveInfo[arcIndex].dvdFileInfo = &mDVDFileInfo[dfiIndex];
    mArchiveInfo[arcIndex].heapType = heap;
    mArchiveInfo[arcIndex].heapGroup = fn_801D363C();

    const char *compressedExt = ".szs";
    mArchiveInfo[arcIndex].compressed = strstr(sDVDPathBuffer, compressedExt) != NULL;

    mArchiveInfo[arcIndex].state = eArchiveInfoState_Loading;

    DVDOpen(sDVDPathBuffer, &mDVDFileInfo[dfiIndex]);

    u32 allocSize = ROUND_UP(mDVDFileInfo[dfiIndex].length, 32);
    mArchiveInfo[arcIndex].data = new (mArchiveInfo[arcIndex].heapType, alignment) u8[allocSize];
    DCInvalidateRange(mArchiveInfo[arcIndex].data, allocSize);

    DVDReadAsyncPrio(&mDVDFileInfo[dfiIndex], mArchiveInfo[arcIndex].data, allocSize, 0, fn_801D3988, 2);
    mDVDFileInfoActive[dfiIndex] = true;
}

void CFileManager::fn_801D412C(s32 result, DVDFileInfo *fileInfo) {
    s32 index = -1;
    for (s32 i = 0; i < mMaxFileCount; i++) {
        if (&mDVDFileInfo[i] == fileInfo) {
            index = i;
            break;
        }
    }

    if (result == DVD_RESULT_FATAL_ERROR) {
        return;
    }

    mDVDFileInfoActive[index] = false;
    for (s32 i = 0; i < mMaxArchiveCount; i++) {
        if (mArchiveInfo[i].dvdFileInfo == fileInfo) {
            mArchiveInfo[i].state = eArchiveInfoState_Loaded;
            return;
        }
    }
}

void CFileManager::fn_801D41CC(s32 arcIndex) {
    delete[] mArchiveInfo[arcIndex].data;
    mArchiveInfo[arcIndex].data = NULL;

    mArchiveInfo[arcIndex].state = eArchiveInfoState_Free;
}

u32 CFileManager::fn_801D422C(s32 arcIndex, const char *path) {
    ARCFileInfo fileInfo;
    ARCOpen(&mArchiveInfo[arcIndex].arcHandle, path, &fileInfo);

    u32 length = ARCGetLength(&fileInfo);

    // ARCClose call is missing here! Luckily, ARCClose doesn't actually do anything ..
    // ARCClose(&fileInfo);
    return length;
}

void *CFileManager::fn_801D4270(s32 arcIndex, const char *path) {
    ARCFileInfo fileInfo;
    ARCOpen(&mArchiveInfo[arcIndex].arcHandle, path, &fileInfo);

    void *startAddr = ARCGetStartAddrInMem(&fileInfo);

    ARCClose(&fileInfo);
    return startAddr;
}

void *CFileManager::fn_801D42CC(s32 arcIndex) {
    return mArchiveInfo[arcIndex].data;
}

bool CFileManager::fn_801D42E0(s32 arcIndex) {
    return mArchiveInfo[arcIndex].state == eArchiveInfoState_Free;
}

bool CFileManager::fn_801D42FC(s32 arcIndex) {
    return mArchiveInfo[arcIndex].state == eArchiveInfoState_Ready;
}

bool CFileManager::fn_801D431C(void) {
    bool isIdle = true;

    // @bug mMaxArchiveCount should be used instead of mMaxFileCount
    for (s32 i = 0; i < mMaxFileCount; i++) {
        if (
            (mArchiveInfo[i].state != eArchiveInfoState_Free) &&
            (mArchiveInfo[i].state != eArchiveInfoState_Ready)
        ) {
            isIdle = false;
        }
    }
    return isIdle;
}

void CFileManager::fn_801D4364(s32 arcIndex) {
    while (mArchiveInfo[arcIndex].state != eArchiveInfoState_Ready) {
        waitLoadFinalizeTick();
    }
}

void CFileManager::fn_801D443C(void) {
    while (!fn_801D431C()) {
        waitLoadFinalizeTick();
    }
}

void CFileManager::fn_801D4544(void) {
    for (s16 i = 0; i < mMaxArchiveCount; i++) {
        ArchiveInfo *archiveInfo = &mArchiveInfo[i];
        bool shouldArcInit = false;

        if (archiveInfo->state == eArchiveInfoState_Loaded) {
            if (archiveInfo->compressed) {
                // NOTE: this returns the decompressed buffer before the decompression is actually finished.
                //       if NULL is returned, the decompression thread is still busy; we'll try again next time.
                void *decompDst = fn_801D46A4(archiveInfo->data, TRUE, i, archiveInfo->heapType, -32);
                if (decompDst != NULL) {
                    archiveInfo->data = static_cast<u8 *>(decompDst);
                    archiveInfo->state = eArchiveInfoState_Decompressing;
                }
            }
            else {
                shouldArcInit = true;
            }
        }
        else if (archiveInfo->state == eArchiveInfoState_Decompressed) {
            shouldArcInit = true;
        }

        if (shouldArcInit) {
            ARCInitHandle(archiveInfo->data, &archiveInfo->arcHandle);
            archiveInfo->dvdFileInfo = NULL;
            archiveInfo->state = eArchiveInfoState_Ready;
        }
    }
}

#define getSZSExpandSize(src) ( \
    (((u8 *)(src))[4] << 24) | (((u8 *)(src))[5] << 16) | \
    (((u8 *)(src))[6] << 8) | (((u8 *)(src))[7] << 0) \
)

void *CFileManager::fn_801D461C(void *data, BOOL deleteSrc, EHeapMEM heap, s32 alignment) {
    s32 expandSize = getSZSExpandSize(data);

    u32 dstSize = ROUND_UP(expandSize, 32);
    u8 *dst = new (heap, alignment) u8[dstSize];

    fn_801D47F8(static_cast<u8 *>(data), dst, expandSize, dstSize, -1, deleteSrc);
    return dst;
}

struct DecompThreadData {
    void *src;
    void *dst;
    u32 expandSize;
    u32 dstSize;
    s16 arcInfoIdx; // Index into gFileManager->mArchiveInfo.
    u8 deleteSrc; // If nonzero, src will be deleted when decompression finishes.
};
static DecompThreadData sDecompThreadData;

void *CFileManager::fn_801D46A4(void *data, BOOL deleteSrc, s32 arcInfoIdx, EHeapMEM heap, s32 alignment) {
    BOOL threadInactive = OSIsThreadTerminated(&gFileManager->mThread);
    if (threadInactive) {
        u32 expandSize = getSZSExpandSize(data);
        u32 dstSize = ROUND_UP(expandSize, 32);

        if (arcInfoIdx >= 0) {
            fn_801D369C(gFileManager->mArchiveInfo[arcInfoIdx].heapGroup);
        }
        void *dst = new (heap, alignment) u8[dstSize];
        if (arcInfoIdx >= 0) {
            fn_801D3644();
        }

        sDecompThreadData.src = data;
        sDecompThreadData.dst = dst;
        sDecompThreadData.expandSize = expandSize;
        sDecompThreadData.dstSize = dstSize;
        sDecompThreadData.arcInfoIdx = arcInfoIdx;
        sDecompThreadData.deleteSrc = deleteSrc;

        OSCreateThread(
            &gFileManager->mThread,
            fn_801D47B8, &sDecompThreadData,
            &gFileManager->mThreadStack[THREAD_STACK_SIZE], THREAD_STACK_SIZE * sizeof(u32),
            OS_PRIORITY_MAX, OS_THREAD_DETACHED
        );
        OSResumeThread(&gFileManager->mThread);

        return dst;
    }

    return NULL;
}

void *CFileManager::fn_801D47B8(void *arg) {
    DecompThreadData *data = static_cast<DecompThreadData *>(arg);

    fn_801D47F8(
        static_cast<u8 *>(data->src), static_cast<u8 *>(data->dst),
        data->expandSize, data->dstSize, data->arcInfoIdx, data->deleteSrc
    );
    return NULL;
}

// Yaz0 (SZS) decompression.
bool CFileManager::fn_801D47F8(u8 *src, u8 *dst, u32 expandSize, u32 dstSize, s32 arcInfoIdx, BOOL deleteSrc) {
    DCInvalidateRange(dst, dstSize);

    s32 srcIdx = 0x10;
    u8 mask = 0;
    u8 opByte;

    for (s32 dstIdx = 0; dstIdx < expandSize; mask >>= 1) {
        if (mask == 0) {
            mask = (1 << 7);
            opByte = src[srcIdx++];
        }

        if (opByte & mask) {
            dst[dstIdx++] = src[srcIdx++];
        }
        else {
            s32 packed = src[srcIdx] << 8 | src[srcIdx + 1];
            srcIdx += 2;

            s32 runIdx = dstIdx - (packed & 0x0FFF);
            s32 runLen = ((packed >> 12) == 0) ?
                (src[srcIdx++] + 0xF + 3) :
                ((packed >> 12) + 2);

            for (; runLen > 0; runLen--, dstIdx++, runIdx++) {
                dst[dstIdx] = dst[runIdx - 1];
            }
        }
    }

    if (deleteSrc) {
        delete[] src;
    }

    DCFlushRange(dst, dstSize);

    if (arcInfoIdx >= 0) {
        gFileManager->mArchiveInfo[arcInfoIdx].state = eArchiveInfoState_Decompressed;
    }

    return false;
}

void CFileManager::fn_801D49D4(void) {
    s32 driveStatus = DVDGetDriveStatus();
    switch (driveStatus) {
    case DVD_STATE_NO_DISK:
    case DVD_STATE_WRONG_DISK:
    case DVD_STATE_RETRY:
        if (mDVDErrorFuncF) {
            mDVDErrorFuncF();
        }
        CGameManager::fn_801D7538(driveStatus);
        if (mDVDErrorFuncB) {
            mDVDErrorFuncB();
        }
        break;

    default:
        break;
    }
}
