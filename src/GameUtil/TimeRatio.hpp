#ifndef GAMEUTIL_TIMERATIO_HPP
#define GAMEUTIL_TIMERATIO_HPP

#include <revolution/types.h>

#include "List.hpp"

class CTimeRatio : public CList {
public:
    CTimeRatio *getNext(void) const {
        return static_cast<CTimeRatio *>(this->CList::getNext());
    }
    CTimeRatio *getPrev(void) const {
        return static_cast<CTimeRatio *>(this->CList::getPrev());
    }

    virtual ~CTimeRatio(void);
    virtual void _14(void);

    CTimeRatio(void) {
        mTicking = false;

        mEasingType = 0;
        mEasingPower = 1;
        mEasingScale = 0.0f;

        mStopAtMaxT = true;

        mMaxT = 48.0f;
        mLinearT = 0.0f;
        fn_801E9EBC(); // Calculate initial mEasedT.

        mTicking = false;
    }

    void fn_801E9DE8(BOOL); // Update.

    void DONT_INLINE fn_801E9EBC(void); // Easing function; result is stored in mEasedT.

    void fn_801EA4D4(f32 addTime);

    void fn_801EA50C(f32 newTime);

    void fn_801EA514(f32 newTime, f32 newMaxTime);

    void fn_801EA550(f32 newTime, f32 newMaxTime, bool stopAtMaxTime);

    void setEasingType(s32 type) { mEasingType = type; }
    void setEasingPower(s32 power) { mEasingPower = power; }

    void setEasing(s32 type, s32 power) {
        mEasingType = type;
        mEasingPower = power;
    }

    bool getIsTicking(void) const { return mTicking; }
    void setIsTicking(bool ticking) { mTicking = ticking; }

    f32 getOutputStart(void) const { return mOutputStart; }
    f32 getOutputEnd(void) const { return mOutputEnd; }

    f32 getLinearTime(void) const { return mLinearT; }
    f32 getMaxTime(void) const { return mMaxT; }

    bool hasEnded(void) const {
        return !mTicking && (mLinearT == mMaxT);
    }

protected:
    // Easing function parameters.
    s32 mEasingType;  // Easing type. TODO: enum
    u8 mEasingPower;  // Controls curve sharpness.
    f32 mEasingScale; // ?

    // Time values (in ticks).
    f32 mLinearT; // Unmodified time input.
    f32 mMaxT;    // Maximum time value
    f32 mEasedT;  // Result of applying easing to mLinearT.

    bool mStopAtMaxT; // Stop ticking when mLinearT reaches mMaxT.
    bool mTicking; // Is time ticking?

    f32 mOutputStart; // Starting value of the output value.
    f32 mOutputEnd; // Ending value of the output value.
};

class CTRFloat : public CTimeRatio {
public:
    CTRFloat *getNext(void) const {
        return static_cast<CTRFloat *>(this->CTimeRatio::getNext());
    }
    CTRFloat *getPrev(void) const {
        return static_cast<CTRFloat *>(this->CTimeRatio::getPrev());
    }

    virtual ~CTRFloat(void);
    virtual void _14(void);

    CTRFloat(void);

    void fn_801EA698(f32, f32);

    f32 getCurrent(void) const { return mOutputStart; }

    f32 getTarget(void) const { return mTarget; }

private:
    f32 mTarget;
};

class CTRParabola : public CTimeRatio {
public:
    CTRParabola *getNext(void) const {
        return static_cast<CTRParabola *>(this->CTimeRatio::getNext());
    }
    CTRParabola *getPrev(void) const {
        return static_cast<CTRParabola *>(this->CTimeRatio::getPrev());
    }

    virtual ~CTRParabola(void);
    virtual void _14(void);

    CTRParabola(void);

    void fn_801EA8E0(f32 x0, f32 y0, f32 velY0, f32 x1, f32 y1, f32 velY1, f32 curvature);

    f32 getCurrentX(void) const { return mCurrentX; }
    f32 getCurrentY(void) const { return mCurrentY; }
    f32 getCurrentZ(void) const { return mCurrentZ; }

private:
    f32 mCurrentX;
    f32 mY0; // Parabola C
    f32 mY1;
    f32 mCurrentY;
    f32 mZ0;
    f32 mZ1;
    f32 mCurrentZ;
    f32 mCurvature;
    f32 mA; // Parabola A
    f32 mB; // Parabola B
};

#endif
