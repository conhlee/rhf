#include <nw4r/ut.h>

namespace nw4r {
namespace ut {

namespace {

class LCImpl {
public:
    LCImpl() :
        mEnabled(false)
    {
        OSInitMutex(&mMutex);
    }

    void Enable() {
        Lock_ lock (*this);

        if (!mEnabled) {
            LCEnable();
            mEnabled = true;
        }
    }

    void Disable() {
        Lock_ lock (*this);

        if (mEnabled) {
            LC::QueueWaitEx(0);
            LCDisable();
            mEnabled = false;
        }
    }

    bool Lock() {
        OSLockMutex(&mMutex);

        if (mEnabled) {
            LC::QueueWaitEx(0);
            return true;
        }

        OSUnlockMutex(&mMutex);
        return false;
    }

    void Unlock() {
        LC::QueueWaitEx(0);
        OSUnlockMutex(&mMutex);
    }

private:
    class Lock_ {
    public:
        Lock_(LCImpl &impl) :
            mMutex(impl.mMutex)
        {
            OSLockMutex(&mMutex);
        }
        ~Lock_() {
            OSUnlockMutex(&mMutex);
        }

    private:
        OSMutex& mMutex;
    };

private:
    u8 mEnabled;
    OSMutex mMutex;
};

static LCImpl sLCImpl;

} // namespace

namespace LC {

void Enable() {
    sLCImpl.Enable();
}

void Disable() {
    sLCImpl.Disable();
}

bool Lock() {
    return sLCImpl.Lock();
}

void Unlock() {
    sLCImpl.Unlock();
}

void LoadBlocks(void* pDst, void* pSrc, u32 blocks) {
    LCLoadBlocks(pDst, pSrc, blocks);
}

void StoreBlocks(void* pDst, void* pSrc, u32 blocks) {
    LCStoreBlocks(pDst, pSrc, blocks);
}

void StoreData(void* pDst, void* pSrc, u32 size) {
    LCStoreData(pDst, pSrc, size);
}

} // namespace LC
    
} // namespace ut
} // namespace nw4r

