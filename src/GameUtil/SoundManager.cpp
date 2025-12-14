#include "SoundManager.hpp"

#include <cstring>

#include <revolution/AI.h>

#include "FileManager.hpp"

#include "TickFlowManager.hpp"

SNDHandle::~SNDHandle(void) {
    gSoundManager->end_sound_handle(this);
}

CSoundManager::CSoundManager(void) {
    AIInit(NULL);

    nw4r::snd::SoundSystem::InitSoundSystem(
        nw4r::snd::SoundSystem::DEFAULT_SOUND_THREAD_PRIORITY,
        nw4r::snd::SoundSystem::DEFAULT_DVD_THREAD_PRIORITY
    );

    mFxReverbHi0 = NULL;
    mFxReverbHi1 = NULL;

    for (s32 i = 0; i < (s32)ARRAY_LENGTH(mDelaySound); i++) {
        mDelaySound[i].reset();
    }
}

CSoundManager::~CSoundManager(void) {
    _08();
}

void CSoundManager::_10(void *soundArchiveAddr) {
    mSoundArchiveType = eSoundArchiveType_Mem;
    mMemSoundArchiveData = static_cast<u8 *>(soundArchiveAddr);

    gFileManager->fn_801D3E94();

    mMemSoundArchive = new nw4r::snd::MemorySoundArchive;
    mMemSoundArchive->Setup(mMemSoundArchiveData);

    mSoundArchivePlayer = new nw4r::snd::SoundArchivePlayer;

    u32 setupBufSize = mSoundArchivePlayer->GetRequiredMemSize(mMemSoundArchive);
    u32 setupStrmBufSize = mSoundArchivePlayer->GetRequiredStrmBufferSize(mMemSoundArchive);

    mSetupBuf = new (eHeap_MEM2, 32) u8[setupBufSize];
    mSetupStrmBuf = new (eHeap_MEM2, 32) u8[setupStrmBufSize];

    mSoundArchivePlayer->Setup(
        mMemSoundArchive,
        mSetupBuf, setupBufSize, mSetupStrmBuf, setupStrmBufSize
    );

    fn_801E734C();

    mFxReverbHi0 = new nw4r::snd::FxReverbHi;
    mFxReverbHi1 = new nw4r::snd::FxReverbHi;
    
    mFxReverbHi0->SetParam(mFxReverbHi0Param);
    mFxReverbHi1->SetParam(mFxReverbHi1Param);

    reverb_init();
}

void CSoundManager::_14(const char *soundArchivePath) {
    strcpy(mLoadData.soundArchivePath, soundArchivePath);
    mLoadData.loaded = false;

    OSCreateThread(
        &mThread, fn_801E4948, &mLoadData,
        &mThreadStack[ARRAY_LENGTH(mThreadStack)], sizeof(mThreadStack),
        OS_PRIORITY_MAX, OS_THREAD_DETACHED
    );
    OSResumeThread(&mThread);

    while (!mLoadData.loaded) {
        gFileManager->waitLoadFinalizeTick();
    }
}

void *CSoundManager::fn_801E4948(void *arg) {
    LoadData *data = static_cast<LoadData *>(arg);

    gSoundManager->fn_801E4988(data->soundArchivePath);
    data->loaded = true;

    return NULL;
}

