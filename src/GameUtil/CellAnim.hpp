#ifndef GAMEUTIL_CELLANIM_HPP
#define GAMEUTIL_CELLANIM_HPP

#include <revolution/types.h>

#include <revolution/MTX.h>

#include <nw4r/math/types.h>

#include "List.hpp"

class CCellAnim : public CList {
public:
    typedef void (*CellAnimPrepFn)(CCellAnim *anim, u16 animID, u16 nextAnimID);

public:
    CCellAnim *getNext(void) const {
        return static_cast<CCellAnim *>(this->CList::getNext());
    }
    CCellAnim *getPrev(void) const {
        return static_cast<CCellAnim *>(this->CList::getPrev());
    }
    
    virtual ~CCellAnim(void);
    virtual void finalInsert(void);
    virtual void finalDestroy(void);

    CCellAnim(void);

    u8 getID(void) const { return mID; }
    void setID(u8 id) { mID = id; }

    u16 getAnimID(void) const { return mAnimID; }

    f32 getFrame(void) const { return mFrame; }
    void setFrame(f32 frame) { mFrame = frame; }

    u16 getTotalFrames(void) const { return mTotalFrames; }

    s32 getLayer(void) const { return mLayer; }

    f32 getSpeed(void) const { return mSpeed; }
    void setSpeed(f32 speed) { mSpeed = speed; }

    bool getTempoUpdate(void) const { return mTempoUpdate; }
    void setTempoUpdate(bool tempoUpdate) { mTempoUpdate = tempoUpdate; }

    u16 getTempo(void) const { return mTempo; }
    void setTempo(u16 tempo) { mTempo = tempo; }

    f32 getPosX(void) const { return mPos.x; }
    f32 getPosY(void) const { return mPos.y; }

    void setPosX(f32 x) { mPos.x = x; }
    void setPosY(f32 y) { mPos.y = y; }

    nw4r::math::VEC2 &getPos(void) { return mPos; }
    void setPos(nw4r::math::VEC2 &pos) { mPos = pos; }
    void setPos(f32 x, f32 y) { mPos = nw4r::math::VEC2(x, y); }

    f32 getScaleX(void) const { return mSize.x; }
    f32 getScaleY(void) const { return mSize.y; }

    nw4r::math::VEC2 getScale(void) { return mSize; }
    void setScale(f32 x, f32 y) { mSize = nw4r::math::VEC2(x, y); }

    f32 getAngle(void) const { return mAngle; }
    void setAngle(f32 angle) { mAngle = angle; }

    void setBackColor(u8 r, u8 g, u8 b) {
        mBgColorR = r;
        mBgColorG = g;
        mBgColorB = b;
    }

    u8 getOpacity(void) const { return mOpacity; }
    void setOpacity(u8 opacity) { mOpacity = opacity; }

    bool getEnabled(void) const { return mEnabled; }
    void setEnabled(bool enabled) { mEnabled = enabled; }

    bool getPlaying(void) const { return mPlaying; }
    void setPlaying(bool playing) { mPlaying = playing; }

    bool getLooping(void) const { return mLooping; }
    void setLooping(bool looping) { mLooping = looping; }

    bool getPlaybackReverse(void) const { return mReversePlayback; }
    void setPlaybackReverse(bool reversed) { mReversePlayback = reversed; }

    CCellAnim *getBaseAnim(void) const { return mBaseAnim; }

    CCellAnim *getBaseLinkedHead(void) const { return mBaseLinkedHead; }
    CCellAnim *getBaseLinkedNext(void) const { return mBaseLinkedNext; }
    CCellAnim *getBaseLinkedPrev(void) const { return mBaseLinkedPrev; }

    void clearBase(void) {
        mBaseAnim = NULL;
        mBaseLinkedNext = NULL;
        mBaseLinkedPrev = NULL;
    }

    void init(u8 id, u16 animID);
    bool update(void);
    void makeMtx(BOOL defMtx, Mtx baseMtx);
    void draw(BOOL forceDraw);
    void setBase(CCellAnim *baseAnim, u16 partIndex, bool drawBase);
    void fn_801DCE9C(u16 animID);
    void fn_801DCEE0(u16 animID);
    void fn_801DCEE8(u16 animID, CellAnimPrepFn callback);
    void fn_801DCF18(void);
    u16 fn_801DCF2C(void);
    void fn_801DCF38(void);
    void fn_801DCF94(s32 layer);
    void fn_801DD0AC(u16 animID);
    void fn_801DD118(u16 animID);
    void fn_801DD184(u16 animID);
    void fn_801DD1F0(u16 animID);
    void fn_801DD24C(u16 animID, f32 frame);
    void fn_801DD2B4(u16 keyIndex);
    u16 fn_801DD43C(void);
    u16 fn_801DD4DC(void);
    bool fn_801DD5A0(void);

private:
    u16 handlePrepAnim(void) {
        u16 prevAnimID = mAnimID;
        u16 nextAnimID = mPrepAnimID[0];

        if (mPrepAnimCallback[0] != NULL) {
            (mPrepAnimCallback[0])(this, prevAnimID, nextAnimID);
        }
        mPrepAnimCount--;

        // Move queue back.
        for (s32 i = 0; i < mPrepAnimCount; i++) {
            mPrepAnimID[i + 0] = mPrepAnimID[i + 1];
            mPrepAnimCallback[i + 0] = mPrepAnimCallback[i + 1];
        }

        return nextAnimID;
    }

private:
    u8 mID;
    u16 mAnimID;

    u16 mPrepAnimID[8];
    u8 mPrepAnimCount;
    CellAnimPrepFn mPrepAnimCallback[8];

    f32 mFrame;
    u16 mTotalFrames;

    f32 mSpeed;
    u16 mTempo;

    Mtx mMtx;
    nw4r::math::VEC2 mPos;
    nw4r::math::VEC2 mSize;
    f32 mAngle;

    s32 mLayer;

    s32 mTextureIndex;

    u8 mFgColorR, mFgColorG, mFgColorB;
    u8 mBgColorR, mBgColorG, mBgColorB;
    u8 mOpacity; // alpha

    bool mEnabled;
    bool mLinearFiltering; // Linear texture filtering (as opposed to nearest).
    bool mPlaying;
    bool mLooping;
    bool mReversePlayback;
    bool mDestroyAtEnd; // This CellAnim will be destroyed when the animation ends.
    bool mDisableAtEnd; // This CellAnim will be disabled when the animation ends.
    bool mTempoUpdate;

    CCellAnim *mBaseAnim;
    CCellAnim *mBaseLinkedHead;
    CCellAnim *mBaseLinkedNext;
    CCellAnim *mBaseLinkedPrev;
    u16 mBasePartIndex;
    bool mBaseAnimDraw;
};

#endif