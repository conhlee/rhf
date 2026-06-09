#include "CellAnimManager.hpp"

#include <revolution/SC.h>

#include "GraphicManager.hpp"

#include "Mem.hpp"

CCellAnimManager::CCellAnimManager(void) {}

CCellAnimManager::~CCellAnimManager(void) {
    _08();
}

void CCellAnimManager::_10(s32 cellAnimCount) {
    // @bug The heap won't end up being able to fit exactly as much CCellAnim
    //      instances as cellAnimCount might suggest because of heap- header &
    //      block data.
    u32 heapSize = ROUND_UP(cellAnimCount * sizeof(CCellAnim), 32);

    mHeapStart = new (eHeap_MEM2, 32) u8[heapSize];
    mHeap = MEMCreateExpHeap(mHeapStart, heapSize);
}

void CCellAnimManager::_08(void) {
    CCellAnim *current = mCellAnimHead;
    while (current != NULL) {
        CCellAnim *next = current->getNext();
        endCellAnim(current);
        current = next;
    }

    for (u8 i = 0; i < BANK_COUNT; i++) {
        if (mBank[i].loaded) {
            endBank(i);
        }
    }

    MEMDestroyExpHeap(mHeap);
    delete[] mHeapStart;
}

void CCellAnimManager::_14(void) {
    for (s32 i = 0; i < BANK_COUNT; i++) {
        mBank[i].texObj = NULL;
        mBank[i].loaded = false;
        mBank[i].chrIsCI = false;
        mBank[i].usingTexObj = false;
    }

    mCellAnimHead = NULL;

    mCellAnimBase = NULL;
    mCellAnimBaseCallback = NULL;

    mCellAnimTempoUpdate = false;
    mCellAnimTempo = 120;
}

void CCellAnimManager::_18(void) {
    if ((mCellAnimBase != NULL) && (mCellAnimBaseCallback != NULL)) {
        mCellAnimBaseCallback();
    }

    CCellAnim *current = mCellAnimHead;
    while (current != NULL) {
        CCellAnim *next = static_cast<CCellAnim *>(current->getNext());

        bool end = current->update();
        if (end) {
            endCellAnim(current);
        }

        current = next;
    }
}

// not matching (argument passing via stack/high registers, maybe a struct?)
static void setupProj(Mtx44 mtx, f32 left, f32 top, f32 right, f32 bottom, f32 near, f32 far, f32 wWide, f32 wStd) {
    if (SCGetAspectRatio() == SC_ASPECT_STD) {
        C_MTXOrtho(mtx, top, bottom, left, right, near, far);
    }
    else {
        left *= wWide;
        right *= wWide;
        
        left /= wStd;
        right /= wStd;

        C_MTXOrtho(mtx, top, bottom, left, right, near, far);
    }
}

void CCellAnimManager::_1C(DrawSetupFn setupFn, DrawRestoreFn endFn) {
    if (setupFn) {
        setupFn();
    }
    else {
        drawSetupDefault();
    }

    Mtx44 projMtx;
    setupProj(projMtx, -304.0f, -228.0f, 304.0f, 228.0f, 0.0f, 10000.0f, 832.0f, 608.0f);
    GXSetProjection(projMtx, GX_ORTHOGRAPHIC);

    for (CCellAnim *cellAnim = mCellAnimHead; cellAnim != NULL; cellAnim = cellAnim->getNext()) {
        cellAnim->makeMtx(TRUE, NULL);
    }
    for (CCellAnim *cellAnim = mCellAnimHead; cellAnim != NULL; cellAnim = cellAnim->getNext()) {
        if (cellAnim->getEnabled()) {
            cellAnim->draw(FALSE);
        }
    }

    if (endFn) {
        endFn();
    }
}