void CSoundManager::fn_801E4988(const char *soundArchivePath) {
    mSoundArchiveType = eSoundArchiveType_DVD;

    mDVDSoundArchive = new nw4r::snd::DvdSoundArchive;
    mDVDSoundArchive->Open(soundArchivePath);

    u32 headerSize = mDVDSoundArchive->GetHeaderSize();
    mDVDSoundArchiveHeader = new (eHeap_MEM2, 32) u8[headerSize];

    mDVDSoundArchive->LoadHeader(mDVDSoundArchiveHeader, headerSize);

    mSoundArchivePlayer = new nw4r::snd::SoundArchivePlayer;

    u32 setupBufSize = mSoundArchivePlayer->GetRequiredMemSize(mDVDSoundArchive);
    u32 setupStrmBufSize = mSoundArchivePlayer->GetRequiredStrmBufferSize(mDVDSoundArchive);

    mSetupBuf = new (eHeap_MEM2, 32) u8[setupBufSize];
    mSetupStrmBuf = new (eHeap_MEM2, 32) u8[setupStrmBufSize];

    mSoundArchivePlayer->Setup(
        mDVDSoundArchive,
        mSetupBuf, setupBufSize, mSetupStrmBuf, setupStrmBufSize
    );

    fn_801E734C();

    mFxReverbHi0 = new nw4r::snd::FxReverbHi;
    mFxReverbHi1 = new nw4r::snd::FxReverbHi;
    
    mFxReverbHi0->SetParam(mFxReverbHi0Param);
    mFxReverbHi1->SetParam(mFxReverbHi1Param);

    reverb_init();
}

void CSoundManager::_18(void) {
    mSoundArchiveType = eSoundArchiveType_Nand;

    // NOTE: NAND Sound Archive init impl is mysteriously missing
}

void CSoundManager::_08(void) {
    switch (mSoundArchiveType) {
    case eSoundArchiveType_Mem: {
        mMemSoundArchive->Shutdown();
        delete mMemSoundArchive;
        delete[] mMemSoundArchiveData;
    } break;

    case eSoundArchiveType_DVD: {
        mDVDSoundArchive->Close();
        delete mDVDSoundArchive;
        delete[] mDVDSoundArchiveHeader;
    } break;

    case eSoundArchiveType_Nand: {
        mNandSoundArchive->Close();
        delete mNandSoundArchive;
        delete[] mDVDSoundArchiveHeader;
    } break;
    }

    mSoundArchivePlayer->Shutdown();
    delete mSoundArchivePlayer;

    delete[] mSetupBuf;
    delete[] mSetupStrmBuf;

    if (mFxReverbHi0 != NULL) {
        delete mFxReverbHi0;
        delete[] mFxReverbHi0Work;
        mFxReverbHi0 = NULL;
        mFxReverbHi0Work = NULL;
    }

    if (mFxReverbHi1 != NULL) {
        delete mFxReverbHi1;
        delete[] mFxReverbHi1Work;
        mFxReverbHi1 = NULL;
        mFxReverbHi1Work = NULL;
    }
}

void CSoundManager::_1C(void) {
    mSystemPlayerID = 0;

    mVolumeFadeFrames = 0;
    mVolumeFadeStep = 0.0f;

    mSoundCooldown = NULL;
    mSoundCooldownCount = 0;

    mNoSoloSystemPlayer = true;

    nw4r::snd::SoundSystem::SetMasterVolume(0.7f, 0);
}

// TODO: rename to reflect purpose better (see usage of fn_801E4D4C)
static bool sEnableSoundUpdate;

bool CSoundManager::fn_801E4D4C(void) {
    return sEnableSoundUpdate;
}

void CSoundManager::fn_801E4D54(void) {
    sEnableSoundUpdate = true;
}

void CSoundManager::fn_801E4D60(void) {
    for (s32 i = 0; i < mSoundCooldownCount; i++) {
        SoundCooldown *entry = &mSoundCooldown[i];
        if (entry->coolTimer != 0) {
            entry->coolTimer--;
        }
    }

    fn_801E5648();

    if ((mVolumeFadeFrames > 0) && !gTickFlowManager->getUnk6D()) {
        f32 curVolume = first_player_volume();

        f32 finalVolume = curVolume + mVolumeFadeStep;
        if (finalVolume < 0.0) {
            finalVolume = 0.0;
        }

        fn_801E6E4C(finalVolume);

        mVolumeFadeFrames--;
    }

    if (!mNoSoloSystemPlayer) {
        fn_801E7008(0);
    }

    mSoundArchivePlayer->Update();
}

void CSoundManager::prepare(u16 soundID, SNDHandle *soundHandle) {
    if (soundHandle == NULL) {
        soundHandle = &mDefaultSoundHandle;
    }

    nw4r::snd::SoundStartable::StartResult result =
        mSoundArchivePlayer->PrepareSound(soundHandle, soundID);

    if (result != nw4r::snd::SoundStartable::START_SUCCESS) {
        OSReport(
            "CSoundManager::prepare( %d ) : Error %d\n",
            soundID, static_cast<s32>(result)
        );
    }
}

