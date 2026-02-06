#include "TickFlowManager.hpp"

#include <revolution/WPAD.h>

#include <nw4r/snd.h>

#include "Mem.hpp"

#define CTICKFLOWMANAGER_DEBUG_HEAP_ALLOC(block)                \
    do {                                                        \
        if ((block) == NULL) {                                  \
            OSReport(                                           \
                "MEMGetTotalFreeSizeForExpHeap()   = %d\n",     \
                MEMGetTotalFreeSizeForExpHeap(this->mHeap)      \
            );                                                  \
            OSReport(                                           \
                "MEMGetAllocatableSizeForExpHeap() = %d\n",     \
                MEMGetAllocatableSizeForExpHeap(this->mHeap)    \
            );                                                  \
        }                                                       \
    } while (0)

static s32 sTickFlowInstCount;

CTickFlowManager::CTickFlowManager(void) {
    mTickFlowHead = NULL;
    mUnk110 = NULL;
}

CTickFlowManager::~CTickFlowManager(void) {
    _08();
}

void CTickFlowManager::_10(u32 heapSize) {
    heapSize = ROUND_UP(heapSize, 32);

    mHeapStart = new (eHeap_MEM2, 32) u8[heapSize];
    mHeap = MEMCreateExpHeap(mHeapStart, heapSize);
}

void CTickFlowManager::_08(void) {
    gSoundManager->fn_801E7954();

    mTickFlowHead->removeAll();
    mTickFlowHead = NULL;

    if (mCurrentBGMType == eBGMType_Seq) {
        fn_801E3120();
    }
    else if (mCurrentBGMType == eBGMType_Strm) {
        fn_801E3690();
    }
    else if (mCurrentBGMType == eBGMType_Wave) {
        fn_801E3C00();
    }

    MEMDestroyExpHeap(mHeap);
    delete[] mHeapStart;
}

void CTickFlowManager::_14(CTickFlow::CreateFn createFn, u32 size, const TickFlowCode *tickFlowCode) {
    _1C(createFn, size);

    if (tickFlowCode != NULL) {
        void *block = MEMAllocFromExpHeap(mHeap, mTickFlowSize);
        CTICKFLOWMANAGER_DEBUG_HEAP_ALLOC(block);

        mTickFlowHead = mTickFlowCreateFn(block, tickFlowCode, 0.0f);
    }
    else {
        mTickFlowHead = NULL;
    }

    mTickFlowTail = NULL;

    fn_801E2C04(1.0f);
    fn_801E2B9C(120.0f);
    fn_801E4048(false);

    mSkipHandler = NULL;
    mSceneTransHandler = NULL;
    mSkipAllowed = false;
    mSkipControllerIndex = 0;
    mSkipButton = WPAD_BUTTON_MINUS;

    mStrmSoundID = -1;
    mWaveSoundID = -1;

    mPreparedSeqSoundID  = -1;
    mPreparedStrmSoundID = -1;
    mPreparedWaveSoundID = -1;

    mUnk104 = false;
    mUnk105 = false;

    mUnk10C = 0;

    mUnk114[0] = 32;
    mUnk114[1] = 16;
    mUnk114[2] = 16;
    mUnk114[3] = 8;
    mUnk114[4] = 0;
    mUnk114[5] = 0;
    mUnk114[6] = 0;
    mUnk114[7] = 0;
    mUnk114[8] = 0;
    mUnk114[9] = 0;

    mUnk128 = NULL;
}

void CTickFlowManager::fn_801E0578(void) {
    nw4r::snd::SeqSoundHandle seqHandle (mCurrentSoundHandle);
    if (seqHandle.IsAttachedSound()) {
        u32 prevTick = mUnk28;
        u32 curTick = seqHandle.GetTick();
        u32 tickPass = curTick - prevTick;

        mUnk28 = curTick;

        mUnk70[mUnkF0 % ARRAY_LENGTH(mUnk70)] = tickPass;
        mUnkF0++;

        if (mUnk2E) {
            f32 temp = fn_801E3EA8(8);
            mTicksPerFrame = temp + (mUnk28 - (mTickPass + temp)) * 0.1f; 
        }
        else {
            mTicksPerFrame = mUnk28 - mTickPass;
        }

        mTickPass += mTicksPerFrame;
    }
    else {
        fn_801E3120();
    }
}

