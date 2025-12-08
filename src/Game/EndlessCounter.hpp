#ifndef GAME_ENDLESS_COUNTER_HPP
#define GAME_ENDLESS_COUNTER_HPP

#include <revolution/types.h>

#include "CellAnim.hpp"

class CEndlessCounter {
public:
    CEndlessCounter(u8, u8, u8, u16, bool, s32, f32);
    ~CEndlessCounter(void);

    void fn_8008397C(s32);
    void fn_80083A80(s32);
    void fn_80083AF4(s32);
    void fn_80083B20(s32, s32);
    u32 fn_80083C54(void);
    void fn_80083C5C(f32, f32);
    void fn_80083C84(f32, f32);
    void fn_80083CAC(bool);
    void fn_80083CB8(s32);
    void fn_80083CC0(f32);
    void fn_80083CCC(u16);
    void fn_80083DD0(void);

private:
    CCellAnim *mBaseAnim;
    CCellAnim **mDigitAnim;
    s32 mCountLimit;
    s32 mCurrentCount;
    u8 mDigitCount1;
    u8 mDigitCount2;
    u16 mFirstDigitAnimID;
    f32 mUnk14;
    bool mNoTrailingZero; // unsure
    s32 mUnk1C;
};

#endif
