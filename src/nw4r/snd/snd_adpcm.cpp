#include <nw4r/snd/adpcm.h>

namespace nw4r {
namespace snd {
namespace detail {

s16 DecodeDspAdpcm(AXPBADPCM *adpcm, u8 bits) {
    s16 yn1 = static_cast<s16>(adpcm->yn1);
	s16 yn2 = static_cast<s16>(adpcm->yn2);

	s16 scale = 1 << (adpcm->pred_scale & 0xF);
	s16 bits2 = bits << 12;
	u16 index = adpcm->pred_scale >> 4;

	s16 coef0 = adpcm->a[index][0];
	s16 coef1 = adpcm->a[index][1];

	s32 sample = coef0 * yn1;
	sample += coef1 * yn2;
	sample += scale * (bits2 >> 1);
	sample >>= 10;
	sample += 1;
	sample >>= 1;

	if (sample > 0x7FFF) {
		sample = 0x7FFF;
    }
	else if (sample < -0x8000) {
		sample = -0x8000;
    }

	adpcm->yn2 = adpcm->yn1;
	adpcm->yn1 = sample;

	return sample;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
