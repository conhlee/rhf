#include "CellAnim.hpp"

#include <revolution/GX.h>

#include "CellAnimManager.hpp"

#include "TickFlowManager.hpp"

CCellAnim::CCellAnim(void) {}

CCellAnim::~CCellAnim(void) {}

void CCellAnim::init(u8 bankID, u16 animID) {
    mBankID = bankID;
    mAnimID = animID;

    mPrepAnimCount = 0;

    mTotalFrames = gCellAnimManager->getCurAnimFrames(this);

    mEnabled = true;

    mLinearFiltering = true;

    mPlaying = false;

    mFrame = 0.0f;

    mSpeed = 1.0f;

    mLooping = false;
    mReversePlayback = false;

    mDestroyAtEnd = false;
    mDisableAtEnd = false;

    mTempoUpdate = false;
    mTempo = 120;

    mPos = nw4r::math::VEC2(0.0f, 0.0f);
    mScale = nw4r::math::VEC2(1.0f, 1.0f);
    mAngle = 0.0f;

    mLayer = 0;

    mTextureIndex = -1;

    mFgColorR = 0x00;
    mFgColorG = 0x00;
    mFgColorB = 0x00;

    mBgColorR = 0xFF;
    mBgColorG = 0xFF;
    mBgColorB = 0xFF;

    mOpacity = 0xFF;

    setNext(NULL);
    setPrev(NULL);

    mBaseAnim = NULL;
    mBaseLinkedHead = NULL;
    mBaseLinkedNext = NULL;
    mBaseLinkedPrev = NULL;

    mBaseAnimDraw = false;
}

bool CCellAnim::update(void) {
    if (mPlaying) {
        if (mFrame < 0.0f) {
            mFrame = 0.0f;
        }
        else {
            f32 speed;
            if (mTempoUpdate) {
                f32 curTempo = gTickFlowManager->fn_801E2CA8();
                f32 myTempo = mTempo;
            
                f32 delta = (curTempo - myTempo);
                if ((-2.0 < delta) && (delta < 2.0)) {
                    speed = mSpeed;
                }
                else {
                    speed = (mSpeed * curTempo) / myTempo;
                }
            }
            else {
                speed = mSpeed;
            }

            f32 frame;
            if (gTickFlowManager->fn_801E4178()) {
                frame = mFrame + speed;
            }
            else {
                frame = mFrame + (60.0f / 50.0f) * speed;
            }

            mFrame = frame;
        }

        u16 totalFrames = mTotalFrames;
        while (mFrame >= totalFrames) {
            if (mPrepAnimCount > 0) {
                u16 nextAnimID = handlePrepAnim();

                mAnimID = nextAnimID;
                mTotalFrames = gCellAnimManager->getCurAnimFrames(this);
                mPlaying = true;
                mFrame = -1.0f;
            }
            else {
                if (mLooping) {
                    mFrame -= totalFrames;
                    continue;
                }

                mPlaying = false;
                mFrame = totalFrames;

                if (mDestroyAtEnd) {
                    return true;
                }
                if (mDisableAtEnd) {
                    mEnabled = false;
                }

                break;
            }
        }
    }
    return false;
}

