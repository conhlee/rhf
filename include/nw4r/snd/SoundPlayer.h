#ifndef NW4R_SND_SOUND_PLAYER_H
#define NW4R_SND_SOUND_PLAYER_H

/*******************************************************************************
 * headers
 */

#include <revolution/types.h> // f32

#include <cstddef> // offsetof

#include "nw4r/snd/BasicSound.h"
#include "nw4r/snd/global.h" // AUX_BUS_NUM
#include "nw4r/snd/PlayerHeap.h"

#include <revolution/WPAD.h>

/*******************************************************************************
 * classes
 */

namespace nw4r { namespace snd
{
	class SoundPlayer
	{
	private:
		typedef ut::LinkList<detail::BasicSound, offsetof(detail::BasicSound, mSoundPlayerPlayLink)> SoundPlayerPlayLinkList;
		typedef ut::LinkList<detail::BasicSound, offsetof(detail::BasicSound, mSoundPlayerPriorityLink)> SoundPlayerPriorityLinkList;

	// methods
	public:
		// cdtors
		SoundPlayer();
		~SoundPlayer();

		// methods
		int GetPlayableSoundCount() const { return mPlayableCount; }
		f32 GetVolume() const { return mVolume; }
		f32 GetLpfFreq() const { return mLpfFreq; }
		int GetDefaultOutputLine() const { return mOutputLineFlag; }
		f32 GetMainOutVolume() const { return mMainOutVolume; }
		int GetBiquadFilterType() const { return mBiquadType; }
		f32 GetBiquadFilterValue() const { return mBiquadValue; }
		f32 GetRemoteOutVolume(int remoteIndex) const;
		f32 GetMainSend() const { return mMainSend; }
		f32 GetFxSend(int index) const { return mFxSend[index]; }

		void SetPlayableSoundCount(int count);

		int GetPlayingSoundCount() const { return mSoundList.GetSize(); }
		detail::BasicSound *GetLowestPrioritySound() {
			return &mPriorityList.GetFront();
		}

		void Update();
		void PauseAllSound(bool flag, int fadeFrames);
		void StopAllSound(int fadeFrames);
        void SetVolume(f32 volume);

		void RemoveSoundList(detail::BasicSound *sound);
		bool detail_AppendSound(detail::BasicSound *sound);
		void detail_RemoveSound(detail::BasicSound *sound);
		void detail_SetPlayableSoundLimit(int limit);
		bool detail_CanPlaySound(int startPriority);

		void InsertPriorityList(detail::BasicSound *sound);
		void RemovePriorityList(detail::BasicSound *sound);
		void detail_SortPriorityList(detail::BasicSound *sound);
		void detail_SortPriorityList();

		detail::PlayerHeap *detail_AllocPlayerHeap(detail::BasicSound *sound);
		void detail_FreePlayerHeap(detail::BasicSound *sound);
		void detail_AppendPlayerHeap(detail::PlayerHeap *heap);

	// members
	private:
		SoundPlayerPlayLinkList							mSoundList;				// size 0x0c, offset 0x00
		SoundPlayerPriorityLinkList						mPriorityList;			// size 0x0c, offset 0x0c
		detail::PlayerHeap::LinkList					mHeapList;				// size 0x0c, offset 0x18
		int												mPlayableCount;			// size 0x04, offset 0x24
		int												mPlayableLimit;			// size 0x04, offset 0x28
		f32												mVolume;				// size 0x04, offset 0x2c
		f32												mLpfFreq;				// size 0x04, offset 0x30
		int												mOutputLineFlag;		// size 0x04, offset 0x34
		f32												mMainOutVolume;			// size 0x04, offset 0x38
		int												mBiquadType;			// size 0x04, offset 0x3c
		f32												mBiquadValue;			// size 0x04, offset 0x40
		f32                                             mRemoteOutVolume[WPAD_MAX_CONTROLLERS]; // offset 0x44, size 0x10
		f32												mMainSend;				// size 0x04, offset 0x54
		f32												mFxSend[AUX_BUS_NUM];	// size 0x0c, offset 0x58
	}; // size 0x64
}} // namespace nw4r::snd

#endif // NW4R_SND_SOUND_PLAYER_H
