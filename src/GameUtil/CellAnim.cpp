#include "CellAnim.hpp"

#include <revolution/GX.h>

#include "Mem.hpp"

#include "CellAnimManager.hpp"

#include "TickFlowManager.hpp"

CCellAnim::CCellAnim(void) {}

CCellAnim::~CCellAnim(void) {}

void CCellAnim::init(u8 id, u16 animID) {
    mID = id;
    mAnimID = animID;

    mPrepAnimCount = 0;

    mTotalFrames = gCellAnimManager->fn_801DBB58(this);

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
    mSize = nw4r::math::VEC2(1.0f, 1.0f);
    mAngle = 0.0f;

    mLayer = 0;

    mTextureIndex = -1;

    mFgColorR = 0;
    mFgColorG = 0;
    mFgColorB = 0;

    mBgColorR = 0xff;
    mBgColorG = 0xff;
    mBgColorB = 0xff;

    mOpacity = 0xff;

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
                mTotalFrames = gCellAnimManager->fn_801DBB58(this);
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
    if (defMtx && mBaseAnim != NULL) {
        return;
    }

    CellAnimAnimationKey *key = gCellAnimManager->fn_801DBC7C(this);
    CellAnimSprite *sprite = gCellAnimManager->fn_801DBD38(this);

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

    f64 myAngle = mAngle; // ???
    if (myAngle != 0.0) {
        MTXRotDeg(rotMtx, 'z', mAngle);
        MTXConcat(mMtx, rotMtx, mMtx);
    }
    if ((mSize.x != 1.0) || (mSize.y != 1.0)) {
        MTXScale(scaleMtx, mSize.x, mSize.y, 1.0f);
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
        u16 texWidth = gCellAnimManager->fn_801DBE04(mID); // unused
        u16 texHeight = gCellAnimManager->fn_801DBE14(mID); // unused

        for (s32 i = 0; i < sprite->partCount; i++) {
            bool hasBasePart = false;
            for (CCellAnim *anim = mBaseLinkedHead; anim != NULL; anim = anim->mBaseLinkedNext) {
                if (i == anim->mBasePartIndex) {
                    hasBasePart = true;
                    break;
                }
            }
            if (hasBasePart) {
                CellAnimSpritePart *part = &sprite->parts[i];

                f32 width = part->regionW * part->scaleX;
                f32 height = part->regionH * part->scaleY;

                f32 centerX = (width / 2.0f) + part->posX;
                f32 centerY = (height / 2.0f) + part->posY;

                if ((centerX != 0.0) || (centerY != 0.0)) {
                    MTXTrans(transMtx, centerX, centerY, 0.0f);
                    MTXConcat(mMtx, transMtx, tempMtx);
                }
                else {
                    MTXCopy(mMtx, tempMtx);
                }

                if (part->angle != 0.0) {
                    MTXRotDeg(rotMtx, 'z', part->angle);
                    MTXConcat(tempMtx, rotMtx, tempMtx);
                }
                if ((part->scaleX != 1.0) || (part->scaleY != 1.0)) {
                    MTXScale(scaleMtx, part->scaleX, part->scaleY, 1.0f);
                    MTXConcat(tempMtx, scaleMtx, tempMtx);
                }

                for (CCellAnim *anim = mBaseLinkedHead; anim != NULL; anim = anim->mBaseLinkedNext) {
                    if (i == anim->mBasePartIndex) {
                        anim->makeMtx(false, tempMtx);
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

        CellAnimAnimationKey *key = gCellAnimManager->fn_801DBC7C(this);
        CellAnimSprite *sprite = gCellAnimManager->fn_801DBD38(this);

        f32 texW = gCellAnimManager->fn_801DBE04(mID);
        f32 texH = gCellAnimManager->fn_801DBE14(mID);

        for (s32 i = 0; i < sprite->partCount; i++) {
            CellAnimSpritePart *part = &sprite->parts[i];
            gCellAnimManager->fn_801DB3D8(mID, part, mLinearFiltering, mTextureIndex);

            f32 sizeX = part->regionW * part->scaleX;
            f32 sizeY = part->regionH * part->scaleY;

            f32 centerX = (sizeX / 2.0f) + part->posX;
            f32 centerY = (sizeY / 2.0f) + part->posY;

            if ((centerX != 0.0) || (centerY != 0.0)) {
                MTXTrans(transMtx, centerX, centerY, 0.0f);
                MTXConcat(mMtx, transMtx, tempMtx);
            }
            else {
                MTXCopy(mMtx, tempMtx);
            }

            if (part->angle != 0.0) {
                MTXRotDeg(rotMtx, 'z', part->angle);
                MTXConcat(tempMtx, rotMtx, tempMtx);
            }

            if ((sizeX != 1.0) || (sizeY != 1.0)) {
                MTXScale(scaleMtx, sizeX, sizeY, 1.0);
                MTXConcat(tempMtx, scaleMtx, tempMtx);
            }

            GXLoadPosMtxImm(tempMtx, 0);

            u8 opacity = (key->opacity * part->opacity * mOpacity) / 255 / 255;
            GXSetTevColor(GX_TEVREG0, (GXColor){ mFgColorR, mFgColorG, mFgColorB, opacity });
            GXSetTevColor(GX_TEVREG1, (GXColor){ mBgColorR, mBgColorG, mBgColorB, opacity });

            f32 uvStartX = part->regionX / texW;
            f32 uvStartY = part->regionY / texH;
            f32 uvEndX = (part->regionX + part->regionW) / texW;
            f32 uvEndY = (part->regionY + part->regionH) / texH;

            if (part->flipX) {
                f32 temp = uvStartX;
                uvStartX = uvEndX;
                uvEndX = temp;
            }
            if (part->flipY) {
                f32 temp = uvStartY;
                uvStartY = uvEndY;
                uvEndY = temp;
            }

            GXBegin(GX_QUADS, GX_VTXFMT0, 4);

            GXPosition2f32(-0.5f, -0.5f);
            GXTexCoord2f32(uvStartX, uvStartY);

            GXPosition2f32(-0.5f, 0.5f);
            GXTexCoord2f32(uvStartX, uvEndY);

            GXPosition2f32(0.5f, 0.5f);
            GXTexCoord2f32(uvEndX, uvEndY);

            GXPosition2f32(0.5f, -0.5f);
            GXTexCoord2f32(uvEndX, uvStartY);

            GXEnd();

            for (CCellAnim *anim = mBaseLinkedHead; anim != NULL; anim = anim->mBaseLinkedNext) {
                if (i == anim->mBasePartIndex && anim->mEnabled && anim->mBaseAnimDraw) {
                    anim->draw(true);
                }
            }
        }
    }
}

void CCellAnim::fn_801DCE9C(u16 animID) {
    mAnimID = animID;
    mPrepAnimCount = 0;
    mTotalFrames = gCellAnimManager->fn_801DBB58(this);
}

void CCellAnim::fn_801DCEE0(u16 animID) {
    mAnimID = animID;
}

void CCellAnim::fn_801DCEE8(u16 animID, CellAnimPrepFn callback) {
    mPrepAnimID[mPrepAnimCount] = animID;
    mPrepAnimCallback[mPrepAnimCount] = callback;
    mPrepAnimCount++;
}

void CCellAnim::fn_801DCF18(void) {
    mPlaying = true;
    mFrame = -1.0f;
}

u16 CCellAnim::fn_801DCF2C(void) {
    return gCellAnimManager->fn_801DBB58(this);
}

void CCellAnim::fn_801DCF38(void) {
    mFrame = gCellAnimManager->fn_801DBB58(this) - 1;
}

void CCellAnim::fn_801DCF94(s32 layer) {
    mLayer = layer;
    gCellAnimManager->fn_801DC0D4(this);
}

void CCellAnim::setBase(CCellAnim *baseAnim, u16 partIndex, bool drawBase) {
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

        mBasePartIndex = partIndex;
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
    gCellAnimManager->fn_801DBE24(this);
}

void CCellAnim::fn_801DD0AC(u16 animID) {
    mAnimID = animID;
    mPrepAnimCount = 0;
    mTotalFrames = gCellAnimManager->fn_801DBB58(this);
    mLooping = false;
    mDestroyAtEnd = false;
    mDisableAtEnd = false;
    mPlaying = true;
    mFrame = -1.0f;
    mEnabled = true;
}

void CCellAnim::fn_801DD118(u16 animID) {
    mAnimID = animID;
    mPrepAnimCount = 0;
    mTotalFrames = gCellAnimManager->fn_801DBB58(this);
    mLooping = false;
    mDestroyAtEnd = false;
    mPlaying = true;
    mFrame = -1.0f;
    mEnabled = true;
    mDisableAtEnd = true;
}

void CCellAnim::fn_801DD184(u16 animID) {
    mAnimID = animID;
    mPrepAnimCount = 0;
    mTotalFrames = gCellAnimManager->fn_801DBB58(this);
    mLooping = false;
    mDisableAtEnd = false;
    mPlaying = true;
    mFrame = -1.0f;
    mEnabled = true;
    mDestroyAtEnd = true;
}

void CCellAnim::fn_801DD1F0(u16 animID) {
    mAnimID = animID;
    mPrepAnimCount = 0;
    mTotalFrames = gCellAnimManager->fn_801DBB58(this);
    mLooping = true;
    mPlaying = true;
    mFrame = -1.0f;
    mEnabled = true;
}

void CCellAnim::fn_801DD24C(u16 animID, f32 frame) {
    mAnimID = animID;
    mPrepAnimCount = 0;
    mTotalFrames = gCellAnimManager->fn_801DBB58(this);
    mFrame = frame;
    mPlaying = false;
    mEnabled = true;
}

void CCellAnim::fn_801DD2B4(u16 keyIndex) {
    CellAnimAnimation *anim = gCellAnimManager->fn_801DBC5C(this);
    mFrame = anim->getFrameOfKey(keyIndex);
}

u16 CCellAnim::fn_801DD43C(void) {
    CellAnimAnimation *anim = gCellAnimManager->fn_801DBC5C(this);
    return anim->findKeyAtFrame(getFrame());
}

u16 CCellAnim::fn_801DD4DC(void) {
    CellAnimAnimation *anim = gCellAnimManager->fn_801DBC5C(this);
    return anim->findKeyAtFrame(gCellAnimManager->fn_801DBB58(this));
}

bool CCellAnim::fn_801DD5A0(void) {
    return fn_801DD43C() == fn_801DD4DC();
}
