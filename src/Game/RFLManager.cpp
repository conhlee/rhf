#include "RFLManager.hpp"

#include "FileManager.hpp"

#include "Mem.hpp"

#include <cstring> // for memcmp

CRFLManager::CRFLManager(void) {
    mIconsInitialized = false;
}

CRFLManager::~CRFLManager(void) {
    _08();
}

// Calculate size of RGB5A3 image data (16bpp).
#define IMAGE_BYTE_SIZE_16(width, height) \
    ((((width) + 3) / 4) * (((height) + 3) / 4) * 32u)

void CRFLManager::_10(void) {
    mIcons = new (eHeap_MEM2, 32) Icon[MAX_ICON_COUNT];
    for (s32 i = 0; i < MAX_ICON_COUNT; i++) {
        mIcons[i].texData = new (eHeap_MEM2, 32) u8[IMAGE_BYTE_SIZE_16(ICON_WIDTH, ICON_HEIGHT)];
    }

    mIconsInitialized = true;
}

void CRFLManager::_08(void) {
    for (s32 i = 0; i < MAX_ICON_COUNT; i++) {
        delete[] static_cast<u8 *>(mIcons[i].texData);
    }
    delete[] mIcons;

    mIconsInitialized = false;
}

void CRFLManager::fn_800C2C40(void) {
    fn_801D369C(eHeapGroup_SceneAsset);
    if (gFileManager->fn_801D42E0(1)) {
        gFileManager->fn_801D3F94(1, "content2/RFLRes01.arc");
    }
    fn_801D3644();
}

void CRFLManager::fn_800C2C98(void) {
    gFileManager->fn_801D41CC(1);
}

RFLErrcode CRFLManager::fn_800C2CA4(void) {
    if (!mIconsInitialized) {
        return RFLErrcode_Unknown;
    }

    mRFLWorkMem = new (eHeap_MEM2, -32) u8[RFLGetWorkSize(TRUE)];
    
    u32 resLen = gFileManager->getRFLResLen();
    void *resAddr = gFileManager->getRFLResAddr();

    mLastRFLErr = RFLInitRes(mRFLWorkMem, resAddr, resLen, TRUE);
    if (mLastRFLErr != RFLErrcode_Success) {
        return mLastRFLErr;
    }
    
    mIconCount = 0;
    for (s32 i = 0; i < 2; i++) {
        BOOL favorite = i == 0;
        for (u8 j = 0; j < MAX_ICON_COUNT; j++) {
            mLastRFLErr = fn_800C2E44(j, favorite);
            if ((mLastRFLErr != RFLErrcode_Success) || (mIconCount >= MAX_ICON_COUNT)) {
                break;
            }
        }

        if ((mLastRFLErr != RFLErrcode_Success) || (mIconCount >= MAX_ICON_COUNT)) {
            break;
        }
    }

    GXSetCopyClear((GXColor){ 0, 0, 0, 0 }, 0xFFFFFF);
    GXSetDither(TRUE);

    return mLastRFLErr;
}

void CRFLManager::fn_800C2E04(void) {
    if (mIconsInitialized) {
        RFLExit();
        delete[] static_cast<u8 *>(mRFLWorkMem);
    }
}

RFLErrcode CRFLManager::fn_800C2E44(u16 dataIndex, BOOL favorite) {
    BOOL available = RFLIsAvailableOfficialData(dataIndex);
    if (available) {
        RFLAdditionalInfo info;

        RFLErrcode err = RFLGetAdditionalInfo(&info, RFLDataSource_Official, NULL, dataIndex);
        if (err != RFLErrcode_Success) {
            return err;
        }

        if ((favorite && info.favorite == FALSE) || (!favorite && info.favorite == TRUE)) {
            return err;
        }

        mIcons[mIconCount].createID = info.createID;

        RFLIconSetting iconSetting;
        iconSetting.bgType = RFLIconBG_Direct;
        iconSetting.bgColor = (GXColor){ 240, 0, 0, 0 };
        iconSetting.drawXluOnly = FALSE;
        iconSetting.width = ICON_WIDTH;
        iconSetting.height = ICON_HEIGHT;

        err = RFLMakeIcon(
            mIcons[mIconCount].texData,
            RFLDataSource_Official,
            NULL,
            dataIndex,
            RFLExp_Normal,
            &iconSetting
        );
        if (err != RFLErrcode_Success) {
            return err;
        }

        GXInitTexObj(
            &mIcons[mIconCount].texObj, mIcons[mIconCount].texData,
            iconSetting.width, iconSetting.height, GX_TF_RGB5A3,
            GX_CLAMP, GX_CLAMP, FALSE
        );

        mIconCount++;
    }

    return RFLErrcode_Success;
}

RFLErrcode CRFLManager::fn_800C2FBC(void) {
    return mLastRFLErr;
}

s32 CRFLManager::fn_800C2FC4(void) {
    return mIconCount;
}

GXTexObj *CRFLManager::fn_800C2FCC(s32 index) {
    return &mIcons[index].texObj;
}

GXTexObj *CRFLManager::fn_800C2FE0(RFLCreateID createID) {
    s32 maxIcon = mIconCount;
    Icon *icons = NULL;
    for (s32 i = 0; i < maxIcon; i++) {
        icons = mIcons;
        if (memcmp(&icons[i].createID, &createID, sizeof(RFLCreateID)) == 0) {
            return &icons[i].texObj;
        }
    }
    
    return NULL;
}

RFLCreateID CRFLManager::fn_800C3060(s32 index) {
    return mIcons[index].createID;
}