void CCellAnimManager::_20(s32 startLayer, DrawSetupFn setupFn, DrawRestoreFn endFn) {
    if (setupFn) {
        setupFn();
    }
    else {
        drawSetupDefault();
    }

    Mtx44 projMtx;
    setupProj(projMtx, -304.0f, -228.0f, 304.0f, 228.0f, 0.0f, 10000.0f, 832.0f, 608.0f);
    GXSetProjection(projMtx, GX_ORTHOGRAPHIC);

    for (CCellAnim *cellAnim = mCellAnimHead; cellAnim != NULL; cellAnim = cellAnim->getNext()) {
        cellAnim->makeMtx(TRUE, NULL);
    }
    for (CCellAnim *cellAnim = mCellAnimHead; cellAnim != NULL; cellAnim = cellAnim->getNext()) {
        if (cellAnim->getLayer() < startLayer) {
            break;
        }
        if (cellAnim->getEnabled()) {
            cellAnim->draw(FALSE);
        }
    }

    if (endFn) {
        endFn();
    }
}

void CCellAnimManager::_24(s32 lastLayer, DrawSetupFn setupFn, DrawRestoreFn endFn) {
    if (setupFn) {
        setupFn();
    }
    else {
        drawSetupDefault();
    }

    Mtx44 projMtx;
    setupProj(projMtx, -304.0f, -228.0f, 304.0f, 228.0f, 0.0f, 10000.0f, 832.0f, 608.0f);
    GXSetProjection(projMtx, GX_ORTHOGRAPHIC);

    for (CCellAnim *cellAnim = mCellAnimHead; cellAnim != NULL; cellAnim = cellAnim->getNext()) {
        if (cellAnim->getLayer() >= lastLayer) {
            continue;
        }
        if (cellAnim->getEnabled()) {
            cellAnim->draw(FALSE);
        }
    }

    if (endFn) {
        endFn();
    }
}

void CCellAnimManager::drawSetupDefault(void) {
    GXSetTevDirect(GX_TEVSTAGE0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS,  GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS,  GX_POS_XY, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetNumTexGens(1);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX_IDENT);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZMode(FALSE, GX_ALWAYS, FALSE);
    GXSetCurrentMtx(GX_PNMTX0);
}

bool CCellAnimManager::getBankActive(u8 bankID) {
    return mBank[bankID].loaded;
}

