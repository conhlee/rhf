#ifndef GAMEUTIL_CELLANIMMANAGER_HPP
#define GAMEUTIL_CELLANIMMANAGER_HPP

#include <revolution/types.h>
#include "Singleton.hpp"
#include "CellAnim.hpp"
#include "Brcad.hpp"

#include "Mem.hpp"

#include <revolution/GX.h>
#include <revolution/TPL.h>

#include <revolution/MTX.h>

#include <revolution/SC.h>

class CCellAnimManager : public TSingleton<CCellAnimManager> {
public:
    typedef void (*DrawSetupFn)();
    typedef void (*DrawRestoreFn)();
    typedef void (*BaseUpdateCallbackFn)();

    virtual void _08(void);
    virtual ~CCellAnimManager(void);
    virtual void _10(s32);
    virtual void _14(void);
    virtual void _18(void);
    virtual void _1C(DrawSetupFn, DrawRestoreFn);
    virtual void _20(s32, DrawSetupFn, DrawRestoreFn);
    virtual void _24(s32, DrawSetupFn, DrawRestoreFn);

    CCellAnimManager(void);

    void fn_801DB28C(void);
    bool fn_801DB558(u8 id);
    void fn_801DB568(void *data, void *tplAddr, u32 id);
    void fn_801DB86C(GXTexObj *, s32, s32, f32, f32, u8);
    void fn_801DB9C0(GXTexObj *, u8);
    void fn_801DBA98(u8 id);
    u16 fn_801DBB58(CCellAnim *cellAnim);
    CellAnimAnimation *fn_801DBC5C(CCellAnim *cellAnim);
    CellAnimAnimationKey *fn_801DBC7C(CCellAnim *cellAnim);
    CellAnimSprite *fn_801DBD38(CCellAnim *cellAnim);
    u16 fn_801DBE04(u8 id);
    u16 fn_801DBE14(u8 id);
    void fn_801DBE24(CCellAnim *cellAnim);
    void fn_801DB3D8(u8, CellAnimSpritePart *, BOOL, s32);
    CCellAnim *fn_801DBE7C(u8, u16);
    void DONT_INLINE fn_801DBFA0(CCellAnim *cellAnim);
    void fn_801DC068(u32 id); // TODO: id is u8
    void fn_801DC0D4(CCellAnim *cellAnim);
    void fn_801DC164(CCellAnim *baseCell);
    CCellAnim* fn_801DC16C(void);
    void fn_801DC174(BaseUpdateCallbackFn callback);
    void fn_801DC17C(bool update, u16 updateRate);

private:
    void insertCellAnim(CCellAnim *cellAnim) {
        CCellAnim *insertCell = mCellAnimHead;
        CCellAnim *prevCell = NULL;

        while (insertCell != NULL) {
            if (cellAnim->getLayer() < insertCell->getLayer()) {
                prevCell = insertCell;
                insertCell = insertCell->getNext();
            }
            else {
                break;
            }
        }

        if (insertCell == mCellAnimHead) {
            mCellAnimHead = cellAnim;
        }

        if (insertCell != NULL) {
            cellAnim->insertBefore(insertCell);
        }
        else {
            cellAnim->insertAfter(prevCell);
        }
    }

    bool containsCellAnim(CCellAnim *cellAnim) {
        CCellAnim *current = mCellAnimHead;
        while (current != NULL) {
            if (current == cellAnim) {
                return true;
            }

            current = current->getNext();
        }

        return false;
    }

    void setupProjection(void) {
        // TODO: nonmatch
        Mtx44 projMtx;

        f32 nearField = 0.0f;
        f32 farField = 10000.0f;

        if (SCGetAspectRatio() == SC_ASPECT_STD) {
            C_MTXOrtho(projMtx, -228.0, 228.0, -304.0, 304.0, nearField, farField);
        }
        else {
            C_MTXOrtho(projMtx, -228.0, 228.0, -416.0, 416.0, nearField, farField);
        }

        GXSetProjection(projMtx, GX_ORTHOGRAPHIC);
    }

    CellAnimAnimationKey* lol(CCellAnim *cellAnim) {
        bool isReverse = cellAnim->getPlaybackReverse();

        CellAnimAnimation *anim = &mCellAnimData[cellAnim->getID()].anims[cellAnim->getAnimID()];

        f32 currentFrame = 0.0f;
        f32 lastFrame = cellAnim->getFrame();

        for (s32 i = 0; i < anim->keyCount; i++) {
            s32 keyIdx = isReverse ? ((anim->keyCount - 1) - i) : i;

            currentFrame += anim->keys[keyIdx].frameCount;
            if (currentFrame > lastFrame)
                return &anim->keys[keyIdx];
        }

        return &anim->keys[isReverse ? 0 : (anim->keyCount - 1)];
    }

private:
    struct CellAnimData {
        TPLPalette *texPalette;
        GXTexObj *texObj;

        bool isLoaded;

        bool texIsPaletted;
        bool usingTexObj; // use texObj when drawing instead of texPalette

        u16 texIndex;
        u16 texWidth, texHeight;

        u16 spriteCount, animCount;
        CellAnimSprite* sprites;
        CellAnimAnimation* anims;
    };

    CellAnimData mCellAnimData[256];
    CCellAnim *mCellAnimHead;

    CCellAnim *mCellAnimBase;
    BaseUpdateCallbackFn mCellAnimBaseCallback;

    u8 *mHeapStart;
    MEMiHeapHead *mHeap;

    bool mCellAnimTempoUpdate;
    u16 mCellAnimTempo;
};

extern CCellAnimManager *gCellAnimManager;

#endif
