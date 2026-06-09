#ifndef GAME_FADERFLASH_HPP
#define GAME_FADERFLASH_HPP

#include <revolution/types.h>

#include "Fader.hpp"

class CFaderFlash : public CFader {
public:
    enum EState {
        eState_Idle,
        eState_1, // (unused)
        eState_Wait1,
        eState_Wait2,
    };

public:
    virtual void _08(void);
    virtual void _0C(void);
    virtual void _10(void);
    virtual ~CFaderFlash(void);
    virtual void _18(void);
    virtual void _1C(void);
    virtual void _20(void);
    virtual void _24(u8);

    CFaderFlash(void) {
        mState = eState_Idle;
    }

    EState getState(void) const { return mState; }
    
    void fn_80007DD8(void);

    void fn_800080B0(f32 frames);
    void fn_800080C0(f32 frames);

    bool fn_800080D0(void);

    void fn_801D859C(void); // destroy

    static CFaderFlash *fn_80007C28(void); // create

private:
    EState mState; // mState
    u8 mWaitFrames; // mWaitFrames
    f32 mAlpha; // mAlpha
    f32 mAlphaStep; // mAlphaStep
};

#endif