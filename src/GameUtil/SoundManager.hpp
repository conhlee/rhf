#ifndef GAMEUTIL_SOUNDMANAGER_HPP
#define GAMEUTIL_SOUNDMANAGER_HPP

#include <revolution/types.h>

#include "Singleton.hpp"

#include <revolution/OS.h>

#include <nw4r/snd.h>

#include "SNDHandle.hpp"

struct SeqTempo {
    enum EType {
        eType_Immediate, // tempoOrSID represents the tempo.
        eType_WaveInfoRef, // tempoOrSID represents a wave sound ID; the wave info table is consulted for the tempo.
        eType_Alias, // tempoOrSID is the sound ID of another SeqTempo entry; that entry is consulted for the tempo.
        eType_FromPrev // tempoOrSID is unused; the last entry (without the type FromPrev) is consulted for the tempo.
    };

    u16 soundID;
    u16 type; // EType
    u16 tempoOrSID;
};

#define SEQ_TEMPO_TABLE_BEGIN(tableName) \
    SeqTempo tableName[] = {

#define SEQ_TEMPO_TABLE_ENTRY(soundName, tempo) \
    { soundName, SeqTempo::eType_Immediate, (tempo) },
#define SEQ_TEMPO_TABLE_ENTRY_WAVE_INFO_REF(soundName, waveName) \
    { soundName, SeqTempo::eType_WaveInfoRef, waveName },
#define SEQ_TEMPO_TABLE_ENTRY_ALIAS(soundName, srcSoundName) \
    { soundName, SeqTempo::eType_Alias, srcSoundName },
#define SEQ_TEMPO_TABLE_ENTRY_FROM_PREV(soundName) \
    { soundName, SeqTempo::eType_FromPrev, -1 },

#define SEQ_TEMPO_TABLE_END() \
    };

#define WAVE_TEMPO_FLAG_NONE ((u16)(0))
#define WAVE_TEMPO_FLAG_LAST_LOOP ((u16)(1 << 0))
#define WAVE_TEMPO_FLAG_LAST_STOP ((u16)(1 << 15))

struct WaveTempo {
    u32 beatCount;
    u32 sampleCount;
    u16 flag; // WAVE_TEMPO_FLAG_
};

#define WAVE_TEMPO_BEGIN(soundName) \
    WaveTempo waveTempo_##soundName[] = {

#define WAVE_TEMPO_ENTRY(beatCount, sampleCount) \
    { (beatCount), (sampleCount), WAVE_TEMPO_FLAG_NONE },
#define WAVE_TEMPO_ENTRY_LAST_LOOP(beatCount, sampleCount) \
    { (beatCount), (sampleCount), WAVE_TEMPO_FLAG_LAST_LOOP },
#define WAVE_TEMPO_ENTRY_LAST_STOP(beatCount, sampleCount) \
    { (beatCount), (sampleCount), WAVE_TEMPO_FLAG_LAST_STOP },

#define WAVE_TEMPO_END() \
    };

struct WaveInfo {
    u16 soundID;
    u16 sampleRate;
    WaveTempo *tempo; // WaveTempo[], cannot be NULL
};