void CSoundManager::play(u16 soundID, f32 start, SNDHandle *soundHandle) {
    SoundCooldown *cooldown = find_sound_cooldown(soundID);
    if (cooldown != NULL) {
        if (cooldown->coolTimer != 0) {
            // Cooldown is still active: don't play.
            return;
        }
        // Restart cooldown.
        cooldown->coolTimer = cooldown->coolFrames;
    }

    mLastSoundType = eSoundType_None;

    if (soundHandle == NULL) {
        soundHandle = &mDefaultSoundHandle;
    }

    if (start >= 0.0f) {
        nw4r::snd::SoundStartable::StartInfo startInfo;

        startInfo.seqSoundInfo.seqDataAddress = NULL;
        startInfo.seqSoundInfo.startLocationLabel = NULL;
        startInfo.enableFlag = nw4r::snd::SoundStartable::StartInfo::ENABLE_START_OFFSET;
        startInfo.startOffsetType = nw4r::snd::SoundStartable::StartInfo::START_OFFSET_TYPE_TICK;
        startInfo.startOffset = static_cast<s32>(start); // NOTE: Decimal precision is lost.

        nw4r::snd::SoundStartable::StartResult result =
            mSoundArchivePlayer->StartSound(soundHandle, soundID, &startInfo);

        if (result != nw4r::snd::SoundStartable::START_SUCCESS) {
            OSReport("CSoundManager::play( %d ) : Error %d\n", soundID, static_cast<s32>(result));
        }

        mLastSoundType = eSoundType_Direct;
        mLastDirectSound = soundHandle;
    }
    else {
        DelaySound *lastDelaySound = find_delay_sound_by_snd_handle(soundHandle);
        if (lastDelaySound != NULL) {
            lastDelaySound->soundHandle = NULL;
        }

        DelaySound *newDelaySound = first_free_delay_sound();
        if (newDelaySound == NULL) {
            // "Delayed playback of sound was ignored (due to lack of buffer).\n"
            OSReport("サウンドの遅延再生が、無視されました(原因はバッファ不足です)。\n");
            return;
        }

        newDelaySound->reset();

        newDelaySound->active = true;
        newDelaySound->soundID = soundID;
        newDelaySound->startOffset = start;
        newDelaySound->soundHandle = soundHandle;

        mLastSoundType = eSoundType_Delayed;
        mLastDelaySound = newDelaySound;
    }

    f32 tempoRatio = fn_801E75C0(soundID);
    gSoundManager->fn_801E6BC8(tempoRatio, NULL);
}

void CSoundManager::fn_801E5640(f32 ticksPerFrame) {
    mTicksPerFrame = ticksPerFrame;
}

void CSoundManager::fn_801E5648(void) {
    for (s32 i = 0; i < (s32)ARRAY_LENGTH(mDelaySound); i++) {
        DelaySound *delaySound = &mDelaySound[i];

        if (delaySound->active) {
            delaySound->startOffset += mTicksPerFrame;
            if (delaySound->startOffset < 0.0f) {
                continue;
            }
            
            gSoundManager->play(delaySound->soundID, 0.0f, delaySound->soundHandle);

            if (delaySound->volumeSet != 1.0) {
                gSoundManager->fn_801E65F4(delaySound->volumeSet, 0, delaySound->soundHandle);
            }
            if (delaySound->volumeFadeFrames > 0) {
                gSoundManager->fn_801E65F4(delaySound->volumeFinal, delaySound->volumeFadeFrames, delaySound->soundHandle);
            }
            if (delaySound->pitch != 1.0) {
                gSoundManager->fn_801E676C(delaySound->pitch, delaySound->soundHandle);
            }
            if (delaySound->pan != 0.0) {
                gSoundManager->fn_801E68E0(delaySound->pan, delaySound->soundHandle);
            }
            if (delaySound->tempoRatio != 1.0) {
                gSoundManager->fn_801E6A54(delaySound->tempoRatio, delaySound->soundHandle);
            }
            if (delaySound->stopFadeFrames >= 0) {
                gSoundManager->fn_801E62B8(delaySound->stopFadeFrames, delaySound->soundHandle);
            }

            delaySound->reset();
        }
    }

    mLastSoundType = eSoundType_None;
}

