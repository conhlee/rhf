#include "TickFlow.hpp"

#include <revolution/OS.h>

#include "TickFlowManager.hpp"

#include "SoundManager.hpp"

#include "Controller.hpp"

#include "TickFlowDecl.hpp"

TFD_BEGIN(lbl_802E4CA0)
    TFC_LABEL(999)
    TFC_BUTTON_PROMPT_GET_FINISHED()
    TFC_REST(1)
    TFC_IF_EQU(1)
        TFC_JUMP(999)
    TFC_ENDIF()
TFD_RETURN()

TFD_BEGIN(lbl_802E4CC0)
    TFC_LABEL(999)
    TFC_GET_WAVE_READY()
    TFC_IF_EQU(1)
        TFC_JUMP(100)
    TFC_ENDIF()
    TFC_REST(1)
    TFC_JUMP(999)
    TFC_LABEL(100)
TFD_RETURN()

TFD_BEGIN(lbl_802E4CE8)
    TFC_LABEL(999)
    TFC_GET_GROUP_LOADING()
    TFC_IF_EQU(0)
        TFC_JUMP(100)
    TFC_ENDIF()
    TFC_REST(1)
    TFC_JUMP(999)
    TFC_LABEL(100)
TFD_RETURN()

#define BYTECODE_GET_OPCODE(instruction) (((u32)(instruction) >>  0) & 0x3FF)
#define BYTECODE_GET_ARGC(instruction)   (((u32)(instruction) >> 10) & 0xF)
#define BYTECODE_GET_ARG0(instruction)   (((u32)(instruction) >> 14))

static nw4r::lyt::TextBox *lbl_803D5D38[8];
static nw4r::lyt::Pane *lbl_803D5D58[8];

CTickFlow::CTickFlow(const TickFlowCode *code, f32 initRest) {
    mCode = code;
    mInstanceCount = gTickFlowManager->fn_801E4124();
    mCategory = 2000;
    mNextInstructionPos = 0;
    mCurrentRest = initRest;
    mExecPaused = false;
    mButtonPromptControllerIdx = 0;
    mButtonPromptIsReleased = false;
    mButtonPromptIsPressed = false;
    mButtonPromptPressSfx = -1;
    mButtonPromptReleaseSfx = -1;
    mCondvar = 0;
    mCondvarStackPos = 0;
    mExecStackPos = 0;
}

CTickFlow::~CTickFlow(void) {}

bool CTickFlow::fn_801DD9E8(void) {
    if (mExecPaused) {
        return false;
    }
    else {
        bool isPressedOrReleased = mButtonPromptIsReleased || mButtonPromptIsPressed;
        if (isPressedOrReleased) {
            CController *controller = gControllerManager->fn_801D5FF0(mButtonPromptControllerIdx);
            if (mButtonPromptIsReleased) {
                if (mButtonPromptButton & controller->getUnk133C()) {
                    mButtonPromptIsReleased = false;
                    mButtonPromptIsPressed = true;
                    if (mButtonPromptPressSfx >= 0) {
                        gSoundManager->play(mButtonPromptPressSfx);
                    }
                }
            }
            else if (mButtonPromptIsPressed && (mButtonPromptButton & controller->getUnk1340())) {
                mButtonPromptIsPressed = false;
                if (mButtonPromptReleaseSfx >= 0) {
                    gSoundManager->play(mButtonPromptReleaseSfx);
                }
            }
        }

        mCurrentRest -= gTickFlowManager->fn_801E2698();
        mExecPaused = true;

        while (mCurrentRest < 0.0f) {
            const u32 *pInstruction = mCode + mNextInstructionPos;
            const s32 *pArg = reinterpret_cast<const s32 *>(pInstruction + 1);

            u32 instruction = *pInstruction;

            u32 opcode = BYTECODE_GET_OPCODE(instruction);
            u32 argc   = BYTECODE_GET_ARGC(instruction);
            s32 arg0   = BYTECODE_GET_ARG0(*(pArg - 1)); // What!

            mNextInstructionPos += 1 + argc;
            if (_1C(opcode, arg0, pArg)) {
                return true;
            }
        }
        return false;
    }
}

