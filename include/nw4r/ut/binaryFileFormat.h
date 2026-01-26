#ifndef NW4R_UT_BINARY_FILE_HEADER_H
#define NW4R_UT_BINARY_FILE_HEADER_H

/*******************************************************************************
 * headers
 */

#include <types.h>

#include <climits> // CHAR_BIT

/*******************************************************************************
 * types
 */

#define NW4R_BYTE_(byte_, offset_)	\
	(static_cast<unsigned char>(byte_) << CHAR_BIT * (offset_))

#define NW4R_FILE_VERSION(MAJOR, MINOR)	\
	(NW4R_BYTE_(MAJOR, 1) | NW4R_BYTE_(MINOR, 0))

#define NW4R_FOUR_BYTE(A, B, C, D)	\
	(NW4R_BYTE_(A, 3) | NW4R_BYTE_(B, 2) | NW4R_BYTE_(C, 1) | NW4R_BYTE_(D, 0))

namespace nw4r { namespace ut
{
	struct BinaryFileHeader
	{
		u32	    signature;	// size 0x04, offset 0x00
		u16		byteOrder;	// size 0x02, offset 0x04
		u16		version;	// size 0x02, offset 0x06
		u32		fileSize;	// size 0x04, offset 0x08
		u16		headerSize;	// size 0x02, offset 0x0c
		u16		dataBlocks;	// size 0x02, offset 0x0e
	}; // size 0x10

	struct BinaryBlockHeader
	{
		u32	    kind;	// size 0x04, offset 0x00
		u32		size;	// size 0x04, offset 0x04
	}; // size 0x08
}} // namespace nw4r::ut

#endif // NW4R_UT_BINARY_FILE_HEADER_H
