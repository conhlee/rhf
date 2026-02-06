#ifndef NW4R_SND_SOUND_HEAP_H
#define NW4R_SND_SOUND_HEAP_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/FrameHeap.h>
#include <nw4r/snd/SoundMemoryAllocatable.h>

#include <nw4r/ut.h>

#include <revolution/OS.h>

namespace nw4r {
namespace snd {

class SoundHeap : public SoundMemoryAllocatable {
public:
    SoundHeap();
    virtual ~SoundHeap(); // at 0x8

    virtual void *Alloc(u32 size); // at 0xC

    void *Alloc(u32 size, detail::FrameHeap::FreeCallback callback, void *callbackArg);

    bool Create(void *startAddress, u32 size);
    void Destroy();

    void Clear();

    int SaveState();
    void LoadState(int id);

    bool IsValid() {
        return mFrameHeap.IsValid();
    }

    int GetCurrentLevel() const {
        ut::detail::AutoLock<OSMutex> lock(mMutex);
        return mFrameHeap.GetCurrentLevel();
    }

    u32 GetFreeSize() const {
        ut::detail::AutoLock<OSMutex> lock(mMutex);
        return mFrameHeap.GetFreeSize();
    }

private:
    static void DisposeCallbackFunc(void *mem, u32 size, void *arg);

private:
    mutable OSMutex mMutex;       // at 0x0
    detail::FrameHeap mFrameHeap; // at 0x1C
};

} // namespace snd
} // namespace nw4r

#endif
