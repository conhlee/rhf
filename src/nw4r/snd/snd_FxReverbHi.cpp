#include <nw4r/snd.h>

#include <nw4r/ut.h>

namespace nw4r {
namespace snd {

FxReverbHi::FxReverbHi() :
    mActive(false)
{
    ReverbHiParam param = ReverbHiParam(); // NOTE: Using default parameters; see ctor
    SetParam(param);
}

u32 FxReverbHi::GetRequiredMemSize() {
    u32 memSize = AXFXReverbHiExpGetMemSize(&mAxfxParam);
    return ut::RoundUp(memSize + detail::AxfxImpl::HEAP_SIZE_MIN, 32);
}

bool FxReverbHi::AssignWorkBuffer(void* pBuffer, u32 size) {
    return mImpl.CreateHeap(pBuffer, size);
}

void FxReverbHi::ReleaseWorkBuffer() {
    mImpl.DestroyHeap();
}

bool FxReverbHi::StartUp() {
    u32 memSize = GetRequiredMemSize(); // TODO: fake temp
    if (memSize > mImpl.GetHeapTotalSize()) {
        return false;
    }

    AXFXAllocHook alloc;
    AXFXFreeHook free;
    mImpl.HookAlloc(&alloc, &free);

    BOOL result = AXFXReverbHiExpInit(&mAxfxParam);

    u32 allocatedSize = mImpl.RestoreAlloc(alloc, free);

    // NOTE: Unused
    u32 requiredMemSize = GetRequiredMemSize();

    mActive = true;

    return result;
}

void FxReverbHi::Shutdown() {
    if (!mActive) {
        return;
    }

    mActive = false;

    AXFXAllocHook allocHook;
    AXFXFreeHook freeHook;
    mImpl.HookAlloc(&allocHook, &freeHook);

    AXFXReverbHiExpShutdown(&mAxfxParam);

    mImpl.RestoreAlloc(allocHook, freeHook);
}

// TODO: match
bool FxReverbHi::SetParam(const ReverbHiParam &rParam) {
    mParam = rParam;

    f32 preDelayTimeMax = ut::Max(rParam.preDelayTimeMax, 0.0f);
    bool needReAlloc = preDelayTimeMax != mAxfxParam.preDelayTimeMax;

    mAxfxParam.earlyMode = static_cast<u32>(rParam.earlyMode);
    mAxfxParam.preDelayTimeMax = preDelayTimeMax;
    mAxfxParam.preDelayTime = ut::Clamp(rParam.preDelayTime, 0.0f, rParam.preDelayTimeMax);
    mAxfxParam.fusedMode = static_cast<u32>(rParam.fusedMode);
    mAxfxParam.fusedTime = ut::Max(rParam.fusedTime, 0.0f);
    mAxfxParam.coloration = ut::Clamp(rParam.coloration, 0.0f, 1.0f);
    mAxfxParam.damping = ut::Clamp(rParam.damping, 0.0f, 1.0f);
    mAxfxParam.crosstalk = ut::Clamp(rParam.crosstalk, 0.0f, 1.0f);
    mAxfxParam.earlyGain = ut::Clamp(rParam.earlyGain, 0.0f, 1.0f);
    mAxfxParam.fusedGain = ut::Clamp(rParam.fusedGain, 0.0f, 1.0f);
    mAxfxParam.outGain = ut::Clamp(rParam.outGain, 0.0f, 1.0f);

    mAxfxParam.busIn = NULL;
    mAxfxParam.busOut = NULL;
    mAxfxParam.sendGain = 0.0f;

    if (!mActive) {
        return true;
    }

    u32 memSize = GetRequiredMemSize(); // TODO: fake temp
    if (memSize > mImpl.GetHeapTotalSize()) {
        return false;
    }

    BOOL result;
    if (needReAlloc) {
        AXFXAllocHook alloc;
        AXFXFreeHook free;
        mImpl.HookAlloc(&alloc, &free);
        result = AXFXReverbHiExpSettings(&mAxfxParam);
        mImpl.RestoreAlloc(alloc, free);
    }
    else {
        result = AXFXReverbHiExpSettingsUpdate(&mAxfxParam);
    }

    return result;
}

void FxReverbHi::UpdateBuffer(
    int channels, void **ppBuffer, u32 size,
    SampleFormat format, f32 sampleRate,
    OutputMode mode
) {
#pragma unused(channels)
#pragma unused(size)
#pragma unused(format)
#pragma unused(sampleRate)

    if (mActive && (mode != OUTPUT_MODE_DPL2)) {
        AXFX_BUFFERUPDATE axfxbuf;
        axfxbuf.left        = (s32 *)ppBuffer[AX_STEREO_L];
        axfxbuf.right       = (s32 *)ppBuffer[AX_STEREO_R];
        axfxbuf.surround    = (s32 *)ppBuffer[AX_STEREO_S];

        AXFXReverbHiExpCallback(&axfxbuf, &mAxfxParam);
    }
}

} // namespace snd
} // namespace nw4r
