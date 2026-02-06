#ifndef NW4R_SND_FX_REVERB_HI_H
#define NW4R_SND_FX_REVERB_HI_H
#include <nw4r/types_nw4r.h>

#include "nw4r/snd/AxfxImpl.h"
#include "nw4r/snd/FxBase.h"

#include <revolution/AXFX.h>

namespace nw4r {
namespace snd {

enum FxReverbEarlyReflectionMode {
    FX_REVERB_EARLY_REFLECTION_5MS = 0,
    FX_REVERB_EARLY_REFLECTION_10MS = 1,
    FX_REVERB_EARLY_REFLECTION_15MS = 2,
    FX_REVERB_EARLY_REFLECTION_20MS = 3,
    FX_REVERB_EARLY_REFLECTION_25MS = 4,
    FX_REVERB_EARLY_REFLECTION_30MS = 5,
    FX_REVERB_EARLY_REFLECTION_35MS = 6,
    FX_REVERB_EARLY_REFLECTION_40MS = 7,
};

enum FxReverbFusedMode {
    FX_REVERB_FUSED_OLD_AXFX = 0,
    FX_REVERB_FUSED_METAL_TANK = 1,
    FX_REVERB_FUSED_SMALL_ROOM = 2,
    FX_REVERB_FUSED_LARGE_ROOM = 3,
    FX_REVERB_FUSED_HALL = 4,
    FX_REVERB_FUSED_CAVERNOUS = 5,
};

namespace detail {

struct FxReverbHiParam {
    FxReverbHiParam() :
        preDelayTime(0.02f),
        fusedTime(3.0f),
        coloration(0.6f),
        damping(0.4f),
        crosstalk(0.1f),
        outGain(1.0f),
        earlyMode(FX_REVERB_EARLY_REFLECTION_30MS),
        preDelayTimeMax(0.02),
        fusedMode(FX_REVERB_FUSED_OLD_AXFX),
        earlyGain(0.0f),
        fusedGain(1.0f)
    {}

    // total size: 0x2C
    f32 preDelayTime; // offset 0x0, size 0x4
    f32 fusedTime; // offset 0x4, size 0x4
    f32 coloration; // offset 0x8, size 0x4
    f32 damping; // offset 0xC, size 0x4
    f32 crosstalk; // offset 0x10, size 0x4
    f32 outGain; // offset 0x14, size 0x4
    FxReverbEarlyReflectionMode earlyMode; // offset 0x18, size 0x4
    f32 preDelayTimeMax; // offset 0x1C, size 0x4
    FxReverbFusedMode fusedMode; // offset 0x20, size 0x4
    f32 earlyGain; // offset 0x24, size 0x4
    f32 fusedGain; // offset 0x28, size 0x4
};

} // namespace detail

class FxReverbHi : public FxBase {
public:
    typedef detail::FxReverbHiParam ReverbHiParam;

public:
    FxReverbHi();

    virtual ~FxReverbHi() {
        Shutdown();
        ReleaseWorkBuffer();
    } // at 0x8

    virtual bool StartUp();  // at 0xC
    virtual void Shutdown(); // at 0x10

    virtual void UpdateBuffer(int channels, void** ppBuffer, u32 size,
                              SampleFormat format, f32 sampleRate,
                              OutputMode mode); // at 0x14

    virtual bool AssignWorkBuffer(void* pBuffer, u32 size); // at 0x18
    virtual void ReleaseWorkBuffer();                       // at 0x1C

    u32 GetRequiredMemSize();
    bool SetParam(const ReverbHiParam& rParam) DONT_INLINE;

private:
    bool mActive;
    detail::AxfxImpl mImpl;   
    ReverbHiParam mParam;
    AXFX_REVERBHI_EXP mAxfxParam;
};

} // namespace snd
} // namespace nw4r

#endif