#define WAVE_INFO_TABLE_BEGIN(tableName) \
    WaveInfo tableName[] = {

#define WAVE_INFO_TABLE_ENTRY(soundName, sampleRate) \
    { soundName, (sampleRate), waveTempo_##soundName },

#define WAVE_INFO_TABLE_END() \
    };

struct SoundCooldown {
    u16 soundID;
    u16 coolFrames;
    u16 coolTimer; // Mutable.
};

#define SOUND_COOLDOWN_TABLE_BEGIN(tableName) \
    SoundCooldown tableName[] = {

#define SOUND_COOLDOWN_TABLE_ENTRY(soundID, frames) \
    { (soundID), (frames), 0 },

#define SOUND_COOLDOWN_TABLE_END() \
    };

class CSoundManager : public TSingleton<CSoundManager> {
    friend class SNDHandle; // SNDHandle needs access to end_sound_handle

private:
    struct DelaySound {
        void reset(void) {
            active = false;
            soundID = -1;
            volumeSet = 1.0f;
            volumeFinal = 1.0f;
            volumeFadeFrames = 0;
            pitch = 1.0f;
            pan = 0.0f;
            tempoRatio = 1.0f;
            startOffset = 0.0f;
            soundHandle = NULL;
            stopFadeFrames = 0;
            pauseFlag = false;
            pauseFadeFrames = 0;
        }

        bool active;

        u16 soundID;

        f32 volumeSet;
        f32 volumeFinal;
        s32 volumeFadeFrames;

        f32 pitch;
        f32 pan;
        f32 tempoRatio;

        f32 startOffset;

        SNDHandle *soundHandle;

        s32 stopFadeFrames;

        bool pauseFlag;
        s32 pauseFadeFrames;
    };

    struct LoadData {
        nw4r::snd::SoundHeap *soundHeap;
        u16 groupID;
        char soundArchivePath[256];
        bool loaded;
    };

    enum ETuneType {
        eTuneType_Volume,
        eTuneType_Pitch,
        eTuneType_Pan,
        eTuneType_TempoRatio,
        eTuneType_Stop,
        eTuneType_Pause
    };
    
    enum ESoundArchiveType {
        eSoundArchiveType_Mem,
        eSoundArchiveType_DVD,
        eSoundArchiveType_Nand,
    };

    enum ESoundType {
        eSoundType_None,
        eSoundType_Direct,
        eSoundType_Delayed
    };

public:
    virtual void _08(void);
    virtual ~CSoundManager(void);
    virtual void _10(void *soundArchiveAddr);
    virtual void _14(const char *soundArchivePath);
    virtual void _18(void);
    virtual void _1C(void);
    
    CSoundManager(void);

    void fn_801E4D60(void);
    void prepare(u16 soundID, SNDHandle *soundHandle = NULL);
    void play(u16 soundID, f32 start = 0.0f, SNDHandle *soundHandle = NULL);
    void fn_801E5640(f32 ticksPerFrame);
    void fn_801E60B4(SNDHandle *soundHandle = NULL);
    void fn_801E60D4(SoundCooldown *soundCooldown, u16 count);
    void fn_801E62B8(s32 fadeFrames = 0, SNDHandle *soundHandle = NULL);
    void fn_801E6440(bool paused, s32 fadeFrames = 0, SNDHandle *soundHandle = NULL);
    void fn_801E65F4(f32 volume, s32 fadeFrames = 0, SNDHandle *soundHandle = NULL);
    void fn_801E676C(f32 pitch, SNDHandle *soundHandle = NULL);
    void fn_801E68E0(f32 pan, SNDHandle *soundHandle = NULL);
    void fn_801E6A54(f32 tempoRatio, SNDHandle *soundHandle = NULL);
    void fn_801E6BC8(f32 tempoRatio, SNDHandle *soundHandle = NULL);
    void fn_801E6E00(s32 playerID);
    void fn_801E6E08(f32 volume);
    void fn_801E6E4C(f32 volume);
    void fn_801E6ECC(f32 volume);
    void fn_801E6F40(bool paused, s32 fadeFrames = 0);
    void fn_801E6F98(bool paused, s32 fadeFrames = 0);
    void fn_801E7008(s32 fadeFrames = 0);
    void fn_801E7088(s32 fadeFrames = 0);
    void fn_801E70FC(void);
    void fn_801E7108(void);
    void fn_801E7114(f32 volume, s32 fadeFrames);
    void fn_801E71C0(void);
    void fn_801E71CC(u16 groupID, nw4r::snd::SoundHeap *soundHeap);
    void fn_801E7230(u16 groupID, nw4r::snd::SoundHeap *soundHeap);
    bool fn_801E7334(void);
    void fn_801E7344(nw4r::snd::SeqUserprocCallback *callback, void *callbackArg);
    void fn_801E73C8(WaveInfo *waveInfo, u16 count);
    WaveInfo *fn_801E73D4(u16 soundID);
    WaveTempo *fn_801E7414(WaveInfo *waveInfo, s32 pos);
    s32 fn_801E7450(WaveInfo *waveInfo, WaveTempo *tempo);
    s32 fn_801E748C(WaveInfo *waveInfo);
    s32 fn_801E74BC(WaveInfo *waveInfo);
    f32 fn_801E74EC(WaveInfo *waveInfo);
    BOOL fn_801E7584(WaveInfo *waveInfo);
    void fn_801E75B4(SeqTempo *seqTempo, u16 count);
    f32 fn_801E75C0(u16 soundID);
    f32 fn_801E7884(u16 soundID);
    void fn_801E7954(void);
    SNDHandle *fn_801E7B30(s32 index);

    static bool fn_801E4D4C(void);
    static void fn_801E4D54(void);

    u16 get_wave_tempo(u16 soundID) {
        WaveInfo *waveInfo = fn_801E73D4(soundID);
        return fn_801E74EC(waveInfo); 
    }

private:
    void fn_801E4988(const char *soundArchivePath);
    void fn_801E5648(void);
    void tune_sound_handle(ETuneType type, f32 value, s32 fadeFrames, SNDHandle *soundHandle = NULL);
    void tune_delay_sound(ETuneType type, f32 value, s32 fadeFrames, DelaySound *delaySound);
    void fn_801E734C(void);

    static void *fn_801E4948(void *);
    static void *fn_801E72E8(void *);

    void end_sound_handle(SNDHandle *soundHandle) {
        if (soundHandle != NULL) {
            for (s32 i = 0; i < (s32)ARRAY_LENGTH(mDelaySound); i++) {
                DelaySound *delaySound = &mDelaySound[i];
                if (delaySound->active && delaySound->soundHandle == soundHandle) {
                    delaySound->reset();
                }
            }
        }
    }

    f32 first_player_volume(void) {
        u32 soundPlayerCount = mSoundArchivePlayer->GetSoundPlayerCount();
        for (u32 i = 0; i < soundPlayerCount; i++) {
            if (static_cast<s32>(i) != mSystemPlayerID) {
                return mSoundArchivePlayer->GetSoundPlayer(i).GetVolume();
            }
        }
        return 0.0f;
    }

    DelaySound *find_delay_sound_by_snd_handle(SNDHandle *soundHandle) {
        if (soundHandle == NULL) {
            return NULL;
        }
        
        for (s32 i = 0; i < (s32)ARRAY_LENGTH(mDelaySound); i++) {
            DelaySound *delaySound = &mDelaySound[i];
            if (delaySound->active && delaySound->soundHandle == soundHandle) {
                return delaySound;
            }
        }
        return NULL;
    }
    
    DelaySound *first_free_delay_sound(void) {
        for (s32 i = 0; i < (s32)ARRAY_LENGTH(mDelaySound); i++) {
            DelaySound *delaySound = &mDelaySound[i];
            if (!delaySound->active) {
                return delaySound;
            }
        }
        return NULL;
    }

    void tune(ETuneType type, f32 value, s32 fadeFrames, SNDHandle *soundHandle = NULL) {
        if (soundHandle != NULL) {
            DelaySound *delaySound = find_delay_sound_by_snd_handle(soundHandle);
            if (delaySound != NULL) {
                tune_delay_sound(type, value, fadeFrames, delaySound);
            }
            else {
                tune_sound_handle(type, value, fadeFrames, soundHandle);
            }
        }
        else {
            switch (mLastSoundType) {
            case eSoundType_Direct:
                tune_sound_handle(type, value, fadeFrames, mLastDirectSound);
                break;
            case eSoundType_Delayed:
                tune_delay_sound(type, value, fadeFrames, mLastDelaySound);
                break;
            case eSoundType_None:
                break;
            }
        }
    }

    SoundCooldown *find_sound_cooldown(u16 soundID) {
        for (s32 i = 0; i < mSoundCooldownCount; i++) {
            SoundCooldown *entry = &mSoundCooldown[i];
            if (entry->soundID == soundID) {
                return entry;
            }
        }
        return NULL;
    }

    void reverb_init(void) {
        u32 fxReverbHi0WorkSize = mFxReverbHi0->GetRequiredMemSize();
        u32 fxReverbHi1WorkSize = mFxReverbHi1->GetRequiredMemSize();

        mFxReverbHi0Work = new u8[fxReverbHi0WorkSize];
        mFxReverbHi1Work = new u8[fxReverbHi1WorkSize];

        mFxReverbHi0->AssignWorkBuffer(mFxReverbHi0Work, fxReverbHi0WorkSize);
        mFxReverbHi1->AssignWorkBuffer(mFxReverbHi1Work, fxReverbHi1WorkSize);

        nw4r::snd::SoundSystem::AppendEffect(nw4r::snd::AUX_B, mFxReverbHi0);
        nw4r::snd::SoundSystem::AppendEffect(nw4r::snd::AUX_C, mFxReverbHi1);
    }

private:
    LoadData mLoadData;

    OSThread mThread;
    u8 mThreadStack[0x2800];

    ESoundArchiveType mSoundArchiveType;

    nw4r::snd::MemorySoundArchive *mMemSoundArchive;
    nw4r::snd::DvdSoundArchive *mDVDSoundArchive;
    nw4r::snd::NandSoundArchive *mNandSoundArchive;

    nw4r::snd::SoundArchivePlayer *mSoundArchivePlayer;
    SNDHandle mDefaultSoundHandle;

    u8 *mMemSoundArchiveData;

    u8 *mSetupBuf;
    u8 *mSetupStrmBuf;

    u8 *mDVDSoundArchiveHeader;

    s32 mSystemPlayerID; // PLAYER_SE_SYSTEM

    s32 mVolumeFadeFrames;
    f32 mVolumeFadeStep;

    bool mNoSoloSystemPlayer;

    nw4r::snd::FxReverbHi *mFxReverbHi0;
    nw4r::snd::FxReverbHi *mFxReverbHi1;
    u8 *mFxReverbHi0Work;
    u8 *mFxReverbHi1Work;
    nw4r::snd::FxReverbHi::ReverbHiParam mFxReverbHi0Param;
    nw4r::snd::FxReverbHi::ReverbHiParam mFxReverbHi1Param;

    WaveInfo *mWaveInfo;
    u16 mWaveInfoCount;

    SeqTempo *mSeqTempo;
    u16 mSeqTempoCount;

    SoundCooldown *mSoundCooldown;
    u16 mSoundCooldownCount;

    DelaySound mDelaySound[32];

    ESoundType mLastSoundType;
    DelaySound *mLastDelaySound;
    SNDHandle *mLastDirectSound;

    f32 mTicksPerFrame;

    SNDHandle mSoundHandle[16];

    u32 mUnk3374; // NOTE: This field is never referenced.
};

extern CSoundManager *gSoundManager;

#endif