#ifndef GAME_FADERFLASH_HPP
#define GAME_FADERFLASH_HPP

#include <revolution/types.h>
#include "Fader.hpp"

class CFaderFlash : public CFader {
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
        mUnk04 = 0;
    }

    s32 getUnk04(void) const { return mUnk04; }
    
    void fn_80007DD8(void);

    void fn_800080B0(f32);
    void fn_800080C0(f32);

    bool fn_800080D0(void);

    void fn_801D859C(void);

    static CFaderFlash *fn_80007C28(void);

private:
    s32 mUnk04;
    u8 mUnk08;
    f32 mUnk0C;
    f32 mUnk10;
};

#endif