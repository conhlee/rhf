#include "LayoutManager.hpp"

#include <revolution/GX.h>
#include <revolution/SC.h>

#include "FileManager.hpp"

#include "Mem.hpp"

const char lbl_80329968[] = "\0";

CLayoutManager::CLayoutManager(void) {}

CLayoutManager::~CLayoutManager(void) {
    _08();
}

void CLayoutManager::_10(void) {
    nw4r::lyt::LytInit();

    mHeapStart = new (eHeap_MEM2, 32) u8[0x100000];
    mHeap = MEMCreateExpHeap(mHeapStart, 0x100000);

    MEMInitAllocatorForExpHeap(&mAllocator, mHeap, 4);
    nw4r::lyt::Layout::SetAllocator(&mAllocator);

    mUseTempHeap = false;

    mResAccessor = new nw4r::lyt::MultiArcResourceAccessor;

    mDrawInfo = new nw4r::lyt::DrawInfo;
    if (SCGetAspectRatio() == SC_ASPECT_STD) {
        mDrawInfo->SetLocationAdjust(false);
    }
    else { // SC_ASPECT_WIDE
        mDrawInfo->SetLocationAdjust(true);
        mDrawInfo->SetLocationAdjustScale(nw4r::math::VEC2((608.0f / 832.0f), 1.0f));
    }

    mLayout = NULL;
    mFontInfo = NULL;
}

void CLayoutManager::_08(void) {
    fn_801D705C();
    MEMDestroyExpHeap(mHeap);

    delete mResAccessor;
    delete[] mHeapStart;
}

void CLayoutManager::_14(void) {
    mMaxLayoutCount = 0;
    mLayoutCount = 0;

    mScaleX = 1.0f;
    mScaleY = 1.0f;
}

void CLayoutManager::_20(s32 resCount) {
    mResLink = new nw4r::lyt::ArcResourceLink[resCount];

    mMaxResLinkCount = resCount;
    mResLinkCount = 0;
}

// TODO: ArcResourceLink::ArcResourceLink() is compiled with -O0

void CLayoutManager::_24(s32 arcIndex, const char *rootDir) {
    mResLink[mResLinkCount].Set(gFileManager->fn_801D42CC(arcIndex), rootDir);
    mResAccessor->Attach(&mResLink[mResLinkCount]);

    mResLinkCount++;
}

void CLayoutManager::_28(void) {
    mResAccessor->DetachAll();

    if (mResLink) {
        delete[] mResLink;
        mResLink = NULL;
    }

    mMaxResLinkCount = 0;
    mResLinkCount = 0;

    if (mUseTempHeap) {
        mUseTempHeap = false;

        MEMDestroyExpHeap(mTempHeap);
        delete[] mTempHeapStart;

        nw4r::lyt::Layout::SetAllocator(&mAllocator);
    }
}

void CLayoutManager::fn_801D6AEC(u8 layoutCount) {
    mLayoutCount = 0;
    mMaxLayoutCount = layoutCount;
    mLayout = new CLayout *[layoutCount];
}

void CLayoutManager::fn_801D6B2C(CLayout *layout) {
    mLayout[mLayoutCount] = layout;
    mLayout[mLayoutCount]->_0C();
    mLayout[mLayoutCount]->_10();
    mLayoutCount++;
}

void CLayoutManager::fn_801D6BB0(void) {
    for (s32 i = 0; i < mLayoutCount; i++) {
        delete mLayout[i];
    }

    if (mLayout != NULL) {
        delete[] mLayout;
        mLayout = NULL;
    }

    mMaxLayoutCount = 0;
    mLayoutCount = 0;
}

CLayout *CLayoutManager::fn_801D6C50(u8 index) {
    return mLayout[index];
}

void CLayoutManager::_18(void) {
    for (s32 i = 0; i < mLayoutCount; i++) {
        if (mLayout[i]->getUpdateEnable()) {
            mLayout[i]->_18(mDrawInfo);
        }
    }
}