void CTickFlowManager::fn_801E0A0C(void) {
    nw4r::snd::StrmDataInfo strmDataInfo;
    nw4r::snd::StrmSoundHandle strmHandle (mCurrentSoundHandle);
    
    bool handleOK = strmHandle.IsAttachedSound();
    if (handleOK) {
        handleOK = strmHandle.ReadStrmDataInfo(&strmDataInfo);
    }

    if (
        ((mUnk10C == 2) && !fn_801ED7B8(mUnk110)) ||
        ((mUnk10C != 2) && handleOK)
    ) {
        WaveInfo *waveInfo = gSoundManager->fn_801E73D4(mStrmSoundID);
        s32 len = gSoundManager->fn_801E748C(waveInfo);

        s32 pos;
        if (mUnk10C == 2) {
            pos = fn_801ED6A8(mUnk110);
        }
        else {
            pos = strmHandle.GetPlaySamplePosition();

            if (mUnk10C == 1) {
                pos = fn_801ED704(mUnk110, pos);
            }
        }

        if (pos == len) {
            OSReport("calcTickPassStrm() : pos == len; -> pos--\n");
            pos--;
        }

        if (pos < 0) {
            fn_801E3690();
            return;
        }

        if (len < pos) {
            pos = len;
        }

        WaveTempo *tempoPrev = gSoundManager->fn_801E7414(waveInfo, mUnk34);
        WaveTempo *tempoCur = gSoundManager->fn_801E7414(waveInfo, pos);

        if (tempoPrev == tempoCur) {
            if (pos < static_cast<s32>(mUnk34)) {
                if ((tempoCur->flag & WAVE_TEMPO_FLAG_LAST_LOOP) == 0) {
                    fn_801E3690();
                    OSReport("stopStrm()\n");
                    return;
                }

                mTicksPerFrame = tempoCur->beatCount * 48.0f * (f32)(pos - mUnk34 + tempoCur->sampleCount) / (f32)tempoCur->sampleCount;
            }
            else {
                mTicksPerFrame = tempoCur->beatCount * 48.0f * (f32)(pos - mUnk34) / (f32)tempoCur->sampleCount;
            }
        }
        else {
            s32 sc = gSoundManager->fn_801E7450(waveInfo, tempoCur);

            mTicksPerFrame =
                (tempoPrev->beatCount * 48.0f * (sc - mUnk34) / tempoPrev->sampleCount) +
                (tempoPrev->beatCount * 48.0f * (pos - sc) / tempoCur->sampleCount);

            mUnk70[mUnkF0 % ARRAY_LENGTH(mUnk70)] = mTicksPerFrame;
            mUnk34 = pos;

            mUnkF0++;

            if (mUnk38) {
                mUnk38 = false;
            }
        }
    }
    else {
        fn_801E3690();
    }
}

CTickFlow *CTickFlowManager::fn_801E1CC0(const TickFlowCode *code, f32 initRest) {
    void *block = MEMAllocFromExpHeap(mHeap, mTickFlowSize);
    CTICKFLOWMANAGER_DEBUG_HEAP_ALLOC(block);

    pushTickFlow(mTickFlowCreateFn(block, code, initRest));

    mUnk6C = true;

    return mTickFlowHead;
}

bool CTickFlowManager::fn_801E1D9C(CTickFlow *tickFlow) {
    for (CTickFlow *current = mTickFlowHead; current != NULL; current = current->getNext()) {
        if (current == tickFlow) {
            return true;
        }
    }
    return false;
}

bool CTickFlowManager::fn_801E1DC8(CTickFlow *tickFlow) {
    if (tickFlow == NULL) {
        return false;
    }
    if (!fn_801E1D9C(tickFlow)) {
        return false;
    }

    if (tickFlow == mTickFlowHead) {
        mTickFlowHead = tickFlow->getNext();
    }
    tickFlow->removeCurrent();

    return true;
}

void CTickFlowManager::fn_801E1E4C(void) {
    mTickFlowHead->removeAll();
    mTickFlowHead = NULL;

    if (mCurrentBGMType == eBGMType_Seq) {
        fn_801E3120();
    }
    else if (mCurrentBGMType == eBGMType_Strm) {
        fn_801E3690();
    }
    else if (mCurrentBGMType == eBGMType_Wave) {
        fn_801E3C00();
    }
}

// TODO: match
void CTickFlowManager::fn_801E2490(s32 category) {
    CTickFlow *inst = mTickFlowHead;
    for (;;) {
        CTickFlow *next;
        while (inst != NULL && inst->getCategory() <= category) {
            next = inst->getNext();
            inst = next;
        }
    
        fn_801E1DC8(inst);

        if (mTickFlowHead == NULL) {
            return;
        }
    }
}

