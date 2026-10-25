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
    static const EHeapMEM DEFAULT_HEAP = eHeap_MEM2;
    static const s32 DEFAULT_ALIGN = 32;

public:
    virtual void _08(void);
    virtual ~CFileManager(void);
    virtual void _10(s32 maxFileCount, s32 maxArchiveCount);
    virtual void _14(void);

    CFileManager(void);

    void setLocaleDir(const char *localeDir);
    const char *getLocaleDir(void);

    // NOTE: Alignment must be a power-of-two value >= 32 (or <= -32).
    void *loadFile(
        const char *path,
        EHeapMEM heap = DEFAULT_HEAP, s32 alignment = DEFAULT_ALIGN
    );

    bool getDVDBusy(void);
    void waitDVD(void);
    void waitDVD2(void); // NOTE: functionally equivalent to waitDVD

    // NOTE: Alignment must be a power-of-two value >= 32 (or <= -32).
    void startArchive(
        u16 arcIndex, const char *path,
        EHeapMEM heap = DEFAULT_HEAP, s32 alignment = DEFAULT_ALIGN
    );
    void endArchive(u16 arcIndex);

    u32 arcGetFileLen(u16 arcIndex, const char *path);
    void *arcGetFileAddr(u16 arcIndex, const char *path);
    void *arcGetAddr(u16 arcIndex);
    bool getArcFree(u16 arcIndex);
    bool getArcReady(u16 arcIndex);
    bool getArcIdle(void);
    void waitArc(u16 arcIndex);
    void waitAllArc(void);

    void updateArc(void);
    void updateDVD(void);

    void setDVDErrorFuncF(void (*DVDErrorFuncF)(void)) {
        mDVDErrorFuncF = DVDErrorFuncF;
    }
    void setDVDErrorFuncB(void (*DVDErrorFuncB)(void)) {
        mDVDErrorFuncB = DVDErrorFuncB;
    }

    u32 getRFLResLen(void) {
        return arcGetFileLen(1, RFLGetArcFilePath());
    }
    void *getRFLResAddr(void) {
        return arcGetFileAddr(1, RFLGetArcFilePath());
    }

    static void *expandSZS(
        void *data, BOOL deleteSrc,
        EHeapMEM heap = DEFAULT_HEAP, s32 alignment = DEFAULT_ALIGN
    );

    static void waitTick(void) {
        gFileManager->updateDVD();
        gFileManager->updateArc();
        OSSleepTicks(OS_MSEC_TO_TICKS(10ll));
    }

private:
    s32 getFileInfoIndex(DVDFileInfo *fileInfo) {
        s32 index = -1;
        for (s32 i = 0; i < mMaxFileCount; i++) {
            if (&mDVDFileInfo[i] == fileInfo) {
                index = i;
                break;
            }
        }
        return index;
    }

    void dvdCallbackArcImpl(s32 result, DVDFileInfo *fileInfo);

    static void dvdCallbackFile(s32 result, DVDFileInfo *fileInfo);
    static void dvdCallbackArc(s32 result, DVDFileInfo *fileInfo);

    static void *tryExpandTask(void *data, BOOL deleteSrc, s32 arcInfoIdx, EHeapMEM heap, s32 alignment);
    static void *expandTaskFunc(void *);

    static bool expandSZSImpl(u8 *src, u8 *dst, u32 expandSize, u32 dstSize, s16 arcInfoIdx, BOOL deleteSrc);

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
        u16 memGroup;
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
