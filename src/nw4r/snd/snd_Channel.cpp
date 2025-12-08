#include <nw4r/snd/Channel.h>

#include <nw4r/snd/global.h>

#include <nw4r/snd/WaveFile.h>

#include <nw4r/snd/VoiceManager.h>

#include <nw4r/snd/Util.h>

#include <nw4r/ut/lock.h>

namespace nw4r {
namespace snd {
namespace detail {

ChannelManager &ChannelManager::GetInstance() {
	static ChannelManager instance;
	return instance;
}

ChannelManager::ChannelManager() :
	mInitialized(false),
	mChannelCount(0)
{}

u32 ChannelManager::GetRequiredMemSize(int channelCount) {
	return sizeof(Channel) * (1 + channelCount);
}

void ChannelManager::Setup(void *mem, u32 memSize) {
	ut::AutoInterruptLock lock;

	if (mInitialized) {
		return;
    }

	mChannelCount	= mPool.Create(mem, memSize);
	mMem			= mem;
	mMemSize		= memSize;
	mInitialized	= true;
}

void ChannelManager::Shutdown() {
	ut::AutoInterruptLock lock;

	if (!mInitialized) {
		return;
    }

    for (ChannelLinkList::Iterator itr = mChannelList.GetBeginIter(); itr != mChannelList.GetEndIter();) {
		ChannelLinkList::Iterator curItr = itr++;

		curItr->Stop();
	}

	mPool.Destroy(mMem, mMemSize);
	mInitialized = false;
}

Channel *ChannelManager::Alloc() {
	Channel *channel = mPool.Alloc();

	mChannelList.PushBack(channel);
	return channel;
}

void ChannelManager::Free(Channel *channel) {
	mChannelList.Erase(channel);
	mPool.Free(channel);
}

void ChannelManager::UpdateAllChannel() {
    for (ChannelLinkList::Iterator itr = mChannelList.GetBeginIter(); itr != mChannelList.GetEndIter();) {
		ChannelLinkList::Iterator curItr = itr++;
		curItr->Update(true);
	}
}

Channel::Channel() :
	mPauseFlag(false),
	mActiveFlag(false),
	mAllocFlag(false),
	mVoice(NULL)
{}
Channel::~Channel() {}

void Channel::InitParam(Callback *callback, u32 callbackData) {
	mNextLink					= NULL;
	mCallback					= callback;
	mCallbackData				= callbackData;
	mWaveDataLocationCallback	= NULL;
	mWaveInfo					= NULL;
	mPauseFlag					= false;
	mAutoSweep					= true;
	mReleasePriorityFixFlag		= false;
	mReleaseIgnoreFlag			= false;
	mLength						= 0;
	mKey						= KEY_INIT;
	mOriginalKey				= ORIGINAL_KEY_INIT;
	mInitVolume					= 1.0f;
	mInitPan					= 0.0f;
	mInitSurroundPan			= 0.0f;
	mTune						= 1.0f;
	mUserVolume					= 1.0f;
	mUserPitch					= 0.0f;
	mUserPitchRatio				= 1.0f;
	mUserPan					= 0.0f;
	mUserSurroundPan			= 0.0f;
	mUserLpfFreq				= 0.0f;
	mBiquadType					= 0;
	mBiquadValue				= 0.0f;
	mRemoteFilter				= 0;
	mOutputLineFlag				= OUTPUT_LINE_MAIN;
	mMainOutVolume				= 1.0f;
	mMainSend					= 0.0f;

	for (int i = 0; i < AUX_BUS_NUM; i++) {
		mFxSend[i] = 0.0f;
    }

	for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
		mRemoteOutVolume[i] = 1.0f;
    }

	mSilenceVolume.InitValue(SILENCE_VOLUME_MAX);

	mSweepPitch					= 0.0f;
	mSweepLength				= 0;
	mSweepCounter				= 0;

	mEnvelope.Init(EnvGenerator::VOLUME_INIT);
	mLfo.GetParam().Init();