void CSoundManager::fn_801E60B4(SNDHandle *soundHandle) {
    if (soundHandle == NULL) {
        soundHandle = &mDefaultSoundHandle;
    }

    soundHandle->StartPrepared();
}

void CSoundManager::fn_801E60D4(SoundCooldown *soundCooldown, u16 count) {
    mSoundCooldown = soundCooldown;
    mSoundCooldownCount = count;
}

void CSoundManager::tune_sound_handle(ETuneType type, f32 value, s32 fadeFrames, SNDHandle *soundHandle) {
    if (soundHandle == NULL) {
        soundHandle = &mDefaultSoundHandle;
    }

    switch (type) {
    case eTuneType_Volume: {
        soundHandle->SetVolume(value, fadeFrames);
    } break;

    case eTuneType_Pitch: {
        soundHandle->SetPitch(value);
    } break;
    
    case eTuneType_Pan: {
        soundHandle->SetPan(value);
    } break;

    case eTuneType_TempoRatio: {
        nw4r::snd::SeqSoundHandle seqHandle (soundHandle);
        if (seqHandle.IsAttachedSound()) {
            seqHandle.SetTempoRatio(value);
        }
        else {
            OSReport("Warning!! : seqHandle is not Attached @ CSoundManager::tune_sound_handle()\n");
        }
    } break;

    case eTuneType_Stop: {
        soundHandle->Stop(fadeFrames);
    } break;

    case eTuneType_Pause: {
        soundHandle->Pause(value != 0.0f, fadeFrames);
    } break;
    }
}

void CSoundManager::tune_delay_sound(ETuneType type, f32 value, s32 fadeFrames, DelaySound *delaySound) {
    switch (type) {
    case eTuneType_Volume: {
        if (fadeFrames > 0) {
            delaySound->volumeFinal = value;
            delaySound->volumeFadeFrames = fadeFrames;
        }
        else {
            delaySound->volumeSet = value;
            delaySound->volumeFadeFrames = 0;
        }
    } return;

    case eTuneType_Pitch: {
        delaySound->pitch = value;
    } return;
    
    case eTuneType_Pan: {
        delaySound->pan = value;
    } return;

    case eTuneType_TempoRatio: {
        delaySound->tempoRatio = value;
    } return;

    case eTuneType_Stop: {
        delaySound->stopFadeFrames = fadeFrames;
    } return;

    case eTuneType_Pause: {
        delaySound->pauseFlag = value != 0.0f;
        delaySound->pauseFadeFrames = fadeFrames;
    } return;
    }
}

void CSoundManager::fn_801E62B8(s32 fadeFrames, SNDHandle *soundHandle) {
    tune(eTuneType_Stop, 0.0f, fadeFrames, soundHandle);
}

void CSoundManager::fn_801E6440(bool paused, s32 fadeFrames, SNDHandle *soundHandle) {
    tune(eTuneType_Pause, paused ? 1.0f : 0.0f, fadeFrames, soundHandle);
}

void CSoundManager::fn_801E65F4(f32 volume, s32 fadeFrames, SNDHandle *soundHandle) {
    tune(eTuneType_Volume, volume, fadeFrames, soundHandle);
}

void CSoundManager::fn_801E676C(f32 pitch, SNDHandle *soundHandle) {
    tune(eTuneType_Pitch, pitch, 0, soundHandle);
}

void CSoundManager::fn_801E68E0(f32 pan, SNDHandle *soundHandle) {
    tune(eTuneType_Pan, pan, 0, soundHandle);
}

void CSoundManager::fn_801E6A54(f32 tempoRatio, SNDHandle *soundHandle) {
    tune(eTuneType_TempoRatio, tempoRatio, 0, soundHandle);
}