void CTickFlowManager::fn_801E2540(CTickFlow *tickFlow) {
    tickFlow->_14();
    MEMFreeToExpHeap(mHeap, static_cast<void *>(tickFlow));
}

// TODO: regswaps
void CTickFlowManager::_1C(CTickFlow::CreateFn createFn, u32 size) {
    mTickFlowCreateFn = createFn;
    mTickFlowSize = size;

    CTickFlow *current = mTickFlowHead, *next;
    bool first = true;
    while (current != NULL) {
        next = current->getNext();

        void *block = MEMAllocFromExpHeap(mHeap, mTickFlowSize);
        CTICKFLOWMANAGER_DEBUG_HEAP_ALLOC(block);

        CTickFlow *tickFlow = createTickFlowDerive(block, current);

        tickFlow->insertBefore(current);
        current->removeCurrent();

        current = next;
        if (first) {
            mTickFlowHead = tickFlow;
            first = false;
        }
    }
}

f32 CTickFlowManager::fn_801E2698(void) {
    return mTicksPerFrame;
}

f32 CTickFlowManager::fn_801E26A0(void) {
    return mCurTickPass;
}

void CTickFlowManager::fn_801E26A8(void) {
    mCurTickPass = 0.0f;
}

f32 CTickFlowManager::fn_801E26B4(f32 ticks) {
    f32 ticksPerFrame = (fn_801E2CA8() * 48.0f) / (fn_801E4180() * 60.0f);
    return ticks / ticksPerFrame;
}

f32 CTickFlowManager::fn_801E2928(f32 frames) {
    f32 ticksPerFrame = (fn_801E2CA8() * 48.0f) / (fn_801E4180() * 60.0f);
    return ticksPerFrame * frames;
}

void CTickFlowManager::fn_801E2B9C(f32 tempo) {
    mCurrentBGMType = eBGMType_None;
    mTempo = tempo;
    mTicksPerFrame = calcTicksPerFrame();
}

void CTickFlowManager::fn_801E2C04(f32 speed) {
    f32 prevSpeed = mSpeed;
    
    mSpeed = speed;
    mTicksPerFrame = mTicksPerFrame * (f32)(f64)speed / prevSpeed;

    if (mCurrentBGMType == eBGMType_Seq) {
        nw4r::snd::SeqSoundHandle seqHandle (mCurrentSoundHandle);
        if (seqHandle.IsAttachedSound()) {
            seqHandle.SetTempoRatio(mSpeed);
        }
    }
    else if ((mCurrentBGMType == eBGMType_Strm) || (mCurrentBGMType == eBGMType_Wave)) {
        mCurrentSoundHandle->SetPitch((f32)(f64)speed);
    }
}

f32 CTickFlowManager::fn_801E2CA8(void) {
    if (mCurrentBGMType == eBGMType_None) {
        return mTempo * mSpeed;
    }
    else if (
        (mCurrentBGMType == eBGMType_Seq) ||
        (mCurrentBGMType == eBGMType_Strm) || (mCurrentBGMType == eBGMType_Wave)
    ) {
        return rethinkTempo() * mSpeed;
    }
    else {
        return 0.0f;
    }
}

void CTickFlowManager::fn_801E2ED8(u16 soundID) {
    mPreparedSeqSoundID = soundID;
    mPrepareSoundHandle = nextBGMSoundHandle();

    gSoundManager->prepare(soundID, mPrepareSoundHandle);
}

void CTickFlowManager::fn_801E2F00(u16 soundID) {
    mCurrentBGMType = eBGMType_Seq;
    mCurrentSoundHandle = &mSoundHandle48;

    gSoundManager->play(soundID, 0.0f, mCurrentSoundHandle);

    nw4r::snd::SeqSoundHandle seqHandle (mCurrentSoundHandle);
    if (seqHandle.IsAttachedSound()) {
        seqHandle.SetTempoRatio(mSpeed);
    }

    mUnk28 = 0;

    mUnkF0 = 0;
    mTickPass = 0.0f;
    for (s32 i = 0; i < (s32)ARRAY_LENGTH(mUnk70); i++) {
        mUnk70[i] = 0.0f;
    }
    mUnk2E = true;
}