	mLfoTarget					= LFO_TARGET_PITCH;
	mPanMode					= PAN_MODE_DUAL;
	mPanCurve					= PAN_CURVE_SQRT;
	mAlternateAssign			= 0;
}

void Channel::Update(bool doPeriodicProc) {
	if (!mActiveFlag) {
		return;
    }

	if (mPauseFlag) {
		doPeriodicProc = false;
    }

	f32 lfoValue = mLfo.GetValue();

	mSilenceVolume.Update();

	f32 volume = 1.0f;
	volume *= mInitVolume;
	volume *= mUserVolume;
	volume *=
		mSilenceVolume.GetValue() / static_cast<float>(SILENCE_VOLUME_MAX);

	f32 veInitVolume = 1.0f;
	veInitVolume *= Util::CalcVolumeRatio(mEnvelope.GetValue());

	if (mLfoTarget == LFO_TARGET_VOLUME) {
		veInitVolume *= Util::CalcVolumeRatio(lfoValue * 6.0f);
    }

	if (mEnvelope.GetStatus() == EnvGenerator::STATUS_RELEASE) {
		if (mCallback != NULL) {
			if (veInitVolume == 0.0f) {
				Stop();
				return;
			}
		}
		else {
			if ((volume * veInitVolume) == 0.0f) {
				Stop();
				return;
			}
		}
	}

	f32 cent = 0.0f;
	cent += mKey - mOriginalKey;
	cent += GetSweepValue();
	cent += mUserPitch;
	if (mLfoTarget == LFO_TARGET_PITCH) {
		cent += lfoValue;
    }

	f32 pitchRatio = 1.0f;
	pitchRatio *= mTune;
	pitchRatio *= mUserPitchRatio;

	f32 pitch = Util::CalcPitchRatio(cent * 256.0f);
	pitch *= pitchRatio;

	f32 pan = 0.0f;
	pan += mInitPan;
	pan += mUserPan;
	if (mLfoTarget == LFO_TARGET_PAN)
		pan += lfoValue;

	f32 surroundPan = 0.0f;
	surroundPan += mInitSurroundPan;
	surroundPan += mUserSurroundPan;

	f32 lpfFreq = 1.0f;
	lpfFreq += mUserLpfFreq;

	int remoteFilter = 0;
	remoteFilter += mRemoteFilter;

	f32 mainOutVolume = 1.0f;
	mainOutVolume *= mMainOutVolume;

	f32 mainSend = 0.0f;
	mainSend += mMainSend;

	f32 fxSend[AUX_BUS_NUM];
	for (int i = 0; i < AUX_BUS_NUM; i++)
	{
		fxSend[i] = 0.0f;
		fxSend[i] += mFxSend[i];
	}

	f32 remoteOutVolume[WPAD_MAX_CONTROLLERS];
	for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
		remoteOutVolume[i] = 1.0f;
		remoteOutVolume[i] *= mRemoteOutVolume[i];
	}

	if (doPeriodicProc) {
		if (mAutoSweep) {
			UpdateSweep(3);
        }

		mLfo.Update(3);
		mEnvelope.Update(3);
	}

	f32 nextLfoValue = mLfo.GetValue();

	f32 veTargetVolume = 1.0f;
	veTargetVolume *= Util::CalcVolumeRatio(mEnvelope.GetValue());

	if (mLfoTarget == LFO_TARGET_VOLUME) {
		veTargetVolume *= Util::CalcVolumeRatio(nextLfoValue * 6.0f);
    }

	if (mVoice) {
		mVoice->SetPanMode(mPanMode);
		mVoice->SetPanCurve(mPanCurve);
		mVoice->SetVolume(volume);
		mVoice->SetVeVolume(veTargetVolume, veInitVolume);
		mVoice->SetPitch(pitch);
		mVoice->SetPan(pan);
		mVoice->SetSurroundPan(surroundPan);
		mVoice->SetLpfFreq(lpfFreq);
		mVoice->SetBiquadFilter(mBiquadType, mBiquadValue);
		mVoice->SetRemoteFilter(remoteFilter);
		mVoice->SetOutputLine(mOutputLineFlag);
		mVoice->SetMainOutVolume(mainOutVolume);
		mVoice->SetMainSend(mainSend);

		for (int i = 0; i < AUX_BUS_NUM; i++) {
			mVoice->SetFxSend(static_cast<AuxBus>(i), fxSend[i]);
        }
		
		for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
			mVoice->SetRemoteOutVolume(i, remoteOutVolume[i]);
		}
	}
}

void Channel::Start(WaveInfo const &waveParam, int length, u32 startOffset) {
	mLength = length;

	mLfo.Reset();
	mEnvelope.Reset(EnvGenerator::VOLUME_INIT);
	mSweepCounter = 0;

	mVoice->Setup(waveParam, startOffset);
	mVoice->Start();
	mActiveFlag = true;
}

