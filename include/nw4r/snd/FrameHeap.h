#ifndef NW4R_SND_FRAME_HEAP_H
#define NW4R_SND_FRAME_HEAP_H

#include <nw4r/types_nw4r.h>

#include <nw4r/ut.h>

#include <revolution/MEM.h>

namespace nw4r {
namespace snd {
namespace detail {

class FrameHeap {
public:
    typedef void (*FreeCallback)(void* pBuffer, u32 size, void* pCallbackArg);

public:
    FrameHeap();
    ~FrameHeap();

    bool Create(void* pBase, u32 size);
    void Destroy();
    void Clear();
    void* Alloc(u32 size, FreeCallback pCallback, void* pCallbackArg);

    int SaveState();
    void LoadState(int id);

    int GetCurrentLevel() const;
    u32 GetFreeSize() const;

    bool IsValid() const {
        return mHandle != NULL;
    }

private:
    /******************************************************************************
     * Block
     ******************************************************************************/
    struct Block {
        nw4r::ut::LinkListNode mLink; // at 0x0
        void *mBuffer;
        u32 mSize;                    // at 0x8
        FreeCallback mCallback;       // at 0xC
        void* mCallbackArg;           // at 0x10

        Block(void *buffer, u32 size, FreeCallback pCallback, void* pCallbackArg) :
            mBuffer(buffer), mSize(size), mCallback(pCallback), mCallbackArg(pCallbackArg)
        {}
        ~Block() {
            if (mCallback != NULL) {
                mCallback(mBuffer, mSize, mCallbackArg);
            }
        }

        void* GetBufferAddr() {
            return mBuffer;
        }
    };

    typedef nw4r::ut::LinkList<Block, offsetof(Block, mLink)> BlockList;

    /******************************************************************************
     * Section
     ******************************************************************************/
    struct Section {
        nw4r::ut::LinkListNode mLink; // at 0x0
        BlockList mBlockList;         // at 0x8

        ~Section() {
            for (BlockList::Iterator it = mBlockList.GetEndIter();
                 it != mBlockList.GetBeginIter();) {

                (--it)->~Block();
            }
        }

        void AppendBlock(Block* pBlock) {
            mBlockList.PushBack(pBlock);
        }
    };

    typedef nw4r::ut::LinkList<Section, offsetof(Section, mLink)> SectionList;

    static const int BLOCK_BUFFER_SIZE = ROUND_UP(sizeof(Block), 32);
    static const int HEAP_ALIGN = 32;

private:
    bool NewSection();
    void ClearSection();

private:
    MEMiHeapHead* mHandle;    // at 0x0
    SectionList mSectionList; // at 0x4
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
