#include "CellAnimManager.hpp"

#include "Brcad.hpp"

#include "GraphicManager.hpp"
#include <revolution/SC.h>

CCellAnimManager::CCellAnimManager(void) {}

CCellAnimManager::~CCellAnimManager(void) {
    _08();
}

void CCellAnimManager::_10(s32 cellAnimCount) {
    u32 heapSize = ROUND_UP(cellAnimCount * sizeof(CCellAnim), 32);
    u8 *heapStart = new (eHeap_MEM2, 32) u8[heapSize];

    mHeapStart = heapStart;
    mHeap = MEMCreateExpHeap(heapStart, heapSize);
}

void CCellAnimManager::_08(void) {
    CCellAnim *current = mCellAnimHead;
    while (current != NULL) {
        CCellAnim *next = current->getNext();
        fn_801DBFA0(current);
        current = next;
    }

    for (u8 i = 0; i < (s32)ARRAY_LENGTH(mCellAnimData); i++) {
        if (mCellAnimData[i].isLoaded) {
            fn_801DBA98(i);
        }
    }

    MEMDestroyExpHeap(mHeap);
    delete[] mHeapStart;
}

void CCellAnimManager::_14(void) {
    for (s32 i = 0; i < (s32)ARRAY_LENGTH(mCellAnimData); i++) {
        mCellAnimData[i].texObj = NULL;
        mCellAnimData[i].isLoaded = false;
        mCellAnimData[i].texIsPaletted = false;
        mCellAnimData[i].usingTexObj = false;
    }

    mCellAnimHead = NULL;
    mCellAnimBase = NULL;
    mCellAnimBaseCallback = NULL;
    mCellAnimTempoUpdate = false;
    mCellAnimTempo = 120;
}

void CCellAnimManager::_18(void) {
    if (mCellAnimBase != NULL && mCellAnimBaseCallback != NULL) {
        mCellAnimBaseCallback();
    }

    CCellAnim *current = mCellAnimHead;
    while (current != NULL) {
        CCellAnim *next = static_cast<CCellAnim *>(current->getNext());

        bool doEndCell = current->update();
        if (doEndCell) {
            fn_801DBFA0(current);
        }

        current = next;
    }
}

void CCellAnimManager::_1C(DrawSetupFn setupFn, DrawRestoreFn endFn) {
    // todo
}