void Channel::Release() {
	if (mEnvelope.GetStatus() != EnvGenerator::STATUS_RELEASE) {
		if ((mVoice != NULL) && !mReleasePriorityFixFlag) {
			mVoice->SetPriority(PRIORITY_RELEASE);
        }

		mEnvelope.SetStatus(EnvGenerator::STATUS_RELEASE);
	}

	mPauseFlag = false;
}

void Channel::NoteOff() {
	if (!mReleaseIgnoreFlag) {
		Release();
    }
}

void Channel::Stop() {
	if (mVoice == NULL) {
		return;
    }

	mVoice->Stop();
	mVoice->Free();

	mVoice = NULL;

	mPauseFlag = false;
	mActiveFlag = false;

	if (mCallback != NULL) {
		(*mCallback)(this, CALLBACK_STATUS_STOPPED, mCallbackData);
    }

	if (mWaveDataLocationCallback != NULL) {
		mWaveDataLocationCallback->at_0x0c(mWaveInfo);
    }

	if (mAllocFlag) {
		mAllocFlag = false;
		ChannelManager::GetInstance().Free(this);
	}
}

void Channel::UpdateSweep(int count) {
	mSweepCounter += count;

	if (mSweepCounter > mSweepLength) {
		mSweepCounter = mSweepLength;
    }
}

void Channel::SetSweepParam(f32 sweepPitch, int sweepTime, bool autoUpdate) {
	mSweepPitch		= sweepPitch;
	mSweepLength	= sweepTime;
	mAutoSweep		= autoUpdate;
	mSweepCounter	= 0;
}

f32 Channel::GetSweepValue() const {
	if (mSweepPitch == 0.0f)
		return 0.0f;
	if (mSweepCounter >= mSweepLength)
		return 0.0f;

	f32 sweep = mSweepPitch * (mSweepLength - mSweepCounter);
	sweep /= mSweepLength;

	return sweep;
}

void Channel::SetBiquadFilter(int type, f32 value) {
	mBiquadType		= type;
	mBiquadValue	= value;
}

void Channel::VoiceCallbackFunc(
    Voice *voice, Voice::VoiceCallbackStatus status,
    void *arg
) {
	ChannelCallbackStatus chStatus;
	switch (status)
	{
	case Voice::CALLBACK_STATUS_FINISH_WAVE:
		chStatus = CALLBACK_STATUS_FINISH;
		voice->Free();
		break;

	case Voice::CALLBACK_STATUS_CANCEL:
		chStatus = CALLBACK_STATUS_CANCEL;
		voice->Free();
		break;

	case Voice::CALLBACK_STATUS_DROP_VOICE:
		chStatus = CALLBACK_STATUS_DROP;
		break;

	case Voice::CALLBACK_STATUS_DROP_DSP:
		chStatus = CALLBACK_STATUS_DROP;
		break;
	}

	Channel *channel = static_cast<Channel *>(arg);

	if (channel->mCallback != NULL) {
		(*channel->mCallback)(channel, chStatus, channel->mCallbackData);
    }

	if (channel->mWaveDataLocationCallback != NULL) {
		channel->mWaveDataLocationCallback->at_0x0c(channel->mWaveInfo);
    }

	channel->mVoice = NULL;
	channel->mPauseFlag = false;
	channel->mActiveFlag = false;
	channel->mAllocFlag = false;

	ChannelManager::GetInstance().Free(channel);
}

Channel *Channel::AllocChannel(
    int voiceChannelCount, int voiceOutCount,
    int priority,
    Callback *callback, u32 callbackData
) {
	Channel *channel = ChannelManager::GetInstance().Alloc();
	if (channel == NULL) {
		return NULL;
	}

	channel->mAllocFlag = true;

	Voice *voice = VoiceManager::GetInstance().AllocVoice(
		voiceChannelCount, voiceOutCount, priority, VoiceCallbackFunc, channel);
	if (voice == NULL) {
		ChannelManager::GetInstance().Free(channel);
		return NULL;
	}

	channel->mVoice = voice;
	channel->InitParam(callback, callbackData);

	return channel;
}

void Channel::FreeChannel(Channel *channel) {
	if (channel != NULL) {
		channel->mCallback = NULL;
		channel->mCallbackData = 0;
	}
}

} // namespace detail
} // namespace snd
} // namespace nw4r