void CSoundManager::fn_801E6BC8(f32 tempoRatio, SNDHandle *soundHandle) {
    if (tempoRatio != 0.0f && mTicksPerFrame != 0.0f) {
        f32 framesPerSecond = gTickFlowManager->fn_801E4180();
        f32 effective = (mTicksPerFrame * 60.0f * framesPerSecond) / 48.0f;

        fn_801E6A54(effective / tempoRatio, soundHandle);
    }
}

void CSoundManager::fn_801E6E00(s32 playerID) {
    mSystemPlayerID = playerID;
}

void CSoundManager::fn_801E6E08(f32 volume) {
    if (mSystemPlayerID >= 0) {
        mSoundArchivePlayer->GetSoundPlayer(mSystemPlayerID).SetVolume(volume);
    }
}

void CSoundManager::fn_801E6E4C(f32 volume) {
    u32 soundPlayerCount = mSoundArchivePlayer->GetSoundPlayerCount();
    for (u32 i = 0; i < soundPlayerCount; i++) {
        if (static_cast<s32>(i) != mSystemPlayerID) {
            mSoundArchivePlayer->GetSoundPlayer(i).SetVolume(volume);
        }
    }
}

void CSoundManager::fn_801E6ECC(f32 volume) {
    u32 soundPlayerCount = mSoundArchivePlayer->GetSoundPlayerCount();
    for (u32 i = 0; i < soundPlayerCount; i++) {
        mSoundArchivePlayer->GetSoundPlayer(i).SetVolume(volume);
    }
}

void CSoundManager::fn_801E6F40(bool paused, s32 fadeFrames) {
    if (mSystemPlayerID >= 0) {
        mSoundArchivePlayer->GetSoundPlayer(mSystemPlayerID).PauseAllSound(paused, fadeFrames);
    }
}

void CSoundManager::fn_801E6F98(bool paused, s32 fadeFrames) {
    u32 soundPlayerCount = mSoundArchivePlayer->GetSoundPlayerCount();
    for (u32 i = 0; i < soundPlayerCount; i++) {
        if (static_cast<s32>(i) != mSystemPlayerID) {
            mSoundArchivePlayer->GetSoundPlayer(i).PauseAllSound(paused, fadeFrames);
        }
    }
}

void CSoundManager::fn_801E7008(s32 fadeFrames) {
    u32 soundPlayerCount = mSoundArchivePlayer->GetSoundPlayerCount();
    for (u32 i = 0; i < soundPlayerCount; i++) {
        if (static_cast<s32>(i) != mSystemPlayerID) {
            mSoundArchivePlayer->GetSoundPlayer(i).StopAllSound(fadeFrames);
        }
    }
}

void CSoundManager::fn_801E7088(s32 fadeFrames) {
    u32 soundPlayerCount = mSoundArchivePlayer->GetSoundPlayerCount();
    for (u32 i = 0; i < soundPlayerCount; i++) {
        mSoundArchivePlayer->GetSoundPlayer(i).StopAllSound(fadeFrames);
    }
}

void CSoundManager::fn_801E70FC(void) {
    mNoSoloSystemPlayer = true;
}
void CSoundManager::fn_801E7108(void) {
    mNoSoloSystemPlayer = false;
}

void CSoundManager::fn_801E7114(f32 volume, s32 fadeFrames) {
    mVolumeFadeFrames = fadeFrames;

    // (end - start) / frames
    mVolumeFadeStep = (volume - first_player_volume()) / mVolumeFadeFrames;
}

void CSoundManager::fn_801E71C0(void) {
    mVolumeFadeFrames = 0;
}

void CSoundManager::fn_801E71CC(u16 groupID, nw4r::snd::SoundHeap *soundHeap) {
    mLoadData.soundHeap = soundHeap;
    mLoadData.groupID = groupID;
    mLoadData.loaded = false;

    OSCreateThread(
        &mThread, fn_801E72E8, &mLoadData,
        &mThreadStack[ARRAY_LENGTH(mThreadStack)], sizeof(mThreadStack),
        OS_PRIORITY_MAX, OS_THREAD_DETACHED
    );
    OSResumeThread(&mThread);
}