void CCellAnim::makeMtx(BOOL defMtx, Mtx baseMtx) {
    if (defMtx && (mBaseAnim != NULL)) {
        return;
    }

    CellAnim_AnimKey *key = gCellAnimManager->getCurrentAnimKey(this);
    CellAnim_Cell *cell = gCellAnimManager->getCurrentCell(this);

    Mtx transMtx;
    Mtx rotMtx;
    Mtx scaleMtx;
    Mtx tempMtx;

    if (defMtx) {
        MTXIdentity(mMtx);
    }
    else {
        MTXCopy(baseMtx, mMtx);
    }

    if ((mPos.x != 0.0) || (mPos.y != 0.0)) {
        MTXTrans(transMtx, mPos.x, mPos.y, 0.0f);
        MTXConcat(mMtx, transMtx, mMtx);
    }
    if (getAngle() != 0.0) {
        MTXRotDeg(rotMtx, 'z', getAngle());
        MTXConcat(mMtx, rotMtx, mMtx);
    }
    if ((mScale.x != 1.0) || (mScale.y != 1.0)) {
        MTXScale(scaleMtx, mScale.x, mScale.y, 1.0f);
        MTXConcat(mMtx, scaleMtx, mMtx);
    }

    if ((key->posX != 0.0) || (key->posY != 0.0)) {
        MTXTrans(transMtx, key->posX, key->posY, 0.0f);
        MTXConcat(mMtx, transMtx, mMtx);
    }
    if (key->angle != 0.0) {
        MTXRotDeg(rotMtx, 'z', key->angle);
        MTXConcat(mMtx, rotMtx, mMtx);
    }
    if ((key->scaleX != 1.0) || (key->scaleY != 1.0)) {
        MTXScale(scaleMtx, key->scaleX, key->scaleY, 1.0f);
        MTXConcat(mMtx, scaleMtx, mMtx);
    }

    if (mBaseLinkedHead != NULL) {
        u16 chrWidth = gCellAnimManager->getBankChrWidth(mBankID);
#pragma unused(chrWidth)
        u16 chrHeight = gCellAnimManager->getBankChrHeight(mBankID);
#pragma unused(chrHeight)

        for (s32 i = 0; i < cell->objCount; i++) {
            bool hasBaseOBJ = false;
            for (CCellAnim *cellAnim = mBaseLinkedHead; cellAnim != NULL; cellAnim = cellAnim->mBaseLinkedNext) {
                if (i == cellAnim->mBaseOBJIndex) {
                    hasBaseOBJ = true;
                    break;
                }
            }
            if (hasBaseOBJ) {
                CellAnim_CellOBJ *obj = &cell->objArr[i];

                f32 width = obj->width * obj->scaleX;
                f32 height = obj->height * obj->scaleY;

                f32 centerX = (width / 2.0f) + obj->posX;
                f32 centerY = (height / 2.0f) + obj->posY;

                if ((centerX != 0.0) || (centerY != 0.0)) {
                    MTXTrans(transMtx, centerX, centerY, 0.0f);
                    MTXConcat(mMtx, transMtx, tempMtx);
                }
                else {
                    MTXCopy(mMtx, tempMtx);
                }

                if (obj->angle != 0.0) {
                    MTXRotDeg(rotMtx, 'z', obj->angle);
                    MTXConcat(tempMtx, rotMtx, tempMtx);
                }

                if ((obj->scaleX != 1.0) || (obj->scaleY != 1.0)) {
                    MTXScale(scaleMtx, obj->scaleX, obj->scaleY, 1.0f);
                    MTXConcat(tempMtx, scaleMtx, tempMtx);
                }

                for (CCellAnim *cellAnim = mBaseLinkedHead; cellAnim != NULL; cellAnim = cellAnim->mBaseLinkedNext) {
                    if (i == cellAnim->mBaseOBJIndex) {
                        cellAnim->makeMtx(false, tempMtx);
                    }
                }
            }
        }
    }
}

