#ifndef NW4R_SND_AXFX_IMPL_H
#define NW4R_SND_AXFX_IMPL_H
#include <nw4r/types_nw4r.h>

#include <revolution/AXFX.h>
#include <revolution/MEM.h>

namespace nw4r {
namespace snd {
namespace detail {

struct AxfxImpl {
    MEMiHeapHead* mHandle; // at 0x0
    u32 mAllocCounter;     // at 0x4

    static const u32 HEAP_SIZE_MIN = MEM_FRM_HEAP_MIN_SIZE + 32;

    AxfxImpl() : mHandle(NULL), mAllocCounter(0) {}

    bool CreateHeap(void* pBuffer, u32 size);
    void DestroyHeap();

    u32 GetHeapTotalSize() {
        if (mHandle == NULL) {
            return 0;
        }

        return MEMGetHeapTotalSize(mHandle);
    }

    void HookAlloc(AXFXAllocHook* pAllocHook, AXFXFreeHook* pFreeHook);
    u32 RestoreAlloc(AXFXAllocHook allocHook, AXFXFreeHook freeHook);

    static void* Alloc(u32 size);
    static void Free(void* pBlock);

    static AxfxImpl* mCurrentFx;
    static u32 mAllocatedSize;
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
