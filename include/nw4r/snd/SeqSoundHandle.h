#ifndef NW4R_SND_SEQ_SOUND_HANDLE_H
#define NW4R_SND_SEQ_SOUND_HANDLE_H

#include <types.h>

#include "nw4r/snd/SeqSound.h"

#include "nw4r/ut.h"

namespace nw4r {
namespace snd {

// Forward declarations
class SoundHandle;

class SeqSoundHandle : private ut::NonCopyable {
public:
    explicit SeqSoundHandle(SoundHandle* pHandle);
    ~SeqSoundHandle() {
        DetachSound();
    }

    bool IsAttachedSound() const {
        return mSound != NULL;
    }

    void DetachSound();

    void SetTempoRatio(f32 tempo) {
        if (IsAttachedSound()) {
            mSound->SetTempoRatio(tempo);
        }
    }

    void SetTrackVolume(u32 trackBitFlag, f32 volume) {
        if (IsAttachedSound()) {
            mSound->SetTrackVolume(trackBitFlag, volume);
        }
    }

    void SetTrackMute(u32 trackBitFlag, bool mute) {
        if (IsAttachedSound()) {
            mSound->SetTrackMute(trackBitFlag, mute);
        }
    }

    u32 GetTick() const {
        if (IsAttachedSound()) {
            return mSound->GetTick();
        }
        return 0;
    }

    void WriteVariable(int idx, s16 value) {
        if (IsAttachedSound()) {
            mSound->WriteVariable(idx, value);
        }
    }

private:
    detail::SeqSound* mSound; // at 0x0
};

} // namespace snd
} // namespace nw4r

#endif
