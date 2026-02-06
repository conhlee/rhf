#ifndef NW4R_UT_TAG_PROCESSOR_BASE_H
#define NW4R_UT_TAG_PROCESSOR_BASE_H
#include <nw4r/types_nw4r.h>

namespace nw4r {
namespace ut {

// Forward declarations
struct Rect;

template <typename T>
class TextWriterBase;
template <typename T>
struct PrintContext {
    enum Flags {
        FLAGS_CHARSPACE = (1 << 0),
    };

    TextWriterBase<T>* writer; // at 0x0
    const T* str;              // at 0x4
    f32 x;                     // at 0x8
    f32 y;                     // at 0xC
    u32 flags;                 // at 0x10
};
template <typename T>
class TagProcessorBase {
public:
    typedef PrintContext<T> ContextType;

    enum Operation {
        OPERATION_DEFAULT,
        OPERATION_NO_CHAR_SPACE,
        OPERATION_CHAR_SPACE,
        OPERATION_NEXT_LINE,
        OPERATION_END_DRAW
    };

public:
    TagProcessorBase();
    virtual ~TagProcessorBase(); // at 0x8

    virtual Operation Process(u16 code, ContextType *context); // at 0xC
    virtual Operation CalcRect(Rect *pRect, u16 code, ContextType *context); // at 0x10

private:
    void ProcessLinefeed(ContextType *context);
    void ProcessTab(ContextType *context);
};

} // namespace ut
} // namespace nw4r

#endif