void CCellAnim::draw(BOOL forceDraw) {
    if (mBaseAnim == NULL || mBaseAnimDraw == false || forceDraw) {
        Mtx transMtx;
        Mtx rotMtx;
        Mtx scaleMtx;
        Mtx tempMtx;

        CellAnim_AnimKey *key = gCellAnimManager->getCurrentAnimKey(this);
        CellAnim_Cell *cell = gCellAnimManager->getCurrentCell(this);

        f32 chrWidth = gCellAnimManager->getBankChrWidth(mBankID);
        f32 chrHeight = gCellAnimManager->getBankChrHeight(mBankID);

        for (s32 i = 0; i < cell->objCount; i++) {
            CellAnim_CellOBJ *obj = &cell->objArr[i];
            gCellAnimManager->loadChrForDraw(mBankID, obj, mLinearFiltering, mTextureIndex);

            f32 width = obj->width * obj->scaleX;
            f32 height = obj->height * obj->scaleY;

            f32 centerX = (width / 2.0f) + obj->posX;
            f32 centerY = (height / 2.0f) + obj->posY;

            if ((centerX != 0.0) || (centerY != 0.0)) {
                MTXTrans(transMtx, centerX, centerY, 0.0f);
                MTXConcat(mMtx, transMtx, tempMtx);
            }
            else {
                MTXCopy(mMtx, tempMtx);
            }

            if (obj->angle != 0.0) {
                MTXRotDeg(rotMtx, 'z', obj->angle);
                MTXConcat(tempMtx, rotMtx, tempMtx);
            }

            if ((width != 1.0) || (height != 1.0)) {
                MTXScale(scaleMtx, width, height, 1.0);
                MTXConcat(tempMtx, scaleMtx, tempMtx);
            }

            GXLoadPosMtxImm(tempMtx, GX_PNMTX0);

            u8 opacity = (key->opacity * obj->opacity * mOpacity) / 255 / 255;
            GXSetTevColor(GX_TEVREG0, (GXColor){ mFgColorR, mFgColorG, mFgColorB, opacity });
            GXSetTevColor(GX_TEVREG1, (GXColor){ mBgColorR, mBgColorG, mBgColorB, opacity });

            f32 chrNormX0 = obj->chrX / chrWidth;
            f32 chrNormY0 = obj->chrY / chrHeight;
            f32 chrNormX1 = (obj->chrX + obj->width) / chrWidth;
            f32 chrNormY1 = (obj->chrY + obj->height) / chrHeight;

            if (obj->flipX) {
                f32 temp = chrNormX0;
                chrNormX0 = chrNormX1;
                chrNormX1 = temp;
            }
            if (obj->flipY) {
                f32 temp = chrNormY0;
                chrNormY0 = chrNormY1;
                chrNormY1 = temp;
            }

            GXBegin(GX_QUADS, GX_VTXFMT0, 4);

            GXPosition2f32(-0.5f, -0.5f);
            GXTexCoord2f32(chrNormX0, chrNormY0);

            GXPosition2f32(-0.5f, 0.5f);
            GXTexCoord2f32(chrNormX0, chrNormY1);

            GXPosition2f32(0.5f, 0.5f);
            GXTexCoord2f32(chrNormX1, chrNormY1);

            GXPosition2f32(0.5f, -0.5f);
            GXTexCoord2f32(chrNormX1, chrNormY0);

            GXEnd();

            for (CCellAnim *cellAnim = mBaseLinkedHead; cellAnim != NULL; cellAnim = cellAnim->mBaseLinkedNext) {
                if ((i == cellAnim->mBaseOBJIndex) && cellAnim->mEnabled && cellAnim->mBaseAnimDraw) {
                    cellAnim->draw(true);
                }
            }
        }
    }
}

void CCellAnim::switchAnim(u16 animID) {
    mAnimID = animID;
    mPrepAnimCount = 0;
    mTotalFrames = gCellAnimManager->getCurAnimFrames(this);
}

void CCellAnim::overrideAnim(u16 animID) {
    mAnimID = animID;
}

void CCellAnim::prepareAnim(u16 animID, CellAnimPrepFn callback) {
    mPrepAnimID[mPrepAnimCount] = animID;
    mPrepAnimCallback[mPrepAnimCount] = callback;
    mPrepAnimCount++;
}

void CCellAnim::playFromBegin(void) {
    mPlaying = true;
    mFrame = -1.0f;
}

u16 CCellAnim::calcFrameCount(void) {
    return gCellAnimManager->getCurAnimFrames(this);
}

void CCellAnim::goToAnimEnd(void) {
    mFrame = gCellAnimManager->getCurAnimFrames(this) - 1;
}

void CCellAnim::setLayer(s32 layer) {
    mLayer = layer;
    gCellAnimManager->layerReorder(this);
}