void CTickFlowManager::fn_801E300C(void) {
    mCurrentBGMType = eBGMType_Seq;
    mPreparedSeqSoundID = -1;
    mCurrentSoundHandle = mPrepareSoundHandle;

    gSoundManager->fn_801E60B4(mCurrentSoundHandle);

    nw4r::snd::SeqSoundHandle seqHandle (mCurrentSoundHandle);
    if (seqHandle.IsAttachedSound()) {
        seqHandle.SetTempoRatio(mSpeed);
    }

    mUnk28 = 0;

    mUnkF0 = 0;
    mTickPass = 0.0f;
    for (s32 i = 0; i < (s32)ARRAY_LENGTH(mUnk70); i++) {
        mUnk70[i] = 0.0f;
    }
    mUnk2E = true;
}

void CTickFlowManager::fn_801E3120(s32 fadeFrames) {
    mCurrentBGMType = eBGMType_None;
    gSoundManager->fn_801E62B8(fadeFrames, mCurrentSoundHandle);

    mTempo = rethinkTempo();
}

void CTickFlowManager::fn_801E334C(f32 volume, s32 fadeFrames) {
    gSoundManager->fn_801E65F4(volume, fadeFrames, mCurrentSoundHandle);
}

void CTickFlowManager::fn_801E335C(u32 trackBitFlag, bool mute) {
    nw4r::snd::SeqSoundHandle seqHandle (mCurrentSoundHandle);
    if (seqHandle.IsAttachedSound()) {
        seqHandle.SetTrackMute(trackBitFlag, mute);
    }
}

void CTickFlowManager::fn_801E33C0(bool value) {
    mUnk2E = value;
}

void CTickFlowManager::fn_801E33C8(u16 soundID) {
    mPreparedStrmSoundID = soundID;
    mPrepareSoundHandle = nextBGMSoundHandle();

    gSoundManager->prepare(soundID, mPrepareSoundHandle);
}

void CTickFlowManager::fn_801E33F0(u16 soundID) {
    mStrmSoundID = soundID;
    mCurrentBGMType = eBGMType_Strm;
    mCurrentSoundHandle = &mSoundHandle48;

    gSoundManager->play(mStrmSoundID, 0.0f, mCurrentSoundHandle);

    mCurrentSoundHandle->SetPitch(mSpeed);

    mUnk38 = true;
    mUnk34 = 0;

    mUnkF0 = 0;
    for (s32 i = 0; i < (s32)ARRAY_LENGTH(mUnk70); i++) {
        mUnk70[i] = 0.0f;
    }
}

void CTickFlowManager::fn_801E34EC(void) {
    mStrmSoundID = mPreparedStrmSoundID;
    mCurrentBGMType = eBGMType_Strm;
    mPreparedStrmSoundID = -1;
    
    while (!fn_801E38CC()) {
        OSReport("Wait for Stream Data Prepared\n");
        OSSleepTicks(OS_MSEC_TO_TICKS(10ll));
    }

    mCurrentSoundHandle = mPrepareSoundHandle;
    gSoundManager->fn_801E60B4(mCurrentSoundHandle);

    mCurrentSoundHandle->SetPitch(mSpeed);

    mUnk38 = true;
    mUnk34 = 0;

    mUnkF0 = 0;
    for (s32 i = 0; i < (s32)ARRAY_LENGTH(mUnk70); i++) {
        mUnk70[i] = 0.0f;
    }
}

void CTickFlowManager::fn_801E3690(s32 fadeFrames) {
    mCurrentBGMType = eBGMType_None;
    gSoundManager->fn_801E62B8(fadeFrames, mCurrentSoundHandle);

    mTempo = rethinkTempo();
}

void CTickFlowManager::fn_801E38BC(f32 volume, s32 fadeFrames) {
    gSoundManager->fn_801E65F4(volume, fadeFrames, mCurrentSoundHandle);
}

bool CTickFlowManager::fn_801E38CC(void) {
    nw4r::snd::StrmSoundHandle strmHandle (mPrepareSoundHandle);
    return strmHandle.IsPrepared();
}

void CTickFlowManager::fn_801E3938(u16 soundID) {
    mPreparedWaveSoundID = soundID;
    mPrepareSoundHandle = nextBGMSoundHandle();

    gSoundManager->prepare(soundID, mPrepareSoundHandle);
}

void CTickFlowManager::fn_801E3960(u16 soundID) {
    mWaveSoundID = soundID;
    mCurrentBGMType = eBGMType_Wave;
    mCurrentSoundHandle = &mSoundHandle48;

    gSoundManager->play(mWaveSoundID, 0.0f, mCurrentSoundHandle);

    mCurrentSoundHandle->SetPitch(mSpeed);

    mUnk44 = true;
    mUnk40 = 0;

    mUnkF0 = 0;
    for (s32 i = 0; i < (s32)ARRAY_LENGTH(mUnk70); i++) {
        mUnk70[i] = 0.0f;
    }
}

