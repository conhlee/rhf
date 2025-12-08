#include <nw4r/snd/DisposeCallbackManager.h>

#include <nw4r/snd/SoundThread.h>

#include <nw4r/ut/lock.h>

namespace nw4r {
namespace snd {
namespace detail {

DisposeCallbackManager &DisposeCallbackManager::GetInstance() {
	ut::AutoInterruptLock lock; // What

	static DisposeCallbackManager instance;
	return instance;
}

DisposeCallbackManager::DisposeCallbackManager() {}

void DisposeCallbackManager::RegisterDisposeCallback(DisposeCallback *callback) {
	mCallbackList.PushBack(callback);
}

void DisposeCallbackManager::UnregisterDisposeCallback(
	DisposeCallback *callback
) {
	mCallbackList.Erase(callback);
}

void DisposeCallbackManager::Dispose(void *mem, u32 size, void *arg ATTR_UNUSED) {
	void *start = mem;
	void *end = static_cast<u8 *>(mem) + size;

	SoundThread::AutoLock lock;

	for (
        DisposeCallbackLinkList::Iterator itr = GetInstance().mCallbackList.GetBeginIter();
        itr != GetInstance().mCallbackList.GetEndIter();
    ) {
		DisposeCallbackLinkList::Iterator curItr = itr++;

		curItr->InvalidateData(start, end);
	}
}

void DisposeCallbackManager::DisposeWave(void *mem, u32 size, void *arg ATTR_UNUSED) {
	void *start = mem;
	void *end = static_cast<u8 *>(mem) + size;

	SoundThread::AutoLock lock;

	// same stuff here as the stuff over there

    for (
        DisposeCallbackLinkList::Iterator itr = GetInstance().mCallbackList.GetBeginIter();
        itr != GetInstance().mCallbackList.GetEndIter();
    ) {
		DisposeCallbackLinkList::Iterator curItr = itr++;

		curItr->InvalidateWaveData(start, end);
	}
}

} // namespace detail
} // namespace snd
} // namespace nw4r