bool CTickFlow::_1C(u32 opcode, u32 arg0, const s32 *args) {
    switch (opcode) {
    case TF_ASYNC_CALL: {
        gTickFlowManager->fn_801E1CC0(
            reinterpret_cast<const TickFlowCode *>(args[0]),
            args[1] + mCurrentRest + gTickFlowManager->fn_801E2698()
        );
    } break;
    case TF_CALL:
        mExecStack[mExecStackPos].code = mCode;
        mExecStack[mExecStackPos].instructionPos = mNextInstructionPos;
        mCode = reinterpret_cast<const TickFlowCode *>(args[0]);
        mExecStackPos++;
        mNextInstructionPos = 0;
        break;
    case TF_RETURN:
        mExecStackPos--;
        mCode = mExecStack[mExecStackPos].code;
        mNextInstructionPos = mExecStack[mExecStackPos].instructionPos;
        break;
    case TF_STOP:
        return true;
    case TF_CATEGORY:
        mCategory = args[0];
        break;
    case TF_SET_CONDVAR:
        mCondvar = args[0];
        break;
    case TF_INCR_CONDVAR:
        mCondvar += args[0];
        break;
    case TF_PUSH_CONDVAR:
        mCondvarStack[mCondvarStackPos++] = mCondvar;
        break;
    case TF_POP_CONDVAR:
        mCondvar = mCondvarStack[--mCondvarStackPos];
        break;
    case TF_REST:
        mCurrentRest += arg0;
        break;
    case TF_00A:
        if (arg0 == 0) {
            mCurrentRest += gTickFlowManager->fn_801E4144(args[0]);
        }
        else if (arg0 == 1) {
            gTickFlowManager->fn_801E4134(args[0], args[1]);
        }
        break;
    case TF_REST_FRAMES:
        mCurrentRest += gTickFlowManager->fn_801E2928(arg0);
        break;
    case TF_UNREST:
        mCurrentRest -= arg0;
        break;
    case TF_RESET_TICK_PASS:
        if (arg0 == 0) {
            gTickFlowManager->fn_801E26A8();
        }
        break;
    case TF_JUMP:
        mNextInstructionPos = fn_801DECFC(mCode, arg0);
        break;
    case TF_IF: {
        s32 value = args[0];
        s32 condvar = mCondvar;
        
        bool condPass = false;
        switch (arg0) {
        case 0:
            condPass = value == condvar;
            break;
        case 1:
            condPass = value != condvar;
            break;
        case 2:
            condPass = value < condvar;
            break;
        case 3:
            condPass = value <= condvar;
            break;
        case 4:
            condPass = value > condvar;
            break;
        case 5:
            condPass = value >= condvar;
            break;
        }

        if (!condPass) {
            mNextInstructionPos = fn_801DEDFC(mCode,
                TF_ELSE, 0,
                TF_ENDIF, 0,
                TF_ENDIF, 0,
                TF_IF, TF_ENDIF,
                mNextInstructionPos, true
            );
        }
    } break;
    case TF_ELSE:
        mNextInstructionPos = fn_801DEDFC(mCode,
            TF_ENDIF, 0,
            TF_ENDIF, 0,
            TF_ENDIF, 0,
            TF_IF, TF_ENDIF,
            mNextInstructionPos, true
        );
        break;
    case TF_SWITCH_BEGIN:
        mNextInstructionPos = fn_801DEDFC(mCode,
            TF_SWITCH_CASE, mCondvar,
            TF_SWITCH_DEFAULT, 0,
            TF_SWITCH_END, 0,
            TF_SWITCH_BEGIN, TF_SWITCH_END,
            mNextInstructionPos, true
        );
        break;
    case TF_SWITCH_BREAK:
        mNextInstructionPos = fn_801DEDFC(mCode,
            TF_SWITCH_END, 0,
            TF_SWITCH_END, 0,
            TF_SWITCH_END, 0,
            TF_SWITCH_BEGIN, TF_SWITCH_END,
            mNextInstructionPos, true
        );
        break;
    case TF_019:
        if (arg0 == 0) {
            switch (args[0]) {
            case 0:
                mCondvar = (gTickFlowManager->getUnk1C() == 0) ? 1 : 0;
                break;
            case 1:
                mCondvar = (gTickFlowManager->getUnk1C() == 1) ? 1 : 0;
                break;
            case 2:
                mCondvar = (gTickFlowManager->getUnk1C() == 2) ? 1 : 0;
                break;
            case 3:
                mCondvar = (gTickFlowManager->getUnk1C() == 3) ? 1 : 0;
                break;
            }
        }
        break;
    case TF_TEMPO: {
        gTickFlowManager->fn_801E2B9C((u16)arg0);
    } break;
    case TF_TEMPO_SEQ: {
        f32 seqTempo = gSoundManager->fn_801E75C0(arg0);
        u16 seqTempoInt = seqTempo;
        if (seqTempoInt == 0) {
            seqTempoInt = 120;

            // "TFC_TEMPO_SEQ( %d ) : no tempo data was found\n"
            OSReport("TFC_TEMPO_SEQ( %d ) : テンポデータがありませんでした\n", arg0);
        }
        gTickFlowManager->fn_801E2B9C(seqTempoInt);
    } break;
    case TF_TEMPO_WAVE: {
        WaveInfo *waveInfo = gSoundManager->fn_801E73D4(arg0);
        f32 waveTempo = gSoundManager->fn_801E74EC(waveInfo);
        gTickFlowManager->fn_801E2B9C(waveTempo);
    } break;
    case TF_SPEED: {
        gTickFlowManager->fn_801E2C04(arg0 / 256.0f);
    } break;
    case TF_01E: {
        
    } break;
    case TF_01F: {
    } break;
        
    case TF_020: {
        
    } break;
    case TF_SPAWN_CELLANIM: {
        
    } break;
    case TF_PLAY_SFX_VOL: {
        
    } break;
    }
    return false;
}

/*
u32 CTickFlow::fn_801DECFC(const TickFlowCode *code, u32 labelId) {

}
*/

/*
u32 CTickFlow::fn_801DEDFC(
    const TickFlowCode *code,
    u32 elseOp, u32 elseArg0,
    u32 defaultOp, u32 defaultArg0,
    u32 endOp, u32 endArg0,
    u32 beginNestedBlockOp, u32 endNestedBlockOp,
    u32 instrOffs,
    bool skipOneInstr
) {

}
*/

void CTickFlow::finalInsert(void) {}

void CTickFlow::finalDestroy(void) {
    gTickFlowManager->fn_801E2540(this);
}

void CTickFlow::fn_801DEF58(u8 accessIndex, nw4r::lyt::TextBox *textBox, nw4r::lyt::Pane *container) {
    lbl_803D5D38[accessIndex] = textBox;
    lbl_803D5D58[accessIndex] = container;
}

nw4r::lyt::TextBox *CTickFlow::fn_801DEF78(u8 accessIndex) {
    return lbl_803D5D38[accessIndex];
}

void CTickFlow::fn_801DEF8C(const TickFlowCode *code) {
    mExecStack[mExecStackPos].code = mCode;
    mExecStack[mExecStackPos].instructionPos = mNextInstructionPos;
    mCode = code;
    mExecStackPos++;
    mNextInstructionPos = 0;
}

void CTickFlow::_14(void) {
    this->~CTickFlow();
}