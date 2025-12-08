#include <nw4r/snd/WaveSound.h>

#include <nw4r/snd/SoundInstanceManager.h>

#include <nw4r/snd/WaveSoundHandle.h>

#include <nw4r/ut/RuntimeTypeInfo.h>

namespace nw4r {
namespace snd {
namespace detail {

ut::detail::RuntimeTypeInfo const WaveSound::typeInfo(&BasicSound::typeInfo);

} // namespace detail
} // namespace snd
} // namespace nw4r

namespace nw4r {
namespace snd {
namespace detail {

WaveSound::WaveSound(SoundInstanceManager<WaveSound> *manager, int priority,
                     int ambientPriority) :
	BasicSound			(priority, ambientPriority),
	mTempSpecialHandle	(NULL),
	mManager			(manager),
	mPreparedFlag		(false)
{
}

bool WaveSound::Prepare(
    void const *waveSoundBase, s32 waveSoundOffset,
    WsdPlayer::StartOffsetType startOffsetType, s32 offset,
    WsdPlayer::WsdCallback const *callback,
    u32 callbackData
) {
	InitParam();

	bool result =
		mWsdPlayer.Prepare(waveSoundBase, waveSoundOffset, startOffsetType,
	                       offset, GetVoiceOutCount(), callback, callbackData);
	if (!result)
		return false;

	mPreparedFlag = true;
	return true;
}

void WaveSound::Shutdown() {
	BasicSound::Shutdown();

	mManager->Free(this);
}

void WaveSound::SetChannelPriority(int priority)
{
	mWsdPlayer.SetChannelPriority(priority);
}

void WaveSound::SetReleasePriorityFix(bool flag)
{
	mWsdPlayer.SetReleasePriorityFix(flag);
}

void WaveSound::OnUpdatePlayerPriority()
{
	mManager->UpdatePriority(this, CalcCurrentPlayerPriority());
}

bool WaveSound::IsAttachedTempSpecialHandle()
{
	return mTempSpecialHandle != NULL;
}

void WaveSound::DetachTempSpecialHandle()
{
	mTempSpecialHandle->DetachSound();
}

} // namespace detail
} // namespace snd
} // namespace nw4r