void CCellAnimManager::fn_801DB28C(void) {
    GXSetTevDirect(GX_TEVSTAGE0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS,  GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS,  GX_POS_XY, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetNumTexGens(1);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZMode(FALSE, GX_ALWAYS, FALSE);
    GXSetCurrentMtx(0);
}

bool CCellAnimManager::fn_801DB558(u8 id) {
    return mCellAnimData[id].isLoaded;
}

void CCellAnimManager::fn_801DB568(void *data, void *tplAddr, u32 id) {
    // TODO
}

void CCellAnimManager::fn_801DB86C(GXTexObj *texObj, s32 texWidth, s32 texHeight, f32 scaleX, f32 scaleY, u8 id) {
    CellAnimData *cellAnimData = &mCellAnimData[id];

    cellAnimData->texObj = texObj;
    cellAnimData->isLoaded = true;
    cellAnimData->usingTexObj = true;
    cellAnimData->texIsPaletted = false;
    cellAnimData->texIndex = 0;
    cellAnimData->texWidth = static_cast<u16>(texWidth);
    cellAnimData->texHeight = static_cast<u16>(texHeight);

    cellAnimData->spriteCount = 1;
    CellAnimSprite *sprites = new CellAnimSprite[cellAnimData->spriteCount];
    cellAnimData->sprites = sprites;

    sprites[0].partCount = 1;
    CellAnimSpritePart *spriteParts = new CellAnimSpritePart[sprites[0].partCount];
    sprites[0].parts = spriteParts;

    spriteParts[0].regionX = 0;
    spriteParts[0].regionY = 0;
    spriteParts[0].regionW = static_cast<u16>(texWidth);
    spriteParts[0].regionH = static_cast<u16>(texHeight);
    spriteParts[0].texNumber = 0;
    spriteParts[0].posX = static_cast<s16>(-texWidth / 2);
    spriteParts[0].posY = static_cast<s16>(-texHeight / 2);
    spriteParts[0].scaleX = 1.0f;
    spriteParts[0].scaleY = 1.0f;
    spriteParts[0].angle = 0.0f;
    spriteParts[0].flipX = false;
    spriteParts[0].flipY = false;
    spriteParts[0].opacity = 0xFF;

    cellAnimData->animCount = 1;
    CellAnimAnimation *animations = new CellAnimAnimation[cellAnimData->animCount];
    cellAnimData->anims = animations;

    animations[0].keyCount = 1;
    CellAnimAnimationKey *keys = new CellAnimAnimationKey[animations[0].keyCount];
    animations[0].keys = keys;

    keys[0].spriteIndex = 0;
    keys[0].frameCount = 1;
    keys[0].posX = 0;
    keys[0].posY = 0;
    keys[0].scaleX = scaleX;
    keys[0].scaleY = scaleY;
    keys[0].angle = 0.0f;
    keys[0].opacity = 0xFF;
}

void CCellAnimManager::fn_801DB9C0(GXTexObj *texObj, u8 id) {
    u16 texWidth, texHeight;
    f32 scaleY, scaleX;

    texWidth = gGraphicManager->getRenderMode()->fbWidth;
    texHeight = gGraphicManager->getRenderMode()->efbHeight;

    scaleX = 608.0f / texWidth;
    scaleY = 456.0f / texHeight;

    if (SCGetAspectRatio() == SC_ASPECT_WIDE) {
        scaleX *= (832.0f / 608.0f);
    }

    fn_801DB86C(texObj, texWidth, texHeight, scaleX, scaleY, id);
}

void CCellAnimManager::fn_801DBA98(u8 id) {
    for (s32 i = 0; i < mCellAnimData[id].spriteCount; i++) {
        delete[] mCellAnimData[id].sprites[i].parts;
    }
    delete[] mCellAnimData[id].sprites;

    for (s32 i = 0; i < mCellAnimData[id].animCount; i++) {
        delete[] mCellAnimData[id].anims[i].keys;
    }
    delete[] mCellAnimData[id].anims;

    mCellAnimData[id].isLoaded = false;
    mCellAnimData[id].texIsPaletted = false;
    mCellAnimData[id].usingTexObj = false;
}

u16 CCellAnimManager::fn_801DBB58(CCellAnim *cellAnim) {
    return 0;
}

CellAnimAnimation *CCellAnimManager::fn_801DBC5C(CCellAnim *cellAnim) {
    return mCellAnimData[cellAnim->getID()].anims + cellAnim->getAnimID();
}

CellAnimAnimationKey *CCellAnimManager::fn_801DBC7C(CCellAnim *cellAnim) {
    return lol(cellAnim);
}

CellAnimSprite *CCellAnimManager::fn_801DBD38(CCellAnim *cellAnim) {
    CellAnimData *data = &mCellAnimData[cellAnim->getID()];

    return &data->sprites[lol(cellAnim)->spriteIndex];
}

u16 CCellAnimManager::fn_801DBE04(u8 id) {
    return mCellAnimData[id].texWidth;
}
u16 CCellAnimManager::fn_801DBE14(u8 id) {
    return mCellAnimData[id].texHeight;
}

void CCellAnimManager::fn_801DBE24(CCellAnim *cellAnim) {
    cellAnim->~CCellAnim();
    MEMFreeToExpHeap(mHeap, static_cast<void *>(cellAnim));
}

void CCellAnimManager::fn_801DB3D8(u8 id, CellAnimSpritePart *part, BOOL linearFilter, s32 texIndex) {   
    if (mCellAnimData[id].texIsPaletted) {
        GXTexObj texObj;
        GXTlutObj tlutObj;

        s32 myTexIndex = texIndex;
        if (myTexIndex == -1) {
            myTexIndex = part->texNumber;
        }

        TPLDescriptor *desc = TPLGet(mCellAnimData[id].texPalette, myTexIndex);
        TPLClutHeader *clut = desc->clutHeader;

        GXInitTlutObj(&tlutObj, clut->data, clut->format, clut->numEntries);
        TPLGetGXTexObjFromPaletteCI(mCellAnimData[id].texPalette, &texObj, &tlutObj, GX_TLUT0, myTexIndex);

        GXLoadTlut(&tlutObj, GX_TLUT0);

        if (linearFilter) {
            GXInitTexObjFilter(&texObj, GX_LINEAR, GX_LINEAR);
        }
        else {
            GXInitTexObjFilter(&texObj, GX_NEAR, GX_NEAR);
        }

        GXLoadTexObj(&texObj, GX_TEXMAP0);
    }
    else {
        GXTexObj texObj;

        if (mCellAnimData[id].usingTexObj) {
            texObj = *mCellAnimData[id].texObj;
        }
        else {
            TPLGetGXTexObjFromPalette(mCellAnimData[id].texPalette, &texObj, mCellAnimData[id].texIndex);
        }

        if (linearFilter) {
            GXInitTexObjFilter(&texObj, GX_LINEAR, GX_LINEAR);
        }
        else {
            GXInitTexObjFilter(&texObj, GX_NEAR, GX_NEAR);
        }

        GXLoadTexObj(&texObj, GX_TEXMAP0);
    }
}

CCellAnim *CCellAnimManager::fn_801DBE7C(u8 id, u16 animID) {
    void *alloc = MEMAllocFromExpHeap(mHeap, sizeof(CCellAnim));
    CCellAnim *cellAnim;
    if (alloc == NULL) {
        cellAnim = NULL;
    }
    else {
        cellAnim = new (alloc) CCellAnim;
    }

    if (cellAnim == NULL) {
        return NULL;
    }

    cellAnim->init(id, animID);

    insertCellAnim(cellAnim);

    if (mCellAnimBase != NULL) {
        cellAnim->setBase(mCellAnimBase, 0, false);
    }

    cellAnim->setTempoUpdate(mCellAnimTempoUpdate);
    cellAnim->setTempo(mCellAnimTempo);

    return cellAnim;
}

void CCellAnimManager::fn_801DBFA0(CCellAnim *cellAnim) {
    if (containsCellAnim(cellAnim)) {
        if (cellAnim->getBaseAnim() != NULL) {
            cellAnim->setBase(NULL, 0, false);
        }

        CCellAnim *current = cellAnim->getBaseLinkedHead();
        while (current != NULL) {
            CCellAnim *next = current->getBaseLinkedNext();
            current->clearBase();
            current = next;
        }

        if (cellAnim == mCellAnimHead) {
            mCellAnimHead = cellAnim->getNext();
        }

        cellAnim->removeCurrent();
    }
}

void CCellAnimManager::fn_801DC068(u32 id) {
    CCellAnim *current = mCellAnimHead;
    while (current != NULL) {
        CCellAnim *next = current->getNext();
        if (id == current->getID()) {
            fn_801DBFA0(current);
        }
        current = next;
    }
}

void CCellAnimManager::fn_801DC0D4(CCellAnim *cellAnim) {
    CCellAnim *prev = cellAnim->getPrev();
    if (prev != NULL) {
        prev->setNext(cellAnim->getNext());
    }
    else {
        mCellAnimHead = cellAnim->getNext();
    }

    CCellAnim *next = cellAnim->getNext();
    if (cellAnim->getNext() != NULL) {
        next->setPrev(cellAnim->getPrev());
    }

    insertCellAnim(cellAnim);
}

void CCellAnimManager::fn_801DC164(CCellAnim *baseCell) {
    mCellAnimBase = baseCell;
}

CCellAnim *CCellAnimManager::fn_801DC16C(void) {
    return mCellAnimBase;
}

void CCellAnimManager::fn_801DC174(BaseUpdateCallbackFn callback) {
    mCellAnimBaseCallback = callback;
}

void CCellAnimManager::fn_801DC17C(bool update, u16 updateRate) {
    mCellAnimTempoUpdate = update;
    mCellAnimTempo = updateRate;
}
