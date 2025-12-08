#include "Metroman/MyChecker.hpp"

u32 Metroman::CMyChecker::_24(u32 button) {
    mScene->fn_800D8890(button);

    bool handCheck;
    if (mScene->mManIsLeft == 0) {
        handCheck = mScene->mHandDirection != 0;
    }
    else {
        handCheck = mScene->mHandDirection != 1;
    }
    if (handCheck) {
        mScene->fn_800D8940(mScene->mHandDirection);
    }
    else {
        mScene->fn_800D8C18(1);
        if (mScene->mUnk9E) {
            mScene->fn_800D8A78(0, 1);
            mScene->fn_800D8CA4(mScene->mLampSize);
            mScene->mLampSize++;
            if ((u16)mScene < mScene->mLampSize) {
                mScene->mUnk9E = false;
            }
            mScene->mUnk9D = true;
        }
    }
}