// not matching (messy function)
void CCellAnimManager::registBank(void *brcadAddr, void *tplAddr, u8 bankID) {
    CellAnim_Bank *bank = &mBank[bankID];

    // TODO: these need to be shuffled
    bank->loaded = true;
    bank->usingTexObj = false;
    bank->chrIsCI = static_cast<CellAnimBin_Header *>(brcadAddr)->chrIsCI;
    bank->chrPalette = static_cast<TPLPalette *>(tplAddr);
    bank->chrIndex = static_cast<CellAnimBin_Header *>(brcadAddr)->chrIndex;
    bank->chrWidth = static_cast<CellAnimBin_Header *>(brcadAddr)->chrWidth;
    bank->chrHeight = static_cast<CellAnimBin_Header *>(brcadAddr)->chrHeight;

    bank->cellCount = static_cast<CellAnimBin_Header *>(brcadAddr)->cellCount;
    bank->cellArr = new CellAnim_Cell[bank->cellCount];

    u8 *curAddrCell = reinterpret_cast<u8 *>(brcadAddr) + sizeof(CellAnimBin_Header);
    for (s32 i = 0; i < bank->cellCount; i++) {
        u16 objCount = reinterpret_cast<CellAnimBin_Cell *>(curAddrCell)->objCount;
        curAddrCell += sizeof(CellAnimBin_Cell);

        bank->cellArr[i].objCount = objCount;
        bank->cellArr[i].objArr = new CellAnim_CellOBJ[objCount];

        for (s32 j = 0; j < objCount; j++) {
            CellAnim_CellOBJ *obj = &bank->cellArr[i].objArr[j];

            obj->chrX = reinterpret_cast<CellAnimBin_CellOBJ *>(curAddrCell)->chrX;
            obj->chrY = reinterpret_cast<CellAnimBin_CellOBJ *>(curAddrCell)->chrY;
            obj->width = reinterpret_cast<CellAnimBin_CellOBJ *>(curAddrCell)->width;
            obj->height = reinterpret_cast<CellAnimBin_CellOBJ *>(curAddrCell)->height;
            obj->chrIndexCI = reinterpret_cast<CellAnimBin_CellOBJ *>(curAddrCell)->chrIndexCIRel;
            obj->posX = reinterpret_cast<CellAnimBin_CellOBJ *>(curAddrCell)->posX;
            obj->posY = reinterpret_cast<CellAnimBin_CellOBJ *>(curAddrCell)->posY;
            obj->scaleX = reinterpret_cast<CellAnimBin_CellOBJ *>(curAddrCell)->scaleX;
            obj->scaleY = reinterpret_cast<CellAnimBin_CellOBJ *>(curAddrCell)->scaleY;
            obj->angle = reinterpret_cast<CellAnimBin_CellOBJ *>(curAddrCell)->angle;
            obj->flipX = reinterpret_cast<CellAnimBin_CellOBJ *>(curAddrCell)->flipX;
            obj->flipY = reinterpret_cast<CellAnimBin_CellOBJ *>(curAddrCell)->flipY;
            obj->opacity = reinterpret_cast<CellAnimBin_CellOBJ *>(curAddrCell)->opacity;

            obj->posX = reinterpret_cast<CellAnimBin_CellOBJ *>(curAddrCell)->posX - 512;
            obj->posY = reinterpret_cast<CellAnimBin_CellOBJ *>(curAddrCell)->posY - 512;

            obj->chrIndexCI =
                reinterpret_cast<CellAnimBin_CellOBJ *>(curAddrCell)->chrIndexCIRel +
                bank->chrIndex;

            curAddrCell += sizeof(CellAnimBin_CellOBJ);
        }
    }

    u16 animCount = reinterpret_cast<CellAnimBin_AnimHdr *>(curAddrCell)->animCount;
    u8 *curAddrAnim = curAddrCell + sizeof(CellAnimBin_AnimHdr);

    bank->animCount = animCount;
    bank->animArr = new CellAnim_Anim[bank->animCount];
    
    for (s32 i = 0; i < bank->animCount; i++) {
        u16 keyCount = reinterpret_cast<CellAnimBin_Anim *>(curAddrAnim)->keyCount;
        curAddrAnim += sizeof(CellAnimBin_Anim);

        bank->animArr[i].keyCount = keyCount;
        bank->animArr[i].keyArr = new CellAnim_AnimKey[keyCount];

        for (s32 j = 0; j < keyCount; j++) {
            CellAnim_AnimKey *key = &bank->animArr[i].keyArr[j];

            key->cellIndex = reinterpret_cast<CellAnimBin_AnimKey *>(curAddrAnim)->cellIndex;
            key->frameCount = reinterpret_cast<CellAnimBin_AnimKey *>(curAddrAnim)->frameCount;
            key->posX = reinterpret_cast<CellAnimBin_AnimKey *>(curAddrAnim)->posX;
            key->posY = reinterpret_cast<CellAnimBin_AnimKey *>(curAddrAnim)->posY;
            key->scaleX = reinterpret_cast<CellAnimBin_AnimKey *>(curAddrAnim)->scaleX;
            key->scaleY = reinterpret_cast<CellAnimBin_AnimKey *>(curAddrAnim)->scaleY;
            key->angle = reinterpret_cast<CellAnimBin_AnimKey *>(curAddrAnim)->angle;
            key->opacity = reinterpret_cast<CellAnimBin_AnimKey *>(curAddrAnim)->opacity;

            curAddrAnim += sizeof(CellAnimBin_AnimKey);
        }
    }
}