void CTickFlowManager::fn_801E3A5C(void) {
    mWaveSoundID = mPreparedWaveSoundID;
    mCurrentBGMType = eBGMType_Wave;
    mPreparedWaveSoundID = -1;
    
    while (!fn_801E3E3C()) {
        OSReport("Wait for Wave Data Prepared\n");
        OSSleepTicks(OS_MSEC_TO_TICKS(10ll));
    }

    mCurrentSoundHandle = mPrepareSoundHandle;
    gSoundManager->fn_801E60B4(mCurrentSoundHandle);

    mCurrentSoundHandle->SetPitch(mSpeed);

    mUnk44 = true;
    mUnk40 = 0;

    mUnkF0 = 0;
    for (s32 i = 0; i < (s32)ARRAY_LENGTH(mUnk70); i++) {
        mUnk70[i] = 0.0f;
    }
}

void CTickFlowManager::fn_801E3C00(s32 fadeFrames) {
    mCurrentBGMType = eBGMType_None;
    gSoundManager->fn_801E62B8(fadeFrames, mCurrentSoundHandle);

    mTempo = rethinkTempo();
}

void CTickFlowManager::fn_801E3E2C(f32 volume, s32 fadeFrames) {
    gSoundManager->fn_801E65F4(volume, fadeFrames, mCurrentSoundHandle);
}

bool CTickFlowManager::fn_801E3E3C(void) {
    nw4r::snd::WaveSoundHandle waveHandle (mPrepareSoundHandle);
    return waveHandle.IsPrepared();
}

f32 CTickFlowManager::fn_801E3EA8(s32 param_1) {
    if (getUnkF0() == 0) {
        return mTicksPerFrame;
    }
    else {
        param_1 = MIN(getUnkF0(), param_1);

        f32 value = 0.0f;
        for (s32 i = 0; i < param_1; i++) {
            u32 j = (getUnkF0() + 31) - i;
            value += mUnk70[j % ARRAY_LENGTH(mUnk70)];
        }

        return value / param_1;
    }
}

void CTickFlowManager::fn_801E4048(bool paused) {
    mPaused = paused;

    if (
        (mCurrentBGMType == eBGMType_Seq) ||
        (mCurrentBGMType == eBGMType_Strm) || (mCurrentBGMType == eBGMType_Wave)
    ) {
        gSoundManager->fn_801E6440(mPaused, 0, mCurrentSoundHandle);
    }
}

bool CTickFlowManager::fn_801E4078(void) {
    for (CTickFlow *cur = mTickFlowHead; cur != NULL; cur = cur->getNext()) {
        if (cur->getButtonPromptIsReleased()) {
            return true;
        }
    }
    return false;
}

bool CTickFlowManager::fn_801E40A8(void) {
    for (CTickFlow *cur = mTickFlowHead; cur != NULL; cur = cur->getNext()) {
        if (cur->getButtonPromptIsPressed()) {
            return true;
        }
    }
    return false;
}

bool CTickFlowManager::fn_801E40D8(void) {
    for (CTickFlow *cur = mTickFlowHead; cur != NULL; cur = cur->getNext()) {
        if (cur->getButtonPromptIsPressedOrReleased()) {
            return true;
        }
    }
    return false;
}

s32 CTickFlowManager::fn_801E4124(void) {
    return ++sTickFlowInstCount;
}

void CTickFlowManager::fn_801E4134(s32 index, u16 param_2) {
    mUnk114[index] = param_2;
}

u16 CTickFlowManager::fn_801E4144(s32 index) {
    return mUnk114[index];
}

void CTickFlowManager::fn_801E4154(SNDHandle *soundHandle) {
    mUnk128 = soundHandle;
}

SNDHandle *CTickFlowManager::fn_801E415C(void) {
    SNDHandle *soundHandle = mUnk128;
    mUnk128 = NULL;

    return soundHandle;
}

void CTickFlowManager::fn_801E4170(bool ntsc) {
    mNtsc = ntsc;
}

bool CTickFlowManager::fn_801E4178(void) {
    return mNtsc;
}

u8 CTickFlowManager::fn_801E4180(void) {
    return mNtsc ? 60 : 50;
}


