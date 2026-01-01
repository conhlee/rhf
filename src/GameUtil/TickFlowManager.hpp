#ifndef GAMEUTIL_TICKFLOWMANAGER_HPP
#define GAMEUTIL_TICKFLOWMANAGER_HPP

#include <revolution/types.h>
#include "Singleton.hpp"

#include "TickFlow.hpp"

#include "TickFlowDecl.hpp"

class CTickFlowManager : public TSingleton<CTickFlowManager> {
public:
    virtual void _08(void);
    virtual ~CTickFlowManager(void);
    virtual void _10(s32);
    virtual void _14(CTickFlow::CreateFn, s32, const TickFlowCode *);
    virtual void _18(void);
    virtual void _1C(CTickFlow::CreateFn, u32);

    CTickFlowManager();

    template <typename T>
    void registerFlowInitial(const TickFlowCode *code) {
        _14(&T::create, sizeof(T), code);
    }
    template <typename T>
    void registerFlow(void) {
        _1C(&T::create, sizeof(T));
    }

    void fn_801E1E4C(void);
    void fn_801E1CC0(const TickFlowCode *code, f32 initRest = 0.0f);
    void fn_801E2490(s32 category);
    void fn_801E2540(CTickFlow *);
    f32 fn_801E2698(void);
    void fn_801E26A8(void);
    f32 fn_801E26B4(f32);
    f32 fn_801E2928(f32);
    void fn_801E2B9C(f32);
    void fn_801E2C04(f32);
    f32 fn_801E2CA8(void);
    void fn_801E3E2C(f32, u32);
    f32 fn_801E3EA8(s32);
    void fn_801E4048(bool);
    bool fn_801E4078(void);
    bool fn_801E40A8(void);
    bool fn_801E40D8(void);
    s32 fn_801E4124(void);
    void fn_801E4134(s32, u16);
    u16 fn_801E4144(s32);

    void fn_801E4170(bool);
    bool fn_801E4178(void);
    u8 fn_801E4180(void);

    s32 getUnk1C(void) {
        return unk1C;
    }
    u8 getUnk6D(void) {
        return unk6D;
    }

    void setUnkF8(const TickFlowCode *code) { mUnkF8 = code; }

    bool getUnkFC(void) const { return mUnkFC; }
    void setUnkFC(bool flag) { mUnkFC = flag; }

    void setUnkFD(u8 value) { mUnkFD = value; }

    void setUnk100(u32 value) { mUnk100 = value; }

private:
    u8 pad04[0x1c-0x04];
    s32 unk1C;
    u8 pad20[0x6C-0x20];
    u8 unk6C;
    u8 unk6D;
    u8 pad6E[0xF8-0x6E];
    const TickFlowCode *mUnkF8;
    bool mUnkFC;
    u8 mUnkFD;
    u32 mUnk100;
    u8 pad104[0x130 - 0x104];
};


extern CTickFlowManager *gTickFlowManager;

#endif