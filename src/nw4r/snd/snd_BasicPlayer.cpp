#include <nw4r/snd/BasicPlayer.h>

#include <nw4r/snd/BasicSound.h>

namespace nw4r {
namespace snd {
namespace detail {

void PlayerParamSet::Init() {
	volume			= 1.0f;
	pitch			= 1.0f;
	pan				= 0.0f;
	surroundPan		= 0.0f;
	lpfFreq			= 0.0f;
	biquadType		= 0;
	biquadValue		= 0.0f;
	remoteFilter	= 0;
	outputLineFlag	= 1;
	mainOutVolume	= 1.0f;
	mainSend		= 0.0f;
	panMode			= PAN_MODE_DUAL;
	panCurve		= PAN_CURVE_SQRT;

	for (int i = 0; i < AUX_BUS_NUM; i++) {
		fxSend[i] = 0.0f;
    }

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        remoteOutVolume[i] = 1.0f;
    }

	for (int i = 0; i < (int)ARRAY_LENGTH(voiceOutParam); i++) {
		VoiceOutParam *param = &voiceOutParam[i];

		param->volume		= 1.0f;
		param->pitch		= 1.0f;
		param->pan			= 0.0f;
		param->surroundPan	= 0.0f;
		param->fxSend		= 0.0f;
		param->lpf			= 0.0f;
	}
}

BasicPlayer::BasicPlayer() :
	mId(BasicSound::INVALID_ID)
{
	InitParam();
}

void BasicPlayer::InitParam() {
	mPlayerParamSet.Init();
}

void BasicPlayer::SetFxSend(AuxBus bus, f32 send) {
	mPlayerParamSet.fxSend[bus] = send;
}

f32 BasicPlayer::GetFxSend(AuxBus bus) const {
	return mPlayerParamSet.fxSend[bus];
}

void BasicPlayer::SetBiquadFilter(int type, f32 value) {
	mPlayerParamSet.biquadType	= type;
	mPlayerParamSet.biquadValue	= value;
}

void BasicPlayer::SetRemoteFilter(int filter) {
	mPlayerParamSet.remoteFilter = filter;
}

void BasicPlayer::SetRemoteOutVolume(int remoteIndex, f32 volume) {
    mPlayerParamSet.remoteOutVolume[remoteIndex] = volume;
}

f32 BasicPlayer::GetRemoteOutVolume(int remoteIndex) const {
    return mPlayerParamSet.remoteOutVolume[remoteIndex];
}

} // namespace detail
} // namespace snd
} // namespace nw4r
