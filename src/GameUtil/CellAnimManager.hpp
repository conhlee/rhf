#ifndef GAMEUTIL_CELLANIMMANAGER_HPP
#define GAMEUTIL_CELLANIMMANAGER_HPP

#include <revolution/types.h>

#include <revolution/GX.h>
#include <revolution/TPL.h>
#include <revolution/MEM.h>
#include <revolution/MTX.h>
#include <revolution/SC.h>

#include "Singleton.hpp"

#include "CellAnim.hpp"
#include "CellAnimStr.hpp"

class CCellAnimManager : public TSingleton<CCellAnimManager> {
public:
    typedef void (*DrawSetupFn)();
    typedef void (*DrawRestoreFn)();
    typedef void (*BaseUpdateCallbackFn)();

    virtual void _08(void);
    virtual ~CCellAnimManager(void);
    virtual void _10(s32 cellAnimCount);
    virtual void _14(void);
    virtual void _18(void);
    virtual void _1C(DrawSetupFn setupFn, DrawRestoreFn restoreFn);
    virtual void _20(s32, DrawSetupFn setupFn, DrawRestoreFn restoreFn);
    virtual void _24(s32, DrawSetupFn setupFn, DrawRestoreFn restoreFn);

    CCellAnimManager(void);

    bool getBankActive(u8 bankID);
    void registBank(void *brcadAddr, void *tplAddr, u8 bankID);
    void createBankTex(GXTexObj *texObj, s32 width, s32 height, f32 scaleX, f32 scaleY, u8 bankID);
    void createBankScreen(GXTexObj *texObj, u8 bankID);
    void endBank(u8 bankID);
    u16 getCurAnimFrames(CCellAnim *cellAnim);
    CellAnim_Anim *getCurrentAnim(CCellAnim *cellAnim);
    CellAnim_AnimKey *getCurrentAnimKey(CCellAnim *cellAnim);
    CellAnim_Cell *getCurrentCell(CCellAnim *cellAnim);
    u16 getBankChrWidth(u8 bankID);
    u16 getBankChrHeight(u8 bankID);
    void destroyCellAnim(CCellAnim *cellAnim);
    void loadChrForDraw(u8 bankID, CellAnim_CellOBJ *obj, BOOL linearFilter, s32 chrIndex);
    CCellAnim *createCellAnim(u8 bankID, u16 animID);
    void endCellAnim(CCellAnim *cellAnim) DONT_INLINE;
    void endCellAnimByBank(u8 bankID);
    void layerReorder(CCellAnim *cellAnim);
    void setBaseDefault(CCellAnim *baseCell);
    CCellAnim *getBaseDefault(void);
    void setBaseCallback(BaseUpdateCallbackFn callback);
    void setTempoUpdate(bool update, u16 tempo);

private:
    void drawSetupDefault(void);

    void insertCellAnim(CCellAnim *cellAnim) {
        CCellAnim *current = mCellAnimHead;
        CCellAnim *prev = NULL;

        while (current != NULL) {
            if (cellAnim->getLayer() < current->getLayer()) {
                prev = current;
                current = current->getNext();
            }
            else {
                break;
            }
        }

        if (current == mCellAnimHead) {
            mCellAnimHead = cellAnim;
        }

        if (current != NULL) {
            cellAnim->insertBefore(current);
        }
        else {
            cellAnim->insertAfter(prev);
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

    CellAnim_AnimKey* lol(CCellAnim *cellAnim) {
        bool isReverse = cellAnim->getPlaybackReverse();

        CellAnim_Anim *anim = &mBank[cellAnim->getBankID()].animArr[cellAnim->getAnimID()];

        f32 currentFrame = 0.0f;
        f32 lastFrame = cellAnim->getFrame();

        for (s32 i = 0; i < anim->keyCount; i++) {
            s32 keyIdx = isReverse ? ((anim->keyCount - 1) - i) : i;

            currentFrame += anim->keyArr[keyIdx].frameCount;
            if (currentFrame > lastFrame)
                return &anim->keyArr[keyIdx];
        }

        return &anim->keyArr[isReverse ? 0 : (anim->keyCount - 1)];
    }

private:
    enum { BANK_COUNT = 0x100 };

    CellAnim_Bank mBank[BANK_COUNT];
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
