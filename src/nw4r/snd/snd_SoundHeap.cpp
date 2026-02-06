#include <nw4r/snd.h>

#include <nw4r/ut.h>

namespace nw4r {
namespace snd {

SoundHeap::SoundHeap() {
    OSInitMutex(&mMutex);
}

SoundHeap::~SoundHeap() {
    mFrameHeap.Destroy();
}

bool SoundHeap::Create(void *startAddress, u32 size) {
    return mFrameHeap.Create(startAddress, size);
}

void SoundHeap::Destroy() {
    mFrameHeap.Destroy();
}

void* SoundHeap::Alloc(u32 size) {
    ut::detail::AutoLock<OSMutex> lock (mMutex);
    return mFrameHeap.Alloc(size, DisposeCallbackFunc, NULL);
}

void* SoundHeap::Alloc(u32 size, detail::FrameHeap::FreeCallback callback, void *callbackArg) {
    ut::detail::AutoLock<OSMutex> lock (mMutex);
    return mFrameHeap.Alloc(size, callback, callbackArg);
}

void SoundHeap::Clear() {
    ut::detail::AutoLock<OSMutex> lockHeap (mMutex);
    detail::SoundThread::AutoLock lockThread;

    mFrameHeap.Clear();
}

int SoundHeap::SaveState() {
    ut::detail::AutoLock<OSMutex> lock (mMutex);
    return mFrameHeap.SaveState();
}

void SoundHeap::LoadState(int id) {
    ut::detail::AutoLock<OSMutex> lockHeap (mMutex);
    detail::SoundThread::AutoLock lockThread;

    mFrameHeap.LoadState(id);
}

void SoundHeap::DisposeCallbackFunc(void *mem, u32 size, void *arg) {
    detail::DisposeCallbackManager::GetInstance().Dispose(mem, size, arg);
    detail::DisposeCallbackManager::GetInstance().DisposeWave(mem, size, arg);
}

} // namespace snd
} // namespace nw4r
