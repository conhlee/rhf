#ifndef GAMEUTIL_LAYOUTMANAGER_HPP
#define GAMEUTIL_LAYOUTMANAGER_HPP

#include <revolution/types.h>
#include <revolution/MEM.h>

#include <nw4r/lyt.h>
#include <nw4r/ut.h>

#include "Singleton.hpp"

#include "Layout.hpp"

class CLayoutManager : public TSingleton<CLayoutManager> {
public:
    CLayoutManager(void);
    virtual void _08(void);
    virtual ~CLayoutManager(void);
    virtual void _10(void); // init
    virtual void _14(void);
    virtual void _18(void);
    virtual void _1C(void);
    virtual void _20(s32 resCount);
    virtual void _24(s32 arcIndex, const char *rootDir);
    virtual void _28(void);

    void fn_801D6AEC(u8 layoutCount);
    void fn_801D6B2C(CLayout *layout);
    void fn_801D6BB0(void);
    CLayout *fn_801D6C50(u8 index);
    void fn_801D6D78(void);
    void fn_801D6DAC(u8 fontCount);
    bool fn_801D6E2C(void *resAddr, const char *resName);
    void fn_801D705C(void);
    void fn_801D717C(f32 x, f32 y);
    f32 fn_801D7188(void);
    f32 fn_801D7190(void);
    void fn_801D7198(u32 size);

    template <typename T>
    void registerLayout(void) {
        fn_801D6B2C(new T);
    }

    template <typename T>
    T *getLayout(u8 index) {
        return static_cast<T *>(fn_801D6C50(index));
    }

    nw4r::lyt::MultiArcResourceAccessor *getResAccessor(void) const {
        return mResAccessor;
    }
    nw4r::lyt::DrawInfo *getDrawInfo(void) const {
        return mDrawInfo;
    }

private:
    enum EFontType {
        eFontType_ResFont,      // nw4r::ut::ResFont
        eFontType_ArchiveFont,  // nw4r::ut::ArchiveFont
    };

    class CFontInfo {
    public:
        nw4r::ut::ResFont *getResFont(void) {
            return static_cast<nw4r::ut::ResFont *>(font);
        }
        nw4r::ut::ArchiveFont *getArchiveFont(void) {
            return static_cast<nw4r::ut::ArchiveFont *>(font);
        }

    public:
        u8 fontType; // EFontType
        nw4r::lyt::FontRefLink *refLink;
        nw4r::ut::Font *font;
        u8 *buffer;
    };

private:
    u8 *mHeapStart;
    MEMiHeapHead *mHeap;
    MEMAllocator mAllocator;

    bool mUseTempHeap;
    u8 *mTempHeapStart;
    MEMiHeapHead *mTempHeap;
    MEMAllocator mTempAllocator;

    nw4r::lyt::MultiArcResourceAccessor *mResAccessor;

    nw4r::lyt::ArcResourceLink *mResLink; // nw4r::lyt::ArcResourceLink[mMaxResLinkCount]
    u8 mMaxResLinkCount;
    u8 mResLinkCount;

    CFontInfo **mFontInfo; // CFontInfo *[mMaxFontCount]
    u8 mMaxFontCount;
    u8 mFontCount;

    nw4r::lyt::DrawInfo *mDrawInfo;

    u8 mMaxLayoutCount;
    u8 mLayoutCount;
    CLayout **mLayout; // CLayout *[mMaxLayoutCount]

    f32 mScaleX;
    f32 mScaleY;
};

extern CLayoutManager *gLayoutManager;

#endif