void CCellAnimManager::createBankTex(GXTexObj *texObj, s32 width, s32 height, f32 scaleX, f32 scaleY, u8 bankID) {
    CellAnim_Bank *bank = &mBank[bankID];

    bank->texObj = texObj;
    bank->loaded = true;
    bank->usingTexObj = true;
    bank->chrIsCI = false;
    bank->chrIndex = 0;
    bank->chrWidth = static_cast<u16>(width);
    bank->chrHeight = static_cast<u16>(height);

    bank->cellCount = 1;
    CellAnim_Cell *cellArr = new CellAnim_Cell[bank->cellCount];
    bank->cellArr = cellArr;

    cellArr[0].objCount = 1;
    CellAnim_CellOBJ *objArr = new CellAnim_CellOBJ[cellArr[0].objCount];
    cellArr[0].objArr = objArr;

    objArr[0].chrX = 0;
    objArr[0].chrY = 0;
    objArr[0].width = static_cast<u16>(width);
    objArr[0].height = static_cast<u16>(height);
    objArr[0].chrIndexCI = 0;
    objArr[0].posX = static_cast<s16>(-width / 2);
    objArr[0].posY = static_cast<s16>(-height / 2);
    objArr[0].scaleX = 1.0f;
    objArr[0].scaleY = 1.0f;
    objArr[0].angle = 0.0f;
    objArr[0].flipX = false;
    objArr[0].flipY = false;
    objArr[0].opacity = 0xFF;

    bank->animCount = 1;
    CellAnim_Anim *animArr = new CellAnim_Anim[bank->animCount];
    bank->animArr = animArr;

    animArr[0].keyCount = 1;
    CellAnim_AnimKey *keyArr = new CellAnim_AnimKey[animArr[0].keyCount];
    animArr[0].keyArr = keyArr;

    keyArr[0].cellIndex = 0;
    keyArr[0].frameCount = 1;
    keyArr[0].posX = 0;
    keyArr[0].posY = 0;
    keyArr[0].scaleX = scaleX;
    keyArr[0].scaleY = scaleY;
    keyArr[0].angle = 0.0f;
    keyArr[0].opacity = 0xFF;
}

void CCellAnimManager::createBankScreen(GXTexObj *texObj, u8 bankID) {
    u16 width, height;
    f32 scaleY, scaleX;

    width = gGraphicManager->getRenderMode()->fbWidth;
    height = gGraphicManager->getRenderMode()->efbHeight;

    scaleX = 608.0f / width;
    scaleY = 456.0f / height;

    if (SCGetAspectRatio() == SC_ASPECT_WIDE) {
        scaleX *= (832.0f / 608.0f);
    }

    createBankTex(texObj, width, height, scaleX, scaleY, bankID);
}

void CCellAnimManager::endBank(u8 bankID) {
    for (s32 i = 0; i < mBank[bankID].cellCount; i++) {
        delete[] mBank[bankID].cellArr[i].objArr;
    }
    delete[] mBank[bankID].cellArr;

    for (s32 i = 0; i < mBank[bankID].animCount; i++) {
        delete[] mBank[bankID].animArr[i].keyArr;
    }
    delete[] mBank[bankID].animArr;

    mBank[bankID].loaded = false;
    mBank[bankID].chrIsCI = false;
    mBank[bankID].usingTexObj = false;
}

u16 CCellAnimManager::getCurAnimFrames(CCellAnim *cellAnim) {
    CellAnim_Anim *anim = getCurrentAnim(cellAnim);
    return anim->getTotalFrameCount();
}

CellAnim_Anim *CCellAnimManager::getCurrentAnim(CCellAnim *cellAnim) {
    return &mBank[cellAnim->getBankID()].animArr[cellAnim->getAnimID()];
}

// not matching (regswap in lol)
CellAnim_AnimKey *CCellAnimManager::getCurrentAnimKey(CCellAnim *cellAnim) {
    return lol(cellAnim);
}

