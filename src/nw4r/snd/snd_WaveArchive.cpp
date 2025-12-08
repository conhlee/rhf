#include <nw4r/snd/WaveArchive.h>

namespace nw4r {
namespace snd {
namespace detail {

WaveArchiveReader::WaveArchiveReader(void const *waveArc) :
	mTableBlock(NULL),
	mDataBlock(NULL)
{
	if (!VerifyFileHeader(waveArc)) {
		return;
    }

	WaveArchive::FileHeader const *fileHeader =
		static_cast<WaveArchive::FileHeader const *>(waveArc);

	WaveArchive::TableBlock const *tableBlock =
		static_cast<WaveArchive::TableBlock const *>(
			ut::AddOffsetToPtr(fileHeader, fileHeader->tableChunkOffset));

	WaveArchive::DataBlock const *dataBlock =
		static_cast<WaveArchive::DataBlock const *>(
			ut::AddOffsetToPtr(fileHeader, fileHeader->dataChunkOffset));

	mTableBlock	= tableBlock;
	mDataBlock	= dataBlock;
}

WaveFile::FileHeader const *WaveArchiveReader::GetWaveFile(int index) const {
	if (mTableBlock == NULL) {
		return NULL;
    }
	if (mDataBlock == NULL) {
		return NULL;
    }

	if (index < 0) {
		return NULL;
    }
	if (index >= mTableBlock->fileTable.count) {
		return NULL;
    }

	Util::DataRef<WaveFile::FileHeader> waveFileRef =
		mTableBlock->fileTable.item[index].waveFileRef;

	WaveFile::FileHeader const *fileHeader =
		Util::GetDataRefAddress0(waveFileRef, mDataBlock);

	return fileHeader;
}

bool WaveArchiveReader::VerifyFileHeader(void const *waveArc) {
	ut::BinaryFileHeader const *fileHeader =
		static_cast<ut::BinaryFileHeader const *>(waveArc);

	if (fileHeader->signature != WaveArchive::SIGNATURE_FILE) {
		return false;
    }

	if (fileHeader->version < SUPPORTED_FILE_VERSION_MIN) {
		return false;
    }

	if (fileHeader->version > SUPPORTED_FILE_VERSION_MAX) {
		return false;
    }

	return true;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
