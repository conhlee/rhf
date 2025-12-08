#ifndef GAME_WINDPARTICLE_HPP
#define GAME_WINDPARTICLE_HPP

#include <revolution/types.h>

#include "ExScene.hpp"

#include "CellAnim.hpp"

#include "Random.hpp"

class CWindParticle {
public:
    CWindParticle(CExScene *, CCellAnim *);
    ~CWindParticle(void);

    void fn_800C3168(void);

    void fn_800C351C(void);

    void fn_800C366C(f32, s32);

    void fn_800C3724(f32);
    void fn_800C372C(s32);

    void fn_800C3734(u16, BOOL);

    f32 getShitX(void) {
        return sRandom.nextF32(mAreaWidth) - (0.5f * mAreaWidth);
    }
    f32 getShitY(void) {
        return sRandom.nextF32(mAreaHeight) - (0.5f * mAreaHeight);
    }

private:
    CExScene *mParentScene;

    CCellAnim *mAnim;

    f32 mVelocityX;
    f32 mSpeedY;

    f32 mTargetVelocityX;
    f32 mAccelX;

    f32 mAreaWidth, mAreaHeight;

    s32 mStopTimer;
    s32 mInvisibleTimer;

    s32 mNewAnimID;

    bool mDisableWindResist;
};

#endif