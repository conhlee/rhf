#ifndef GAMEUTIL_DEBUGPRINT_HPP
#define GAMEUTIL_DEBUGPRINT_HPP

#include <revolution/types.h>

#include <cstdarg>

#include <nw4r/ut.h>
#include <nw4r/math.h>
#include <nw4r/db.h>

#include "Singleton.hpp"

#include "Math.hpp"

#include "Vector.hpp"

class CDebugPrint : public TSingleton<CDebugPrint> {
public:
    virtual void _08(void);
    virtual ~CDebugPrint(void);
    virtual void _10(void);
    virtual void _14(void);
    virtual void _18(void);

    CDebugPrint(void);

    void fn_801EC674(s32 x, s32 y, s32 frames, const char *format, ...);

    Vec2i fn_801EC850(const char *format, std::va_list args);

private:
    void setFontColor(nw4r::ut::Color color) {
        if (nw4r::db::DbgPrint::GetInstance() != NULL) {
            nw4r::db::DbgPrint::GetInstance()->SetTextColor(color);
        }
    }
    void setFontSize(f32 size) {
        if (nw4r::db::DbgPrint::GetInstance() != NULL) {
            nw4r::db::DbgPrint::GetInstance()->SetFontSize(size);
        }
    }
    void doRegister(s32 x, s32 y, s32 frames, const char *format, std::va_list args) {
        if (nw4r::db::DbgPrint::GetInstance() != NULL) {
            nw4r::db::DbgPrint::GetInstance()->VRegisterf(x, y, frames, format, args);
        }
    }

private:
    struct SBG {
        SBG(void);
        ~SBG(void);

        u8 pad00[0x10];
        s32 unk10;
        u8 pad14[0x04];
    };

private:
    u32 mUnk04;
    CVector<SBG> mSBGList;
    s32 mUnk14;
    bool mEnable;
    f32 mUnk1C;
    f32 mUnk20;
    f32 mUnk24;
    f32 mUnk28;
    s32 mUnk2C;
    u32 mUnk30;
};

extern CDebugPrint *gDebugPrint;

#endif 