void CSoundManager::fn_801E7230(u16 groupID, nw4r::snd::SoundHeap *soundHeap) {
    fn_801E71CC(groupID, soundHeap);

    while (!mLoadData.loaded) {
        gFileManager->waitLoadFinalizeTick();
    }
}

void *CSoundManager::fn_801E72E8(void *arg) {
    LoadData *data = static_cast<LoadData *>(arg);

    gSoundManager->mSoundArchivePlayer->LoadGroup(
        data->groupID, data->soundHeap, 0
    );
    data->loaded = true;

    return NULL;
}

bool CSoundManager::fn_801E7334(void) {
    return !mLoadData.loaded;
}

void CSoundManager::fn_801E7344(nw4r::snd::SeqUserprocCallback *callback, void *callbackArg) {
    mSoundArchivePlayer->SetSeqUserprocCallback(callback, callbackArg);
}

void CSoundManager::fn_801E734C(void) {    
    mFxReverbHi0Param.earlyMode = nw4r::snd::FX_REVERB_EARLY_REFLECTION_30MS;
    mFxReverbHi0Param.preDelayTimeMax = 0.1f;
    mFxReverbHi0Param.preDelayTime = 0.1f;
    mFxReverbHi0Param.fusedMode = nw4r::snd::FX_REVERB_FUSED_OLD_AXFX;
    mFxReverbHi0Param.fusedTime = 2.1f;
    mFxReverbHi0Param.coloration = 0.6f;
    mFxReverbHi0Param.damping = 0.4f;
    mFxReverbHi0Param.crosstalk = 0.1f;
    mFxReverbHi0Param.earlyGain = 0.0f;
    mFxReverbHi0Param.fusedGain = 1.0f;
    mFxReverbHi0Param.outGain = 0.4f;

    mFxReverbHi1Param.earlyMode = nw4r::snd::FX_REVERB_EARLY_REFLECTION_30MS;
    mFxReverbHi1Param.preDelayTimeMax = 0.0f;
    mFxReverbHi1Param.preDelayTime = 0.0f;
    mFxReverbHi1Param.fusedMode = nw4r::snd::FX_REVERB_FUSED_OLD_AXFX;
    mFxReverbHi1Param.fusedTime = 2.1f;
    mFxReverbHi1Param.coloration = 0.6f;
    mFxReverbHi1Param.damping = 0.4f;
    mFxReverbHi1Param.crosstalk = 0.1f;
    mFxReverbHi1Param.earlyGain = 0.0f;
    mFxReverbHi1Param.fusedGain = 1.0f;
    mFxReverbHi1Param.outGain = 0.4f;
}

void CSoundManager::fn_801E73C8(WaveInfo *waveInfo, u16 count) {
    mWaveInfo = waveInfo;
    mWaveInfoCount = count;
}

WaveInfo *CSoundManager::fn_801E73D4(u16 soundID) {
    for (s32 i = 0; i < mWaveInfoCount; i++) {
        WaveInfo *entry = &mWaveInfo[i];
        if (entry->soundID == soundID) {
            return entry;
        }
    }
    return NULL;
}

WaveTempo *CSoundManager::fn_801E7414(WaveInfo *waveInfo, s32 pos) {
    s32 currentSample = 0;
    for (s32 i = 0;; i++) {
        WaveTempo *curTempo = &waveInfo->tempo[i];
        
        currentSample += curTempo->sampleCount;
        if (pos < currentSample) {
            return curTempo;
        }

        if (
            (curTempo->flag & WAVE_TEMPO_FLAG_LAST_LOOP) ||
            (curTempo->flag & WAVE_TEMPO_FLAG_LAST_STOP)
        ) {
            break;
        }
    }

    return NULL;
}

