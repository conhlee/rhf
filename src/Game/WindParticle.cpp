#include "WindParticle.hpp"

#include "Random.hpp"

CWindParticle::CWindParticle(CExScene *scene, CCellAnim *anim) {
    mParentScene = scene;
    mAnim = anim;

    mAnim->setEnabled(false);

    mAnim->setLooping(true);
    mAnim->setSpeed(0.5 + sRandom.nextF32(0.5));

    mVelocityX = 0.0f;
    mSpeedY = 0.0f;

    mTargetVelocityX = 0.0f;
    mAccelX = 0.01f;

    mAreaWidth = 840.0f;
    mAreaHeight = 480.0f;

    mStopTimer = 0;
    mInvisibleTimer = -1;

    mNewAnimID = -1;

    mDisableWindResist = false;
}

CWindParticle::~CWindParticle(void) {}

void CWindParticle::fn_800C3168(void) {
    f32 newPosY = mAnim->getPosY() + mSpeedY;
    f32 newPosX = mAnim->getPosX() + mVelocityX;

    if (newPosY <= (mAreaHeight * 0.5f)) {
        if (newPosX >= -(mAreaWidth * 0.5f)) {
            if (newPosX > (mAreaWidth * 0.5f)) {
                newPosX -= mAreaWidth;
            }
        }
        else {
            newPosX += mAreaWidth;
        }
    }
    else {
        f32 randomPosX = sRandom.nextF32(mAreaWidth);

        newPosY -= mAreaHeight;
        newPosX = -((mAreaWidth * 0.5f) - randomPosX);

        if (!mDisableWindResist) {
            if (mTargetVelocityX <= 0.0f) {
                mVelocityX = -sRandom.nextF32(-mTargetVelocityX);
            }
            else {
                mVelocityX = sRandom.nextF32(mTargetVelocityX);
            }
        }

        if (mInvisibleTimer == 0) {
            mAnim->setEnabled(false);
        }
        else if (mNewAnimID != -1) {
            mAnim->fn_801DCE9C(mNewAnimID);
            mAnim->fn_801DCF18();
            mNewAnimID = -1;
        }
    }

    mAnim->setPos(newPosX, newPosY);

    if (mStopTimer < 1) {
        if (mVelocityX < mTargetVelocityX) {
            mVelocityX += mAccelX;
        }
        if (mTargetVelocityX < mVelocityX) {
            mVelocityX -= mAccelX;
        }
    }
    else {
        mStopTimer--;
    }

    if (mInvisibleTimer >= 0) {
        mInvisibleTimer--;
    }
}

void CWindParticle::fn_800C351C(void) {
    f32 x = sRandom.nextF32(mAreaWidth) - (mAreaWidth / 2);
    f32 y = sRandom.nextF32(mAreaHeight) - (mAreaHeight / 2);

    mAnim->setPos(x, y);
}

void CWindParticle::fn_800C366C(f32 targetVel, s32 stopTimer) {
    if (targetVel > 0.0f) {
        f32 rand = sRandom.nextF32(targetVel / 2);
        mTargetVelocityX =  (targetVel / 4) - (targetVel + rand);
    }
    else {
        f32 rand = sRandom.nextF32(-targetVel / 2);
        mTargetVelocityX =  (-targetVel / 4) - (targetVel - rand);
    }
    mStopTimer = stopTimer;
}

void CWindParticle::fn_800C3724(f32 accel) {
    mAccelX = accel;
}

void CWindParticle::fn_800C372C(s32 timer) {
    mInvisibleTimer = timer;
}

void CWindParticle::fn_800C3734(u16 animID, BOOL queue) {
    if (queue || !mAnim->getEnabled()) {
        mAnim->fn_801DCE9C(animID);
        mAnim->fn_801DCF18();
    }
    else {
        mNewAnimID = animID;
    }
}