void CCellAnim::setBase(CCellAnim *baseAnim, u16 objIndex, bool drawBase) {
    if (baseAnim != NULL) {
        mBaseAnimDraw = drawBase;

        if (mBaseAnim != NULL) {
            setBase(NULL, 0, false);
        }

        if (baseAnim->mBaseLinkedHead != NULL) {
            mBaseLinkedNext = baseAnim->mBaseLinkedHead;
            mBaseLinkedPrev = NULL;
            baseAnim->mBaseLinkedHead->mBaseLinkedPrev = this;
        }
        else {
            mBaseLinkedNext = NULL;
            mBaseLinkedPrev = NULL;
        }

        mBaseAnim = baseAnim;
        baseAnim->mBaseLinkedHead = this;

        mBaseOBJIndex = objIndex;
    }
    else if (mBaseAnim != NULL) {
        if (mBaseLinkedPrev != NULL) {
            mBaseLinkedPrev->mBaseLinkedNext = mBaseLinkedNext;
        }
        else {
            mBaseAnim->mBaseLinkedHead = mBaseLinkedNext;
        }

        if (mBaseLinkedNext != NULL) {
            mBaseLinkedNext->mBaseLinkedPrev = mBaseLinkedPrev;
        }

        mBaseAnim = NULL;
        mBaseLinkedNext = NULL;
        mBaseLinkedPrev = NULL;
    }
}

void CCellAnim::finalInsert(void) {}

void CCellAnim::finalDestroy(void) {
    gCellAnimManager->destroyCellAnim(this);
}

void CCellAnim::setAnim(u16 animID) {
    mAnimID = animID;
    mPrepAnimCount = 0;
    mTotalFrames = gCellAnimManager->getCurAnimFrames(this);
    mLooping = false;
    mDestroyAtEnd = false;
    mDisableAtEnd = false;
    mPlaying = true;
    mFrame = -1.0f;
    mEnabled = true;
}

void CCellAnim::setAnimDisable(u16 animID) {
    mAnimID = animID;
    mPrepAnimCount = 0;
    mTotalFrames = gCellAnimManager->getCurAnimFrames(this);
    mLooping = false;
    mDestroyAtEnd = false;
    mPlaying = true;
    mFrame = -1.0f;
    mEnabled = true;
    mDisableAtEnd = true;
}

void CCellAnim::setAnimDestroy(u16 animID) {
    mAnimID = animID;
    mPrepAnimCount = 0;
    mTotalFrames = gCellAnimManager->getCurAnimFrames(this);
    mLooping = false;
    mDisableAtEnd = false;
    mPlaying = true;
    mFrame = -1.0f;
    mEnabled = true;
    mDestroyAtEnd = true;
}

void CCellAnim::setAnimLoop(u16 animID) {
    mAnimID = animID;
    mPrepAnimCount = 0;
    mTotalFrames = gCellAnimManager->getCurAnimFrames(this);
    mLooping = true;
    mPlaying = true;
    mFrame = -1.0f;
    mEnabled = true;
}

void CCellAnim::setAnimFreezeFrame(u16 animID, f32 frame) {
    mAnimID = animID;
    mPrepAnimCount = 0;
    mTotalFrames = gCellAnimManager->getCurAnimFrames(this);
    mFrame = frame;
    mPlaying = false;
    mEnabled = true;
}

void CCellAnim::setFrameAtKey(u16 keyIndex) {
    CellAnim_Anim *anim = gCellAnimManager->getCurrentAnim(this);
    mFrame = anim->getFrameOfKey(keyIndex);
}

u16 CCellAnim::getKeyIndex(void) {
    CellAnim_Anim *anim = gCellAnimManager->getCurrentAnim(this);
    return anim->findKeyAtFrame(getFrame());
}

u16 CCellAnim::getFinalKeyIndex(void) {
    CellAnim_Anim *anim = gCellAnimManager->getCurrentAnim(this);
    // Unnecessary logic .. returning (anim->keyCount - 1) does the trick
    return anim->findKeyAtFrame(gCellAnimManager->getCurAnimFrames(this));
}

bool CCellAnim::getAtFinalKey(void) {
    return getKeyIndex() == getFinalKeyIndex();
}
