#ifndef GAMEUTIL_TICKFLOWMANAGER_HPP
#define GAMEUTIL_TICKFLOWMANAGER_HPP

#include <revolution/types.h>

#include <revolution/MEM.h>
#include <revolution/OS.h>

#include "Singleton.hpp"

#include "TickFlow.hpp"
#include "TickFlowDecl.hpp"

#include "Sound.hpp"

#include "code_801ED6A8.hpp"

enum EBGMType {
    eBGMType_None,
    eBGMType_Seq,
    eBGMType_Strm,
    eBGMType_Wave
};

class CTickFlowManager : public TSingleton<CTickFlowManager> {
public:
    virtual void _08(void);
    virtual ~CTickFlowManager(void);
    virtual void _10(u32 heapSize);
    virtual void _14(CTickFlow::CreateFn createFn, u32 size, const TickFlowCode *tickFlowCode);
    virtual void _18(void);
    virtual void _1C(CTickFlow::CreateFn createFn, u32 size);

    CTickFlowManager();

    template <typename T>
    void registerFlowInitial(const TickFlowCode *code) {
        _14(&T::create, sizeof(T), code);
    }
    template <typename T>
    void registerFlow(void) {
        _1C(&T::create, sizeof(T));
    }

    void fn_801E0578(void);
    void fn_801E0A0C(void);

    CTickFlow *fn_801E1CC0(const TickFlowCode *code, f32 initRest = 0.0f);
    bool fn_801E1D9C(CTickFlow *tickFlow);
    bool fn_801E1DC8(CTickFlow *tickFlow);
    void fn_801E1E4C(void);
    void fn_801E2490(s32 category);
    void fn_801E2540(CTickFlow *tickFlow);
    f32 fn_801E2698(void);
    f32 fn_801E26A0(void);
    void fn_801E26A8(void);
    f32 fn_801E26B4(f32);
    f32 fn_801E2928(f32);
    void fn_801E2B9C(f32 tempo);
    void fn_801E2C04(f32 speed);
    f32 fn_801E2CA8(void);

    void fn_801E2ED8(u16 soundID);
    void fn_801E2F00(u16 soundID);
    void fn_801E300C(void);
    void fn_801E3120(s32 fadeFrames = 0);
    void fn_801E334C(f32 volume, s32 fadeFrames = 0);
    void fn_801E335C(u32 trackBitFlag, bool mute);
    void fn_801E33C0(bool);

    void fn_801E33C8(u16 soundID);
    void fn_801E33F0(u16 soundID);
    void fn_801E34EC(void);
    void fn_801E3690(s32 fadeFrames = 0);
    void fn_801E38BC(f32 volume, s32 fadeFrames = 0);
    bool fn_801E38CC(void);

    void fn_801E3938(u16 soundID);
    void fn_801E3960(u16 soundID);
    void fn_801E3A5C(void);
    void fn_801E3C00(s32 fadeFrames = 0);
    void fn_801E3E2C(f32 volume, s32 fadeFrames = 0);
    bool fn_801E3E3C(void);

    f32 fn_801E3EA8(s32);

    void fn_801E4048(bool paused);
    bool fn_801E4078(void);
    bool fn_801E40A8(void);
    bool fn_801E40D8(void);
    s32 fn_801E4124(void);
    void fn_801E4134(s32, u16);
    u16 fn_801E4144(s32);

    void fn_801E4154(SNDHandle *soundHandle);
    SNDHandle *fn_801E415C(void);

    void fn_801E4170(bool ntsc);
    bool fn_801E4178(void);

    u8 fn_801E4180(void);

    EBGMType getCurrentBGMType(void) {
        return mCurrentBGMType;
    }
    bool getPaused(void) {
        return mPaused;
    }

    void setUnkF8(const TickFlowCode *code) { mSceneTransHandler = code; }

    bool getUnkFC(void) const { return mSkipAllowed; }
    void setUnkFC(bool flag) { mSkipAllowed = flag; }

    void setUnkFD(u8 value) { mSkipControllerIndex = value; }

    void setUnk100(u32 value) { mSkipButton = value; }

private:
    void pushTickFlow(CTickFlow *tickFlow) {
        tickFlow->insertBefore(mTickFlowHead);
        mTickFlowHead = tickFlow;
    }

    CTickFlow *createTickFlowDerive(void *block, CTickFlow *target) {
        CTickFlow *tickFlow = mTickFlowCreateFn(block, NULL, 0.0f);
        target->_18(tickFlow);

        return tickFlow;
    }

    f32 calcTicksPerFrame(void) {
        f32 tempo = mTempo * mSpeed;
        return tempo * 48.0f / 60.0f / fn_801E4180();
    }

    SNDHandle *nextBGMSoundHandle(void) {
        if (mCurrentSoundHandle != &mSoundHandle48) {
            return &mSoundHandle48;
        }
        else {
            return &mSoundHandle50;
        }
    }

    u32 getUnkF0(void) const {
        return mUnkF0;
    }

    f32 rethinkTempo(void) {
        if (getUnkF0() == 0) {
            return mTempo;
        }
        else {
            return fn_801E3EA8(32) * 60.0f * fn_801E4180() / 48.0f / mSpeed;
        }
    }

private:
    CTickFlow::CreateFn mTickFlowCreateFn;
    u32 mTickFlowSize;

    u8 *mHeapStart;
    MEMiHeapHead *mHeap;

    CTickFlow *mTickFlowHead;
    CTickFlow *mTickFlowTail;

    EBGMType mCurrentBGMType;

    f32 mTempo;
    f32 mTickPass;

    u32 mUnk28;

    u16 mPreparedSeqSoundID;
    bool mUnk2E;

    u16 mStrmSoundID;
    u16 mPreparedStrmSoundID;

    u32 mUnk34;
    bool mUnk38;

    u16 mWaveSoundID;
    u16 mPreparedWaveSoundID;

    u32 mUnk40;
    bool mUnk44;

    SNDHandle mSoundHandle48;
    SNDHandle mSoundHandle50;

    SNDHandle *mCurrentSoundHandle;
    SNDHandle *mPrepareSoundHandle;

    f32 mSpeed;
    f32 mTicksPerFrame;
    f32 mCurTickPass;

    bool mUnk6C;

    bool mPaused;

    f32 mUnk70[32];

    s32 mUnkF0;

    const TickFlowCode *mSkipHandler;
    const TickFlowCode *mSceneTransHandler;

    bool mSkipAllowed;
    u8 mSkipControllerIndex;
    u32 mSkipButton; // WPADButton

    bool mUnk104;
    bool mUnk105;

    u32 mUnk108;

    s32 mUnk10C;

    struct_801ED6A8 *mUnk110;

    u16 mUnk114[10];

    SNDHandle *mUnk128;

    bool mNtsc;
};


extern CTickFlowManager *gTickFlowManager;

#endif