void CLayoutManager::_1C(void) {
    fn_801D6D78();
    for (s32 i = 0; i < mLayoutCount; i++) {
        if (mLayout[i]->getDrawEnable()) {
            mLayout[i]->_1C(mDrawInfo);
        }
    }
}

void CLayoutManager::fn_801D6D78(void) {
    GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(FALSE, GX_NEVER, FALSE);
}

void CLayoutManager::fn_801D6DAC(u8 fontCount) {
    mFontCount = 0;
    mMaxFontCount = fontCount;

    mFontInfo = new CFontInfo *[fontCount];
    for (s32 i = 0; i < mMaxFontCount; i++) {
        mFontInfo[i] = new CFontInfo;
    }
}

bool CLayoutManager::fn_801D6E2C(void *resAddr, const char *resName) {
    if (strstr(resName, ".brfnt") != NULL) {
        mFontInfo[mFontCount]->fontType = eFontType_ResFont;
    }
    else if (strstr(resName, ".brfna") != NULL) {
        mFontInfo[mFontCount]->fontType = eFontType_ArchiveFont;
    }

    switch (mFontInfo[mFontCount]->fontType) {
    case eFontType_ResFont: {
        mFontInfo[mFontCount]->font = new nw4r::ut::ResFont;
        mFontInfo[mFontCount]->getResFont()->SetResource(resAddr);
    } break;
    case eFontType_ArchiveFont: {
        u32 bufferSize = nw4r::ut::ArchiveFont::GetRequireBufferSize(resAddr, lbl_80329968);
        mFontInfo[mFontCount]->buffer = new (eHeap_MEM2, 32) u8[bufferSize];
        mFontInfo[mFontCount]->font = new nw4r::ut::ArchiveFont;
        mFontInfo[mFontCount]->getArchiveFont()->Construct(
            mFontInfo[mFontCount]->buffer, bufferSize, resAddr, lbl_80329968
        );
    } break;
    }

    bool isArchiveFont = mFontInfo[mFontCount]->fontType == eFontType_ArchiveFont;

    mFontInfo[mFontCount]->refLink = new nw4r::lyt::FontRefLink;
    mFontInfo[mFontCount]->refLink->Set(resName, mFontInfo[mFontCount]->font);
    mResAccessor->RegistFont(mFontInfo[mFontCount]->refLink);

    mFontCount++;

    return isArchiveFont;
}

void CLayoutManager::fn_801D705C(void) {
    for (s32 i = 0; i < mFontCount; i++) {
        mResAccessor->UnregistFont(mFontInfo[i]->refLink);

        switch (mFontInfo[i]->fontType) {
        case eFontType_ResFont:
            mFontInfo[i]->getResFont()->RemoveResource();
            break;
        case eFontType_ArchiveFont:
            mFontInfo[i]->getArchiveFont()->Destroy();
            break;
        }

        delete mFontInfo[i]->refLink;
        delete mFontInfo[i]->font;
        delete[] mFontInfo[i]->buffer;

        delete mFontInfo[i];
    }

    if (mFontInfo != NULL) {
        delete[] mFontInfo;
        mFontInfo = NULL;
    }

    mMaxFontCount = 0;
    mFontCount = 0;
}

void CLayoutManager::fn_801D717C(f32 x, f32 y) {
    mScaleX = x;
    mScaleY = y;
}

f32 CLayoutManager::fn_801D7188(void) {
    return mScaleX;
}

f32 CLayoutManager::fn_801D7190(void) {
    return mScaleY;
}

void CLayoutManager::fn_801D7198(u32 size) {
    mUseTempHeap = true;

    mTempHeapStart = new (eHeap_MEM2, 32) u8[size];
    mTempHeap = MEMCreateExpHeap(mTempHeapStart, size);

    MEMInitAllocatorForExpHeap(&mTempAllocator, mTempHeap, 4);
    nw4r::lyt::Layout::SetAllocator(&mTempAllocator);
}