// not matching (regswap in lol)
CellAnim_Cell *CCellAnimManager::getCurrentCell(CCellAnim *cellAnim) {
    CellAnim_Bank *bank = &mBank[cellAnim->getBankID()];

    u16 spr = lol(cellAnim)->cellIndex;
    return &bank->cellArr[spr];
}

u16 CCellAnimManager::getBankChrWidth(u8 bankID) {
    return mBank[bankID].chrWidth;
}
u16 CCellAnimManager::getBankChrHeight(u8 bankID) {
    return mBank[bankID].chrHeight;
}

void CCellAnimManager::destroyCellAnim(CCellAnim *cellAnim) {
    cellAnim->~CCellAnim();
    MEMFreeToExpHeap(mHeap, static_cast<void *>(cellAnim));
}

void CCellAnimManager::loadChrForDraw(u8 bankID, CellAnim_CellOBJ *obj, BOOL linearFilter, s32 chrIndex) {   
    if (mBank[bankID].chrIsCI) {
        GXTexObj texObj;
        GXTlutObj tlutObj;

        s32 chrIndexCI = chrIndex;
        if (chrIndexCI == -1) {
            chrIndexCI = obj->chrIndexCI;
        }

        TPLDescriptor *desc = TPLGet(mBank[bankID].chrPalette, chrIndexCI);
        TPLClutHeader *clut = desc->clutHeader;

        GXInitTlutObj(&tlutObj, clut->data, clut->format, clut->numEntries);
        TPLGetGXTexObjFromPaletteCI(mBank[bankID].chrPalette, &texObj, &tlutObj, GX_TLUT0, chrIndexCI);

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

        if (mBank[bankID].usingTexObj) {
            texObj = *mBank[bankID].texObj;
        }
        else {
            TPLGetGXTexObjFromPalette(mBank[bankID].chrPalette, &texObj, mBank[bankID].chrIndex);
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

CCellAnim *CCellAnimManager::createCellAnim(u8 bankID, u16 animID) {
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

    cellAnim->init(bankID, animID);

    insertCellAnim(cellAnim);

    if (mCellAnimBase != NULL) {
        cellAnim->setBase(mCellAnimBase, 0, false);
    }

    cellAnim->setTempoUpdate(mCellAnimTempoUpdate);
    cellAnim->setTempo(mCellAnimTempo);

    return cellAnim;
}

void CCellAnimManager::endCellAnim(CCellAnim *cellAnim) {
    if (containsCellAnim(cellAnim)) {
        if (cellAnim->getBaseAnim() != NULL) {
            cellAnim->setBase(NULL, 0, false);
        }

        CCellAnim *current = cellAnim->getBaseLinkedHead();
        while (current != NULL) {
            CCellAnim *next = current->getBaseLinkedNext();
            current->clearBaseData();
            current = next;
        }

        if (cellAnim == mCellAnimHead) {
            mCellAnimHead = cellAnim->getNext();
        }

        cellAnim->removeCurrent();
    }
}

void CCellAnimManager::endCellAnimByBank(u8 bankID) {
    CCellAnim *current = mCellAnimHead;
    while (current != NULL) {
        CCellAnim *next = current->getNext();
        if (bankID == current->getBankID()) {
            endCellAnim(current);
        }
        current = next;
    }
}

void CCellAnimManager::layerReorder(CCellAnim *cellAnim) {
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

void CCellAnimManager::setBaseDefault(CCellAnim *baseCell) {
    mCellAnimBase = baseCell;
}

CCellAnim *CCellAnimManager::getBaseDefault(void) {
    return mCellAnimBase;
}

void CCellAnimManager::setBaseCallback(BaseUpdateCallbackFn callback) {
    mCellAnimBaseCallback = callback;
}

void CCellAnimManager::setTempoUpdate(bool update, u16 tempo) {
    mCellAnimTempoUpdate = update;
    mCellAnimTempo = tempo;
}
