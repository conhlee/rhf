#ifndef NW4R_SND_BASIC_PLAYER_H
#define NW4R_SND_BASIC_PLAYER_H

#include <revolution/types.h>

#include <nw4r/snd/global.h>

#include <revolution/WPAD/WPAD.h>

namespace nw4r { namespace snd { namespace detail {
	struct PlayerParamSet {
	public:
		PlayerParamSet() {
			Init();
		}

		void Init();

	public:
		f32				volume;					// size 0x04, offset 0x00
		f32				pitch;					// size 0x04, offset 0x04
		f32				pan;					// size 0x04, offset 0x08
		f32				surroundPan;			// size 0x04, offset 0x0c
		f32				lpfFreq;				// size 0x04, offset 0x10
		f32				biquadValue;			// size 0x04, offset 0x14
		u8				biquadType;				// size 0x01, offset 0x18
		u8				remoteFilter;			// size 0x01, offset 0x19
		/* 2 bytes padding */
		int				outputLineFlag;			// size 0x04, offset 0x1c
		f32				mainOutVolume;			// size 0x04, offset 0x20
		f32				mainSend;				// size 0x04, offset 0x24
		PanMode			panMode;				// size 0x04, offset 0x28
		PanCurve		panCurve;				// size 0x04, offset 0x2c
		f32				fxSend[AUX_BUS_NUM];	// size 0x0c, offset 0x30
		f32 			remoteOutVolume[WPAD_MAX_CONTROLLERS]; // size 0x10, offset 0x3c
		VoiceOutParam	voiceOutParam[4];		// size 0x60, offset 0x4c
	}; // size 0xac
}}} // namespace nw4r::snd::detail

namespace nw4r { namespace snd { namespace detail {
	class BasicPlayer {
	public:
		BasicPlayer();
		virtual ~BasicPlayer() {}

		virtual bool Start() = 0;
		virtual void Stop() = 0;
		virtual void Pause(bool flag) = 0;
		virtual bool IsActive() const = 0;
		virtual bool IsStarted() const = 0;
		virtual bool IsPause() const = 0;

		void InitParam();

		f32 GetVolume() const { return mPlayerParamSet.volume; }
		f32 GetPitch() const { return mPlayerParamSet.pitch; }
		f32 GetPan() const { return mPlayerParamSet.pan; }
		f32 GetSurroundPan() const { return mPlayerParamSet.surroundPan; }
		f32 GetLpfFreq() const { return mPlayerParamSet.lpfFreq; }
		f32 GetBiquadValue() const { return mPlayerParamSet.biquadValue; }
		int GetBiquadType() const { return mPlayerParamSet.biquadType; }
		int GetRemoteFilter() const { return mPlayerParamSet.remoteFilter; }
		f32 GetRemoteOutVolume(int remoteIndex) const;
		int GetOutputLine() const { return mPlayerParamSet.outputLineFlag; }
		f32 GetMainOutVolume() const { return mPlayerParamSet.mainOutVolume; }
		f32 GetMainSend() const { return mPlayerParamSet.mainSend; }
		PanMode GetPanMode() const { return mPlayerParamSet.panMode; }
		PanCurve GetPanCurve() const { return mPlayerParamSet.panCurve; }
		f32 GetFxSend(AuxBus bus) const;
		VoiceOutParam const &GetVoiceOutParam(int index) const {
			return mPlayerParamSet.voiceOutParam[index];
		}
		u32 GetId() const { return mId; }

		void SetVolume(f32 volume) { mPlayerParamSet.volume = volume; }
		void SetPitch(f32 pitch) { mPlayerParamSet.pitch = pitch; }
		void SetPan(f32 pan) { mPlayerParamSet.pan = pan; }
		void SetSurroundPan(f32 surroundPan) { mPlayerParamSet.surroundPan = surroundPan; }
		void SetLpfFreq(f32 lpfFreq) { mPlayerParamSet.lpfFreq = lpfFreq; }
		void SetBiquadFilter(int type, f32 value);
		void SetRemoteFilter(int filter);
		void SetRemoteOutVolume(int remoteIndex, f32 volume);
		void SetOutputLine(int lineFlag) { mPlayerParamSet.outputLineFlag = lineFlag; }
		void SetMainOutVolume(f32 volume) { mPlayerParamSet.mainOutVolume = volume; }
		void SetMainSend(f32 send) { mPlayerParamSet.mainSend = send; }
		void SetPanMode(PanMode panMode) { mPlayerParamSet.panMode = panMode; }
		void SetPanCurve(PanCurve panCurve) { mPlayerParamSet.panCurve = panCurve; }
		void SetFxSend(AuxBus bus, f32 send);
		void SetVoiceOutParam(int index, VoiceOutParam const &param) {
			mPlayerParamSet.voiceOutParam[index] = param;
		}
		void SetId(u32 id) { mId = id; }

	private:
		/* vtable */						// size 0x04, offset 0x00
		PlayerParamSet	mPlayerParamSet;	// size 0x9c, offset 0x04
		u32				mId;				// size 0x04, offset 0xa0
	}; // size 0xa4
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_BASIC_PLAYER_H
