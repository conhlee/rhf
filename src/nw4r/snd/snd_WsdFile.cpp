#include <nw4r/snd/WsdFile.h>

#include <nw4r/snd/WaveFile.h>

#include <nw4r/snd/WaveArchive.h>

#include <nw4r/ut/inlines.h>

namespace nw4r {
namespace snd {
namespace detail {

bool WsdFileReader::IsValidFileHeader(void const *wsdData)
{
	ut::BinaryFileHeader const *fileHeader =
		static_cast<ut::BinaryFileHeader const *>(wsdData);

	if (fileHeader->signature != WsdFile::SIGNATURE_FILE) {
		return false;
    }

	if (fileHeader->version < NW4R_FILE_VERSION(1, 0)) {
		return false;
    }

	if (fileHeader->version > SUPPORTED_FILE_VERSION) {
		return false;
    }

	return true;
}

WsdFileReader::WsdFileReader(void const *wsdData) :
	mHeader(NULL),
	mDataBlock(NULL),
	mWaveBlock(NULL)
{
	if (!IsValidFileHeader(wsdData)) {
		return;
    }

	mHeader = static_cast<WsdFile::Header const *>(wsdData);

	if (mHeader->dataBlockOffset != 0) {
		mDataBlock = static_cast<WsdFile::DataBlock const *>(
			ut::AddOffsetToPtr(mHeader, mHeader->dataBlockOffset));
	}

	if (mHeader->waveBlockOffset != 0) {
		mWaveBlock = static_cast<WsdFile::WaveBlock const *>(
			ut::AddOffsetToPtr(mHeader, mHeader->waveBlockOffset));
	}
}

bool WsdFileReader::ReadWaveSoundInfo(WaveSoundInfo *info, int index) const
{
	WsdFile::Wsd const *wsd = Util::GetDataRefAddress0(
		mDataBlock->refWsd[index], &mDataBlock->wsdCount);

	WsdFile::WsdInfo const *src =
		Util::GetDataRefAddress0(wsd->refWsdInfo, &mDataBlock->wsdCount);

	if (mHeader->fileHeader.version >= NW4R_FILE_VERSION(1, 2)) {
		info->pitch			= src->pitch;
		info->pan			= src->pan;
		info->surroundPan	= src->surroundPan;

		info->fxSendA		= src->fxSendA;
		info->fxSendB		= src->fxSendB;
		info->fxSendC		= src->fxSendC;
		info->mainSend		= src->mainSend;
	}
	else if (mHeader->fileHeader.version >= NW4R_FILE_VERSION(1, 1)) {
		info->pitch			= src->pitch;
		info->pan			= src->pan;
		info->surroundPan	= src->surroundPan;

		info->fxSendA		= 0;
		info->fxSendB		= 0;
		info->fxSendC		= 0;
		info->mainSend		= 127;
	}
	else {
		info->pitch			= 1.0f;
		info->pan			= 64;
		info->surroundPan	= 0;

		info->fxSendA		= 0;
		info->fxSendB		= 0;
		info->fxSendC		= 0;
		info->mainSend		= 127;
	}

	return true;
}

bool WsdFileReader::ReadWaveSoundNoteInfo(
    WaveSoundNoteInfo *noteInfo,
    int index, int noteIndex
) const {
	WsdFile::Wsd const *wsd = Util::GetDataRefAddress0(
		mDataBlock->refWsd[index], &mDataBlock->wsdCount);

	WsdFile::NoteInfoTable const *noteTable =
		Util::GetDataRefAddress0(wsd->refNoteTable, &mDataBlock->wsdCount);

	WsdFile::NoteInfo const *src = Util::GetDataRefAddress0(
		noteTable->item[noteIndex], &mDataBlock->wsdCount);

	noteInfo->waveIndex		= src->waveIndex;
	noteInfo->attack		= src->attack;
	noteInfo->hold			= src->hold;
	noteInfo->decay			= src->decay;
	noteInfo->sustain		= src->sustain;
	noteInfo->release		= src->release;
	noteInfo->originalKey	= src->originalKey;
	noteInfo->volume		= src->volume;

	if (mHeader->fileHeader.version >= NW4R_FILE_VERSION(1, 1)) {
		noteInfo->pan			= src->pan;
		noteInfo->surroundPan	= src->surroundPan;
		noteInfo->pitch			= src->pitch;
	}
	else {
		noteInfo->pan			= 64;
		noteInfo->surroundPan	= 0;
		noteInfo->pitch			= 1.0f;
	}

	return true;
}

bool WsdFileReader::ReadWaveInfo(
    int waveIndex, WaveInfo *waveData, void const *waveDataAddress
) const {
	if (mWaveBlock == NULL) {
		WaveArchiveReader waveArchiveReader(waveDataAddress);

		WaveFile::FileHeader const *fileHeader =
			waveArchiveReader.GetWaveFile(waveIndex);
		if (!fileHeader)
			return false;

		WaveFileReader waveFileReader(fileHeader);
		return waveFileReader.ReadWaveInfo(waveData);
	}
	else {
		WaveFile::WaveInfo const *waveInfo;

		if (mHeader->fileHeader.version >= NW4R_FILE_VERSION(1, 1)) {
			if (waveIndex >= mWaveBlock->waveCount) {
				return false;
            }

			waveInfo =
				static_cast<WaveFile::WaveInfo const *>(ut::AddOffsetToPtr(
					mWaveBlock, mWaveBlock->offsetTable[waveIndex]));
		}
		else {
			WsdFile::WaveBlockOld const *waveBlockOld =
				reinterpret_cast<WsdFile::WaveBlockOld const *>(mWaveBlock);

			waveInfo =
				static_cast<WaveFile::WaveInfo const *>(ut::AddOffsetToPtr(
					waveBlockOld, waveBlockOld->offsetTable[waveIndex]));
		}

		WaveFileReader waveFileReader(waveInfo);
		return waveFileReader.ReadWaveInfo(waveData, waveDataAddress);
	}
}

} // namespace detail
} // namespace snd
} // namespace nw4r
