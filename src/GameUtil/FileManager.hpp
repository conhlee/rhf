#ifndef GAMEUTIL_FILEMANAGER_HPP
#define GAMEUTIL_FILEMANAGER_HPP

#include <revolution/types.h>

#include <revolution/DVD.h>
#include <revolution/ARC.h>

#include <RVLFaceLibrary.h>

#include "Mem.hpp"

#include "Singleton.hpp"

class CFileManager;
extern CFileManager *gFileManager;

class CFileManager : public TSingleton<CFileManager> {
public:
    static const s32 DEFAULT_MAX_FILE_COUNT = 95;
    static const s32 DEFAULT_MAX_ARCHIVE_COUNT = 95;

public:
    virtual void _08(void);
    virtual ~CFileManager(void);
    virtual void _10(
        s32 maxFileCount = DEFAULT_MAX_FILE_COUNT,
        s32 maxArchiveCount = DEFAULT_MAX_ARCHIVE_COUNT
    );
    virtual void _14(void);

    CFileManager(void);

    void fn_801D3C2C(const char *localeDir);
    const char *fn_801D3C44(void);

    // NOTE: alignment must be a multiple of 32, or DVD read will fail!
    void *fn_801D3C4C(const char *path, EHeapMEM heap = eHeap_MEM2, s32 alignment = 32);
    bool fn_801D3D58(void);
    void fn_801D3D94(void);
    void fn_801D3E94(void);
    // NOTE: alignment must be a multiple of 32, or DVD read will fail!
    void fn_801D3F94(s32 arcIndex, const char *path, EHeapMEM heap = eHeap_MEM2, s32 alignment = 32);
    void fn_801D41CC(s32 arcIndex);
    u32 fn_801D422C(s32 arcIndex, const char *path);
    void *fn_801D4270(s32 arcIndex, const char *path);
    void *fn_801D42CC(s32 arcIndex);
    bool fn_801D42E0(s32 arcIndex);
    bool fn_801D42FC(s32 arcIndex);
    bool fn_801D431C(void);
    void fn_801D4364(s32 arcIndex);
    void fn_801D443C(void);
    void fn_801D4544(void);
    void fn_801D49D4(void);

    static void *fn_801D461C(void *data, BOOL deleteSrc, EHeapMEM heap = eHeap_MEM2, s32 alignment = 32);

    void setDVDErrorFuncF(void (*DVDErrorFuncF)(void)) {
        mDVDErrorFuncF = DVDErrorFuncF;
    }
    void setDVDErrorFuncB(void (*DVDErrorFuncB)(void)) {
        mDVDErrorFuncB = DVDErrorFuncB;
    }

    u32 getRFLResLen(void) {
        return fn_801D422C(1, RFLGetArcFilePath());
    }
    void *getRFLResAddr(void) {
        return fn_801D4270(1, RFLGetArcFilePath());
    }

    // TODO: come up with a better name
    static void waitLoadFinalizeTick(void) {
        gFileManager->fn_801D49D4(); // Check DVD errors, wait for them to be resolved if they appear ..
        gFileManager->fn_801D4544(); // Update archive processing (do decompression, relocation, etc.)
        OSSleepTicks(OS_MSEC_TO_TICKS(10ll));
    }

private:
    s32 getFileInfoIdx(DVDFileInfo *fileInfo) {
        s32 index = -1;
        for (s32 i = 0; i < mMaxFileCount; i++) {
            if (&mDVDFileInfo[i] == fileInfo) {
                index = i;
                break;
            }
        }
        return index;
    }

    void fn_801D412C(s32 result, DVDFileInfo *fileInfo);

    static void fn_801D392C(s32, DVDFileInfo *); // DVDAsyncCallback
    static void fn_801D3988(s32, DVDFileInfo *); // DVDAsyncCallback

    static void *fn_801D46A4(void *data, BOOL deleteSrc, s32 arcInfoIdx, EHeapMEM heap, s32 alignment);
    static void *fn_801D47B8(void *); // OSThreadFunc
    static bool fn_801D47F8(u8 *src, u8 *dst, u32 srcSize, u32 dstSize, s32 idx, BOOL deleteSrc);

public:
    enum EArchiveInfoState {
        eArchiveInfoState_Free = 0,          // Slot is unassigned and available.
        eArchiveInfoState_Loading = 1,       // Waiting for DVD read ..
        eArchiveInfoState_Loaded = 2,        // Waiting for decompression to start ..
        eArchiveInfoState_Decompressing = 3, // Waiting for decompression to finish ..
        eArchiveInfoState_Decompressed = 4,  // Waiting for ARCInitHandle ..
        eArchiveInfoState_Ready = 5          // Ready for access.
    };

    struct ArchiveInfo {
        DVDFileInfo *dvdFileInfo; // Will be NULL when ready for access.
        ARCHandle arcHandle;
        u8 *data;
        EHeapMEM heapType;
        u16 heapGroup;
        bool compressed; // The source file was compressed.
        EArchiveInfoState state;
    };

    struct UnusedFileSub {
        u8 _pad00[0x44];
    };

public:
    char mLocaleDir[4];

    s32 mMaxFileCount;
    DVDFileInfo *mDVDFileInfo; // DVDFileInfo[mMaxFileCount]
    UnusedFileSub *mUnusedFileSub; // UnusedFileSub[mMaxFileCount]
    bool *mDVDFileInfoActive; // bool[mMaxFileCount]; describes if a mDVDFileInfo slot at the specified index is in use

    s32 mMaxArchiveCount;
    ArchiveInfo *mArchiveInfo; // ArchiveInfo[mMaxArchiveCount]

    u8 _pad20[0x10];

    OSThread mThread;
    u32 *mThreadStack;

    void (*mDVDErrorFuncF)(void);
    void (*mDVDErrorFuncB)(void);
};

#endif