s32 CSoundManager::fn_801E7450(WaveInfo *waveInfo, WaveTempo *tempo) {
    s32 sampleCount = 0;
    for (s32 i = 0;; i++) {
        WaveTempo *curTempo = &waveInfo->tempo[i];
        if (curTempo == tempo) {
            return sampleCount;
        }

        if (
            (curTempo->flag & WAVE_TEMPO_FLAG_LAST_LOOP) ||
            (curTempo->flag & WAVE_TEMPO_FLAG_LAST_STOP)
        ) {
            break;
        }

        sampleCount += curTempo->sampleCount;
    }

    return -1;
}

s32 CSoundManager::fn_801E748C(WaveInfo *waveInfo) {
    s32 sampleCount = 0;
    for (s32 i = 0;; i++) {
        WaveTempo *curTempo = &waveInfo->tempo[i];
        sampleCount += curTempo->sampleCount;

        if (
            (curTempo->flag & WAVE_TEMPO_FLAG_LAST_LOOP) ||
            (curTempo->flag & WAVE_TEMPO_FLAG_LAST_STOP)
        ) {
            break;
        }
    }

    return sampleCount;
}

s32 CSoundManager::fn_801E74BC(WaveInfo *waveInfo) {
    s32 beatCount = 0;
    for (s32 i = 0;; i++) {
        WaveTempo *curTempo = &waveInfo->tempo[i];
        beatCount += curTempo->beatCount;

        if (
            (curTempo->flag & WAVE_TEMPO_FLAG_LAST_LOOP) ||
            (curTempo->flag & WAVE_TEMPO_FLAG_LAST_STOP)
        ) {
            break;
        }
    }

    return beatCount;
}

f32 CSoundManager::fn_801E74EC(WaveInfo *waveInfo) {
    WaveTempo *tempo = fn_801E7414(waveInfo, 0);

    f32 sampleRate = waveInfo->sampleRate;
    f32 samplesPerMin = sampleRate * 60.0f;

    return (static_cast<f32>(tempo->beatCount) * samplesPerMin) / tempo->sampleCount;
}

BOOL CSoundManager::fn_801E7584(WaveInfo *waveInfo) {
    for (s32 i = 0;; i++) {
        WaveTempo *curTempo = &waveInfo->tempo[i];

        if (curTempo->flag & WAVE_TEMPO_FLAG_LAST_LOOP) {
            return TRUE;
        }
        if (curTempo->flag & WAVE_TEMPO_FLAG_LAST_STOP) {
            break;
        }
    }

    return FALSE;
}

void CSoundManager::fn_801E75B4(SeqTempo *seqTempo, u16 count) {
    mSeqTempo = seqTempo;
    mSeqTempoCount = count;
}

f32 CSoundManager::fn_801E75C0(u16 soundID) {
    u16 prevSoundID = 0;

    for (u16 i = 0; i < mSeqTempoCount; i++) {
        SeqTempo *tempo = &mSeqTempo[i];

        if (tempo->type != SeqTempo::eType_FromPrev) {
            prevSoundID = tempo->soundID;
        }

        if (tempo->soundID == soundID) {
            switch (tempo->type) {
            case SeqTempo::eType_Immediate:
                return static_cast<f32>(tempo->tempoOrSID);

            case SeqTempo::eType_WaveInfoRef:
                return fn_801E7884(tempo->tempoOrSID);

            case SeqTempo::eType_Alias:
                return fn_801E75C0(tempo->tempoOrSID);

            case SeqTempo::eType_FromPrev:
                return fn_801E75C0(prevSoundID);
            }
        }
    }

    return 0.0f;
}

f32 CSoundManager::fn_801E7884(u16 soundID) {
    WaveInfo *waveInfo = fn_801E73D4(soundID);
    f32 tempo = fn_801E74EC(waveInfo);
    return tempo;
}

void CSoundManager::fn_801E7954(void) {
    for (s32 i = 0; i < (s32)ARRAY_LENGTH(mSoundHandle); i++) {
        gSoundManager->tune(eTuneType_Stop, 0.0f, 0, &mSoundHandle[i]);
    }
}

SNDHandle *CSoundManager::fn_801E7B30(s32 index) {
    return &mSoundHandle[index];
}
