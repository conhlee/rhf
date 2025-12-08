#include <nw4r/snd/WaveFile.h>

#include <revolution/macros.h>

namespace nw4r {
namespace snd {
namespace detail {

WaveFileReader::WaveFileReader(WaveFile::FileHeader const *waveFileHeader) :
	mWaveInfo(NULL)
{
	WaveFile::InfoBlock const *infoBlock =
		static_cast<WaveFile::InfoBlock const *>(ut::AddOffsetToPtr(
			waveFileHeader, waveFileHeader->infoChunkOffset
        ));

	if (infoBlock == NULL) {
		return;
    }

	mWaveInfo = &infoBlock->waveInfo;
}

WaveFileReader::WaveFileReader(WaveFile::WaveInfo const *waveInfo) :
	mWaveInfo(waveInfo)
{}

bool WaveFileReader::ReadWaveInfo(
    WaveInfo *waveData, void const *waveDataOffsetOrigin
) const {
	SampleFormat sampleFormat =
		GetSampleFormatFromWaveFileFormat(mWaveInfo->format);

	waveData->sampleFormat = sampleFormat;
	waveData->numChannels = mWaveInfo->numChannels;
	waveData->sampleRate = (mWaveInfo->sampleRate24 << 16) + mWaveInfo->sampleRate;
	waveData->loopFlag = mWaveInfo->loopFlag;
	waveData->loopStart = AxVoice::GetSampleByDspAddress(
		NULL, mWaveInfo->loopStart, sampleFormat
    );
	waveData->loopEnd = AxVoice::GetSampleByDspAddress(
		NULL, mWaveInfo->loopEnd, sampleFormat
    ) + 1;

	u32 const *channelInfoOffset = static_cast<u32 const *>(
		ut::AddOffsetToPtr(mWaveInfo, mWaveInfo->channelInfoTableOffset)
    );

	for (int channelIndex = 0; channelIndex < mWaveInfo->numChannels; channelIndex++) {
		if (channelIndex >= Channel::CHANNEL_MAX) {
			continue;
        }

		ChannelParam &channelParam = waveData->channelParam[channelIndex];

		WaveFile::WaveChannelInfo const *waveChannelInfo =
			static_cast<WaveFile::WaveChannelInfo const *>(ut::AddOffsetToPtr(
                mWaveInfo, channelInfoOffset[channelIndex]
            ));

		if (waveChannelInfo->adpcmOffset != 0) {
			WaveFile::AdpcmParamSet const *adpcmParamSet =
				static_cast<WaveFile::AdpcmParamSet const *>(ut::AddOffsetToPtr(
                    mWaveInfo, waveChannelInfo->adpcmOffset
                ));

			channelParam.adpcmParam		= adpcmParamSet->adpcmParam;
			channelParam.adpcmLoopParam	= adpcmParamSet->adpcmLoopParam;
		}

		channelParam.dataAddr = const_cast<void *>(
			GetWaveDataAddress(waveChannelInfo, waveDataOffsetOrigin));
	}

	return true;
}

DONT_INLINE void const *WaveFileReader::GetWaveDataAddress(
	WaveFile::WaveChannelInfo const *waveChannelInfo,
	void const *waveDataOffsetOrigin
) const {
    // NOTE: Needed to stop this func from being inlined!
    (void)(0); (void)(0);

	void const *waveDataAddress = NULL;

	bool offsetIsDataBlock = waveDataOffsetOrigin == NULL;

	if (waveDataOffsetOrigin == NULL) {
		waveDataOffsetOrigin = mWaveInfo;
    }

	// TODO: InstInfo::WaveDataLocation::WaveDataLocationType?
	switch (mWaveInfo->dataLocationType) {
	case 0:
		waveDataAddress =
			ut::AddOffsetToPtr(waveDataOffsetOrigin, mWaveInfo->dataLocation);

		// TODO: sizeof(InstInfo::WaveDataLocation)?
		if (offsetIsDataBlock) {
			waveDataAddress = ut::AddOffsetToPtr(waveDataAddress, 8);
        }

		break;

	case 1:
		waveDataAddress =
			reinterpret_cast<void const *>(mWaveInfo->dataLocation);
		break;

	default:
		return NULL;
	}

	waveDataAddress = ut::AddOffsetToPtr(
		waveDataAddress, waveChannelInfo->channelDataOffset
    );

	return waveDataAddress;
}

SampleFormat WaveFileReader::GetSampleFormatFromWaveFileFormat(u8 format) {
	switch (format) {
	case 2:
		return SAMPLE_FORMAT_DSP_ADPCM;

	case 1:
		return SAMPLE_FORMAT_PCM_S16;

	case 0:
		return SAMPLE_FORMAT_PCM_S8;

	default:
		return SAMPLE_FORMAT_DSP_ADPCM;
	}
}

} // namespace detail
} // namespace snd
} // namespace nw4r
