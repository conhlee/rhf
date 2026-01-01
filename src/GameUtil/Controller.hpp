#ifndef GAMEUTIL_CONTROLLERMANAGER_HPP
#define GAMEUTIL_CONTROLLERMANAGER_HPP

#include <revolution/types.h>
#include <revolution/OS.h>
#include <revolution/MEM.h>
#include <revolution/PAD.h>
#include <revolution/KPAD.h>

#include <nw4r/ut.h>

#include "Singleton.hpp"

#include "Layout.hpp"

class CController {
public:
    typedef CController *(*CreateFn)(s32 channel);

public:
    virtual ~CController(void) {}
    virtual void _0C(void);
    virtual void _10(void);
    virtual void _14(void);
    virtual u8 _18(void) {
        return mCoreStatus[0].dev_type;
    }
    virtual s8 _1C(void) {
        return mCoreStatus[0].wpad_err;
    }
    virtual s32 _20(void) {
        return mProbeErrcode;
    }
    virtual bool _24(void) {
        return (_20() == WPAD_ERR_OK) && (_18() != WPAD_DEV_NOT_FOUND);
    }
    virtual bool _28(void) {
        return (_20() == WPAD_ERR_OK) && (_18() == WPAD_DEV_CORE);
    }
    virtual bool _2C(void) {
        return (_20() == WPAD_ERR_OK) && (_18() == WPAD_DEV_FREESTYLE);
    }
    virtual bool _30(void) {
        return (_20() == WPAD_ERR_OK) && (_18() == WPAD_DEV_CLASSIC);
    }
    virtual bool _34(void) {
        return (_20() == WPAD_ERR_OK) && (_18() == WPAD_DEV_FUTURE);
    }
    virtual bool _38(void) {
        return (_20() == WPAD_ERR_OK) && (_18() == WPAD_DEV_NOT_SUPPORTED);
    }
    virtual bool _3C(void) {
        return (_20() == WPAD_ERR_OK) && (_18() == WPAD_DEV_UNKNOWN);
    }
    virtual void _40(const char *, bool);
    virtual void _44(void);
    virtual bool _48(void) { return mMotorSeqPlaying; }
    virtual void _4C(void);

    CController(s32 channel);
    void fn_801D4DDC(void);
    void fn_801D4E38(u32);
    void fn_801D4EA4(u32, u32);
    void fn_801D4F74(u32);
    void fn_801D4FD8(void);
    void fn_801D5170(bool);
    Vec2 fn_801D51E4(f32, f32);
    Vec2 fn_801D523C(CLayout *);
    bool fn_801D52D4(void);
    bool fn_801D5340(void);
    f32 fn_801D547C(void);
    void fn_801D5500(u32, u8);
    void fn_801D55D8(u32, u8);
    bool fn_801D5850(void);
    bool fn_801D58A0(void);
    s32 fn_801D58A8(void);

    void do801D4EA4(u32 arg0) {
        u32 temp = mCoreStatus[0].hold;
        if (!_24()) {
            temp = 0;
        }
        fn_801D4EA4(temp, arg0);
    }

    u32 getUnk133C(void) {
        return unk133C;
    }
    u32 getUnk1340(void) {
        return unk1340;
    }
    u32 getUnk1368(void) {
        return unk1368;
    }
    bool unk136CCheck(void) {
        return unk136C && !unk136D;
    }
    bool unk136CCheck1(void) {
        return !unk136C && unk136D;
    }

    u32 getUnk1338(void) {
        return unk1338;
    }

    Vec2 getCorePos(void) { return mCoreStatus[0].pos; }
    u32 getCoreTrig(void) { return mCoreStatus[0].trig; }

private:
    enum { MOTOR_SEQ_OFF = 0, MOTOR_SEQ_ON = 1, MOTOR_SEQ_END = 0xFF };
    enum { MOTOR_SEQ_MAXLEN = 32 };

    static void fn_801D5830(s32, s32); // WPADCallback

    s32 mMyChannel;
    s32 mKPADReadLength;
    s32 mProbeType;
    s32 mProbeErrcode;
    u32 unk14;
    KPADStatus mCoreStatus[KPAD_MAX_SAMPLES];
    u8 pad858[0xf18 - 0x858];
    KPADUnifiedWpadStatus mUnifiedStatus[KPAD_MAX_SAMPLES];
    u8 pad1298[0x1338 - 0x1298];
    u32 unk1338;
    u32 unk133C;
    u32 unk1340;
    u8 unk1344[0x10];
    u8 unk1354[0x10];
    u8 unk1364;
    u8 unk1365;
    u8 unk1366;
    u32 unk1368;
    bool unk136C;
    bool unk136D;
    u32 unk1370;
    u8 unk1374;
    bool mMotorSeqPlaying;
    u8 mMotorSeqPos;
    u8 mMotorSeq[MOTOR_SEQ_MAXLEN + 1];
    bool mInfoUpdated;
    s32 mInfoErrcode;
    WPADInfo mInfo;
};

class CNullController : public CController {
public:
    virtual ~CNullController(void) {}
    virtual u8 _18(void);
    virtual bool _24(void);
    virtual bool _28(void);
    virtual bool _2C(void);
    virtual bool _30(void);
    virtual bool _34(void);
    virtual bool _38(void);
    virtual bool _3C(void);

    CNullController(s32 channel) :
        CController(channel)
    {}
};

class CGCController {
public:
    virtual ~CGCController(void);
    virtual void _0C(void);
    virtual void _10(void) {}

    CGCController(s32 channel) {
        mMyChannel = channel;
        unk10 = 10;
        unk11 = 4;
    }

    void setUnk08(PADStatus *arg0) {
        mStatus = arg0;
    }
    PADStatus *getUnk08(void) {
        return mStatus;
    }
    void setUnk0C(PADStatus *arg0) {
        mStatusPrev = arg0;
    }
    PADStatus *getUnk0C(void) {
        return mStatusPrev;
    }
    u32 getUnk14(void) {
        return unk14;
    }
    bool unkInputCheck(u32 mask) {
        return
            (mStatus->err == PAD_ERR_NONE) && 
            (PADButtonDown(mStatusPrev->button, mStatus->button) & mask);
    }

private:
    s32 mMyChannel;
    PADStatus *mStatus;
    PADStatus *mStatusPrev;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u32 unk14;
};

class CControllerManager : public TSingleton<CControllerManager> {
public:
    virtual void _08(void);
    virtual ~CControllerManager(void);
    virtual void _10(CController::CreateFn createFn);
    virtual void _14(void);
    virtual void _18(void);
    virtual void _1C(void);

    CControllerManager(void);

    CController *fn_801D5FF0(s32 channel);
    CGCController *fn_801D6000(s32 channel);

    void *doAlloc(u32 size) {
        // TODO: doesn't match when using AutoInterruptLock..
        BOOL inter = OSDisableInterrupts();
        void *alloc = MEMAllocFromAllocator(&mAllocator, size);
        OSRestoreInterrupts(inter);
        return alloc;
    }

    BOOL doFree(void *block) {
        nw4r::ut::AutoInterruptLock lock;
        MEMFreeToAllocator(&mAllocator, block);
        return TRUE;
    }

private:
    CController *mController[4];
    CNullController *mNullController;

    MEMiHeapHead *mHeap;
    MEMAllocator mAllocator;
    u8 *mHeapStart;

    CGCController *mGCController[4];

    PADStatus mPadStatus[4];
    PADStatus mPadStatusPrev[4];

    static void *fn_801D5950(u32 size);
    static BOOL fn_801D59B0(void *block);
};

extern CControllerManager *gControllerManager;

#endif