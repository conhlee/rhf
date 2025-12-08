#include <nw4r/snd/WaveSoundHandle.h>

#include <nw4r/snd/SoundHandle.h>

#include <nw4r/snd/BasicSound.h>

#include <nw4r/snd/WaveSound.h>

#include <nw4r/ut/RuntimeTypeInfo.h>

namespace nw4r {
namespace snd {

WaveSoundHandle::WaveSoundHandle(SoundHandle *handle) :
	mSound(NULL)
{
	if (handle == NULL) {
		return;
    }

	detail::BasicSound *basicSound = handle->detail_GetAttachedSound();
	if (basicSound == NULL) {
		return;
    }

	if (detail::WaveSound *sound = ut::DynamicCast<detail::WaveSound *>(basicSound)) {
		mSound = sound;

		if (mSound->IsAttachedTempSpecialHandle()) {
			mSound->DetachTempSpecialHandle();
        }

		mSound->mTempSpecialHandle = this;
	}
}

void WaveSoundHandle::DetachSound() {
	if (IsAttachedSound()) {
		if (mSound->mTempSpecialHandle == this) {
			mSound->mTempSpecialHandle = NULL;
        }
	}

	if (mSound != NULL) {
		mSound = NULL;
    }
}

} // namespace snd
} // namespace nw4r
