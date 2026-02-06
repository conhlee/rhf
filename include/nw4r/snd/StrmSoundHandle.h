#ifndef NW4R_SND_STRM_SOUND_HANDLE_H
#define NW4R_SND_STRM_SOUND_HANDLE_H

/*******************************************************************************
 * headers
 */

#include <revolution/types.h> // NULL

#include "nw4r/snd/StrmSound.h"

#include "nw4r/ut/inlines.h" // ut::NonCopyable

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace snd { class SoundHandle; }}

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd
{
	class StrmSoundHandle : private ut::NonCopyable
	{
	// methods
	public:
		explicit StrmSoundHandle(SoundHandle *handle);
        ~StrmSoundHandle() {
            DetachSound();
        }

		// methods
		bool IsAttachedSound() const { return mSound != NULL; }

        bool IsPrepared() const {
            return IsAttachedSound() && mSound->IsPrepared();
        }

		bool ReadStrmDataInfo(StrmDataInfo *info) const {
			if (IsAttachedSound()) {
				return mSound->ReadStrmDataInfo(info);
			}
			return false;
		}

		s32 GetPlaySamplePosition() const {
			if (IsAttachedSound()) {
				return mSound->GetPlaySamplePosition();
			}
			return -1;
		}

		void DetachSound();

	// members
	private:
		/* base NonCopyable */			// size 0x00, offset 0x00
		detail::StrmSound	*mSound;	// size 0x04, offset 0x00
	}; // size 0x04
}} // namespace nw4r::snd

#endif // NW4R_SND_STRM_SOUND_HANDLE_H