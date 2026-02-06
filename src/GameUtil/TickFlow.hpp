#ifndef GAMEUTIL_TICKFLOW_HPP
#define GAMEUTIL_TICKFLOW_HPP

#include <revolution/types.h>

#include "List.hpp"

#include "TickFlowDecl.hpp"

#include "Mem.hpp"

#include <nw4r/lyt.h>

TFD_EXTERN(lbl_802E4CA0)
TFD_EXTERN(lbl_802E4CC0)
TFD_EXTERN(lbl_802E4CE8)

#define TICKFLOW_DECL_CREATE_FN() static CTickFlow *create(void *memory, const TickFlowCode *code, f32 initRest);
#define TICKFLOW_IMPL_CREATE_FN(_className)                                               \
    CTickFlow *_className::create(void *memory, const TickFlowCode *code, f32 initRest) { \
        _className *instance = new (memory) _className(code, initRest);                   \
        return static_cast<CTickFlow *>(instance);                                        \
    }

class CTickFlow : public CList {
public:
    typedef CTickFlow *(*CreateFn)(void *memory, const TickFlowCode *code, f32 initRest);

    CTickFlow *getNext(void) const {
        return static_cast<CTickFlow *>(this->CList::getNext());
    }
    CTickFlow *getPrev(void) const {
        return static_cast<CTickFlow *>(this->CList::getPrev());
    }

    virtual ~CTickFlow(void);
    virtual void finalInsert(void);
    virtual void finalDestroy(void);
    virtual void _14(void);
    virtual void _18(CTickFlow *); 
    virtual bool _1C(u32 opcode, u32 arg0, const s32 *args);

    CTickFlow(const TickFlowCode *code, f32 initRest);

    bool fn_801DD9E8(void);
    
    void fn_801DEF8C(const TickFlowCode *code);
    
    static void fn_801DEF58(u8 accessIndex, nw4r::lyt::TextBox *textBox, nw4r::lyt::Pane *container);
    static nw4r::lyt::TextBox *fn_801DEF78(u8 accessIndex);

    s32 getCategory(void) const {
        return mCategory;
    }

    bool getButtonPromptIsReleased(void) const {
        return mButtonPromptIsReleased;
    }
    bool getButtonPromptIsPressed(void) const {
        return mButtonPromptIsPressed;
    }
    bool getButtonPromptIsPressedOrReleased(void) const {
        return mButtonPromptIsReleased || mButtonPromptIsPressed;
    }

private:
    u32 fn_801DECFC(const TickFlowCode *code, u32 labelId);
    u32 fn_801DEDFC(
        const TickFlowCode *code,
        u32 elseOp, u32 elseArg0,
        u32 defaultOp, u32 defaultArg0,
        u32 endOp, u32 endArg0,
        u32 beginNestedBlockOp, u32 endNestedBlockOp,
        u32 instrOffs,
        bool skipOneInstr
    );

protected:
    struct ExecutionFrame {
        const TickFlowCode *code;
        u32 instructionPos;
    };

    const TickFlowCode *mCode;

    s32 mInstanceCount; // Number of this instance of all TickFlow instances created while game is running.

    s32 mCategory;

    s32 mNextInstructionPos; // Index into mCode for the next instruction.
    f32 mCurrentRest; // In ticks.

    bool mExecPaused;

    s32 mCondvar;
    u32 mCondvarStack[16];
    u8 mCondvarStackPos;

    ExecutionFrame mExecStack[16];
    u8 mExecStackPos;

    bool mButtonPromptIsReleased;
    bool mButtonPromptIsPressed;
    u8 mButtonPromptControllerIdx;
    u32 mButtonPromptButton;
    s32 mButtonPromptPressSfx;
    s32 mButtonPromptReleaseSfx;
};

#endif
