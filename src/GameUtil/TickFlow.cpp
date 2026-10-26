#include "TickFlow.hpp"

#include <revolution/OS.h>

#include "TickFlowManager.hpp"

#include "Sound.hpp"

#include "InputCheckManager.hpp"

#include "Controller.hpp"

#include "Random.hpp"

#include "CellAnimManager.hpp"

#include "TickFlowDecl.hpp"

TFD_BEGIN(lbl_802E4CA0)
    TFC_LABEL(999)
    TFC_BUTTON_PROMPT_GET_FINISHED()
    TFC_REST(1)
    TFC_IF_EQU(TRUE)
        TFC_JUMP(999)
    TFC_ENDIF()
TFD_RETURN()

TFD_BEGIN(lbl_802E4CC0)
    TFC_LABEL(999)
    TFC_GET_WAVE_PREPARED()
    TFC_IF_EQU(TRUE)
        TFC_JUMP(100)
    TFC_ENDIF()
    TFC_REST(1)
    TFC_JUMP(999)
    TFC_LABEL(100)
TFD_RETURN()

TFD_BEGIN(lbl_802E4CE8)
    TFC_LABEL(999)
    TFC_GET_GROUP_LOADING()
    TFC_IF_EQU(FALSE)
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

static void setMesgPaneVisible(s32 index, bool visible) {
    lbl_803D5D38[index]->SetVisible(visible);
    if (lbl_803D5D58[index] != NULL) {
        lbl_803D5D58[index]->SetVisible(visible);
    }
}

static void setMesgPaneString(s32 index, const wchar_t *string) {
    lbl_803D5D38[index]->SetString(string);
}

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

void CTickFlow::_18(CTickFlow *other) {
#define COPY(member) other->member = member
#define COPYARR(member) for (s32 i = 0; i < (s32)ARRAY_LENGTH(member); i++) COPY(member[i])
    COPY(mCode);
    COPY(mInstanceCount);
    COPY(mCategory);
    COPY(mNextInstructionPos);
    COPY(mCurrentRest);
    COPY(mExecPaused);
    COPY(mCondvar);
    COPY(mCondvarStackPos);
    COPYARR(mCondvarStack);
    COPYARR(mExecStack);
    COPY(mExecStackPos);
    COPY(mButtonPromptControllerIdx);
    COPY(mButtonPromptIsReleased);
    COPY(mButtonPromptIsPressed);
    COPY(mButtonPromptButton);
    COPY(mButtonPromptPressSfx);
    COPY(mButtonPromptReleaseSfx);
#undef COPYARR
#undef COPY
}

bool CTickFlow::fn_801DD9E8(void) {
    if (mExecPaused) {
        return false;
    }
    else {
        bool isPressedOrReleased = mButtonPromptIsReleased || mButtonPromptIsPressed;
        if (isPressedOrReleased) {
            CController *controller = gControllerManager->fn_801D5FF0(mButtonPromptControllerIdx);
            if (mButtonPromptIsReleased) {
                if (mButtonPromptButton & controller->getTrig()) {
                    mButtonPromptIsReleased = false;
                    mButtonPromptIsPressed = true;
                    if (mButtonPromptPressSfx >= 0) {
                        gSoundManager->play(mButtonPromptPressSfx);
                    }
                }
            }
            else if (mButtonPromptIsPressed && (mButtonPromptButton & controller->getRelease())) {
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

// not matching (see TF_CALL, TF_01E, TF_MESG_PANE_VISIBLE, TF_BUTTON_PROMPT)
bool CTickFlow::_1C(u32 opcode, u32 arg0, const s32 *args) {
    switch (opcode) {
    case TF_ASYNC_CALL: {
        gTickFlowManager->fn_801E1CC0(
            reinterpret_cast<const TickFlowCode *>(args[0]), 
            mCurrentRest + gTickFlowManager->fn_801E2698() + static_cast<u32>(args[1])
        );
    } break;
    case TF_CALL: {
        // instruction swap between load of args[0] and mCode
        fn_801DEF8C(reinterpret_cast<const TickFlowCode *>(args[0]));
    } break;
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
        mCondvarStack[mCondvarStackPos] = mCondvar;
        mCondvarStackPos++;
        break;
    case TF_POP_CONDVAR:
        mCondvarStackPos--;
        mCondvar = mCondvarStack[mCondvarStackPos];
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
        s32 condvar = mCondvar;
        s32 value = args[0];
        
        bool condPass = false;
        switch (arg0) {
        case 0:
            condPass = condvar == value;
            break;
        case 1:
            condPass = condvar != value;
            break;
        case 2:
            condPass = condvar < value;
            break;
        case 3:
            condPass = condvar <= value;
            break;
        case 4:
            condPass = condvar > value;
            break;
        case 5:
            condPass = condvar >= value;
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
    case TF_CHECK_BGM_TYPE:
        if (arg0 == 0) {
            switch (args[0]) {
            case eBGMType_None:
                mCondvar = (gTickFlowManager->getCurrentBGMType() == eBGMType_None) ? TRUE : FALSE;
                break;
            case eBGMType_Seq:
                mCondvar = (gTickFlowManager->getCurrentBGMType() == eBGMType_Seq) ? TRUE : FALSE;
                break;
            case eBGMType_Strm:
                mCondvar = (gTickFlowManager->getCurrentBGMType() == eBGMType_Strm) ? TRUE : FALSE;
                break;
            case eBGMType_Wave:
                mCondvar = (gTickFlowManager->getCurrentBGMType() == eBGMType_Wave) ? TRUE : FALSE;
                break;
            }
        }
        break;
    case TF_TEMPO: {
        u16 tempo = arg0; // NOTE: arg0 (u32) is truncated to u16
        gTickFlowManager->fn_801E2B9C(tempo);
    } break;
    case TF_TEMPO_SEQ: {
        u16 tempo = gSoundManager->calc_seq_tempo_sound_id(arg0);
        if (tempo == 0) {
            tempo = 120;
            // "TFC_TEMPO_SEQ( %d ) : no tempo data was found\n"
            OSReport("TFC_TEMPO_SEQ( %d ) : テンポデータがありませんでした\n", arg0);
        }

        gTickFlowManager->fn_801E2B9C(tempo);
    } break;
    case TF_TEMPO_WAVE: {
        gTickFlowManager->fn_801E2B9C(gSoundManager->get_wave_tempo(arg0));
    } break;
    case TF_SPEED: {
        gTickFlowManager->fn_801E2C04(arg0 / 256.0f);
    } break;
    case TF_01E: {
        // TODO: regswap inside CLAMP
        f32 a2 = static_cast<u32>(args[2]) / 256.0f;
        f32 a1 = static_cast<u32>(args[1]) / 256.0f;
        f32 x = (gTickFlowManager->getSpeed() * static_cast<u32>(args[0])) / 256.0f;
        gTickFlowManager->fn_801E2C04(CLAMP(a1, a2, x));
    } break;
    case TF_01F: {
        u16 soundID = args[0];
        f32 temp_f31;
        if (arg0 == 0) {
            temp_f31 = static_cast<u32>(args[1]);
        }
        if (arg0 == 1) {
            temp_f31 = gSoundManager->calc_seq_tempo_sound_id(args[1]);
        }
        gTickFlowManager->fn_801E2C04(gSoundManager->get_wave_tempo(soundID) / temp_f31);
    } break;
    case TF_020: {
        f32 temp_f31;
        f32 temp_f1;
        switch (arg0) {
        case 0:
            temp_f31 = gSoundManager->calc_seq_tempo_sound_id(args[0]);
            temp_f1 = static_cast<u32>(args[1]);
            break;
        case 1:
            temp_f31 = gSoundManager->calc_seq_tempo_sound_id(args[0]);
            temp_f1 = gSoundManager->get_wave_tempo(args[1]);
            break;
        }
        if (temp_f31 == 0.0f) {
            temp_f31 = 120.0f;
        }
        if (temp_f1 == 0.0f) {
            temp_f1 = 120.0f;
        }
        gTickFlowManager->fn_801E2C04(temp_f1 / temp_f31);
    } break;

    case TF_SPAWN_CELLANIM: {
        f32 scaleX = args[5] / 256.0f;
        f32 scaleY = args[6] / 256.0f;

        s32 posX = args[2];
        s32 posY = args[3];

        s32 layer = args[4];

        CCellAnim *cellAnim = gCellAnimManager->createCellAnim(args[0], args[1]);
        
        if ((arg0 == 3) || (arg0 == 4) || (arg0 == 5)) {
            cellAnim->setPlaybackReverse(true);
        }
        if ((arg0 == 1) || (arg0 == 4)) {
            cellAnim->setLooping(true);
        }
        if ((arg0 == 2) || (arg0 == 5)) {
            cellAnim->setDestroyAtEnd(true);
        }
        cellAnim->setPos(posX, posY);
        cellAnim->setLayer(layer);
        cellAnim->setScale(scaleX, scaleY);

        cellAnim->playFromBegin();
    } break;

    case TF_PLAY_SFX_VOL: {
        SNDHandle *soundHandle = gTickFlowManager->fn_801E415C();

        f32 volume = static_cast<u32>(args[1]) / 256.0f;
        gSoundManager->play(args[0], 0.0f, soundHandle);
        gSoundManager->tune_volume(volume, 0, soundHandle);
    } break;
    case TF_PLAY_SFX: {
        SNDHandle *soundHandle = gTickFlowManager->fn_801E415C();
        f32 volume = 1.0f;
        f32 pitch = 1.0f;
        f32 pan = 0.0f;
        f32 delay = 0.0f;
        u16 soundID;

        switch (arg0) {
        case 5:
            delay = static_cast<u32>(args[5]) / 256.0f;
        // case 4:
        // // nothing
        case 3:
            pan = static_cast<s32>(args[3]) / 256.0f;
        case 2:
            pitch = static_cast<u32>(args[2]) / 256.0f;
        case 1:
            volume = static_cast<u32>(args[1]) / 256.0f;
        case 0:
            soundID = args[0];
            break;
        }

        gSoundManager->play(soundID, (f32)(s32)delay, soundHandle);
        gSoundManager->tune_volume(volume, 0, soundHandle);
        gSoundManager->tune_pitch(pitch, soundHandle);
        gSoundManager->tune_pan(pan, soundHandle);
        
    } break;
    case TF_SFX_VOL: {
        SNDHandle *soundHandle = gTickFlowManager->fn_801E415C();
        
        f32 volume = arg0 / 256.0f;
        s32 fadeFrames = gTickFlowManager->fn_801E26B4(static_cast<u32>(args[0]));

        gSoundManager->tune_volume(volume, fadeFrames, soundHandle);
    } break;
    case TF_SFX_PITCH: {
        SNDHandle *soundHandle = gTickFlowManager->fn_801E415C();
        gSoundManager->tune_pitch(static_cast<u32>(args[0]) / 256.0f, soundHandle);
    } break;
    case TF_SFX_PAN: {
        SNDHandle *soundHandle = gTickFlowManager->fn_801E415C();
        gSoundManager->tune_pan(args[0] / 256.0f, soundHandle);
    } break;
    case TF_SFX_TEMPO_RATIO: {
        SNDHandle *soundHandle = gTickFlowManager->fn_801E415C();
        gSoundManager->tune_tempo_ratio(static_cast<u32>(args[0]) / 256.0f, soundHandle);
    } break;
    case TF_SFX_TEMPO_REL: {
        SNDHandle *soundHandle = gTickFlowManager->fn_801E415C();
        if (arg0 == 0) {
            gSoundManager->tune_tempo_rel(static_cast<u16>(args[0]), soundHandle);
        }
        else if (arg0 == 1) {
            gSoundManager->tune_tempo_rel((u16)gSoundManager->get_wave_tempo(args[0]), soundHandle);
        }
    } break;
    case TF_SFX_STOP: {
        SNDHandle *soundHandle = gTickFlowManager->fn_801E415C();
        gSoundManager->tune_stop(gTickFlowManager->fn_801E26B4(arg0), soundHandle);
    } break;
    case TF_02A: {
        gTickFlowManager->fn_801E4154(gSoundManager->fn_801E7B30(arg0));
    } break;

    case TF_PREPARE_SEQ: {
        gTickFlowManager->prepareSeq(arg0);
    } break;
    case TF_START_SEQ: {
        gTickFlowManager->playSeq(arg0);
    } break;
    case TF_START_PREPARED_SEQ: {
        gTickFlowManager->playPreparedSeq();
    } break;
    case TF_STOP_SEQ: {
        gTickFlowManager->stopSeq((u16)arg0);
    } break;
    case TF_SET_SEQ_VOLUME: {
        gTickFlowManager->fn_801E334C(args[0] / 256.0, args[1]);
    } break;
    case TF_030: {
        gTickFlowManager->fn_801E33C0(args[0]);
    } break;

    case TF_PREPARE_STRM: {
        gTickFlowManager->prepareStrm(arg0);
    } break;
    case TF_START_STRM: {
        gTickFlowManager->playStrm(arg0);
    } break;
    case TF_START_PREPARED_STRM: {
        gTickFlowManager->playPreparedStrm();
    } break;
    case TF_STOP_STRM: {
        gTickFlowManager->stopStrm((u16)arg0);
    } break;
    case TF_SET_STRM_VOLUME: {
        gTickFlowManager->fn_801E38BC(args[0] / 256.0, args[1]);
    } break;
    case TF_GET_STRM_PREPARED: {
        mCondvar = gTickFlowManager->fn_801E38CC();
    } break;

    case TF_PREPARE_WAVE: {
        gTickFlowManager->prepareWave(arg0);
    } break;
    case TF_START_WAVE: {
        gTickFlowManager->playWave(arg0);
    } break;
    case TF_START_PREPARED_WAVE: {
        gTickFlowManager->playPreparedWave();
    } break;
    case TF_STOP_WAVE: {
        gTickFlowManager->stopWave((u16)arg0);
    } break;
    case TF_SET_WAVE_VOLUME: {
        gTickFlowManager->fn_801E3E2C(args[0] / 256.0, args[1]);
    } break;
    case TF_GET_WAVE_PREPARED: {
        mCondvar = gTickFlowManager->fn_801E3E3C();
    } break;
    
    case TF_SET_PLAYER_VOLUME: {
        f32 volume = static_cast<u32>(args[0]) / 256.0f;
        if (arg0 == 0) {
            gSoundManager->sys_player_volume(volume);
        } 
        if (arg0 == 1) {
            gSoundManager->nosys_player_volume(volume);
        } 
        if (arg0 == 2) {
            gSoundManager->all_player_volume(volume);
        }
    } break;
    case TF_SET_PLAYER_VOLUME_FADE: {
        if (arg0 == 0) {
            f32 volume = static_cast<u32>(args[0]) / 256.0f;
            s32 fadeFrames;
            if (args[1]) {
                fadeFrames = args[2];
            }
            else {
                fadeFrames = gTickFlowManager->fn_801E26B4(args[2]);
            }
            gSoundManager->nosys_player_vol_fade(volume, fadeFrames);
        }
        else if (arg0 == 1) {
            gSoundManager->nosys_player_vol_fade_stop();
        }
    } break;
    case TF_GET_GROUP_LOADING: {
        mCondvar = gSoundManager->get_loading();
    } break;

    case TF_MESG_PANE_VISIBLE: {
        // TODO: Issues with Pane SetVisible & detail::SetBit
        setMesgPaneVisible(arg0, args[0]);
    } break;
    case TF_MESG_PANE_SET_STRING: {
        setMesgPaneString(arg0, reinterpret_cast<const wchar_t *>(args[0]));
    } break;

    case TF_SET_INPUT_ALLOW: {
        if (arg0 == 0) {
            gInputCheckManager->setUnk429(args[0]);
        }
        else if (arg0 == 1) {
            gInputCheckManager->setUnk42A(args[1], args[0]);
        }
    } break;
    case TF_043: {
        gInputCheckManager->setUnk42E(args[0]);
    } break;
    case TF_044: {
        if (arg0 == 0) {
            gInputCheckManager->setUnk484(args[0]);
        }
        else if (arg0 == 1) {
            gInputCheckManager->fn_801E923C(args[0]);
        }
    } break;
    case TF_045: {
        if (arg0 == 0) {
            gInputCheckManager->fn_801E9C30(args[0]);
        }
        else if (arg0 == 1) {
            gInputCheckManager->fn_801E9C38(static_cast<u32>(args[0]) / 256.0f);
        }
    } break;

    case TF_BUTTON_PROMPT: {
        switch (arg0) {
        case 0:
            mButtonPromptControllerIdx = args[0];
            break;
        case 1:
            // instruction order
            mButtonPromptIsReleased = true;
            mButtonPromptIsPressed = false;
            mButtonPromptButton = args[0];
            mButtonPromptPressSfx = args[1];
            mButtonPromptReleaseSfx = args[2];
            break;
        case 2:
            mCondvar = getButtonPromptIsPressedOrReleased();
            break;
        }
    } break;

    case TF_047: {
        if (arg0 == 0) {
            gInputCheckManager->fn_801E9C40();
        }
        else if (arg0 == 1) {
            gInputCheckManager->fn_801E9D58(args[0], args[1]);
        }
    } break;
    case TF_SET_BUTTON_COOLDOWN: {
        gControllerManager->fn_801D5FF0(args[0])->fn_801D5500(args[1], args[2]);
    } break;

    case TF_SET_SKIPPABLE: {
        gTickFlowManager->setUnkFC(arg0);
    } break;
    case TF_SET_SKIP_HANDLER: {
        gTickFlowManager->setUnkF4(reinterpret_cast<const TickFlowCode *>(args[0]));
    } break;
    case TF_SET_SKIP_CONTROLLER: {
        gTickFlowManager->setUnkFD(args[0]);
    } break;
    case TF_SET_SKIP_INPUT: {
        gTickFlowManager->setUnk100(args[0]);
    } break;

    case TF_ICI_CTRL: {
        char buf[32];

        if (arg0 == 0) {
            gInputCheckManager->fn_801E9D84();
        }
        else if (arg0 == 1) {
            char buf[32];
            sprintf(buf, "%s_%d.ici", reinterpret_cast<char *>(args[0]), args[1]);
            gInputCheckManager->fn_801E9D80(buf);
        }
    } break;

    case TF_CONTROLLER_DO_MOTOR_SEQ: {
        const char *seqText = reinterpret_cast<const char *>(args[1]);
        gControllerManager->fn_801D5FF0(args[0])->_40(seqText, false);
    } break;

    case TF_RANDOM: {
        mCondvar = sRandom.nextU32(arg0);
    } break;
    }
    return false;
}

// not matching (regswaps, weird loads, temp_r7 not generating)
u32 CTickFlow::fn_801DECFC(const TickFlowCode *code, u32 labelId) {
#define FINDONE() \
    opcode = BYTECODE_GET_OPCODE(code[idx]); \
    argc = BYTECODE_GET_ARGC(code[idx]); \
    arg0 = BYTECODE_GET_ARG0(code[idx]); \
    if ((opcode == TF_LABEL) && (arg0 == labelId)) { \
        return idx; \
    } \
    if (opcode == TF_STOP) { \
        break; \
    } \
    idx += argc + 1

    u32 idx = 0;
    u32 temp_r7 = 0;

    for (s32 i = 0; i < 0x4000; i++) {
        u32 opcode;
        u32 argc;
        u32 arg0;
        FINDONE();
        FINDONE();
        FINDONE();
        FINDONE();
        temp_r7 += 3;
    }
    return 0;
#undef FINDONE
}

// not matching (regswaps, weird loads)
u32 CTickFlow::fn_801DEDFC(
    const TickFlowCode *code,
    u32 elseOp, u32 elseArg0,
    u32 defaultOp, u32 defaultArg0,
    u32 endOp, u32 endArg0,
    u32 beginNestedBlockOp, u32 endNestedBlockOp,
    u32 instrOffs,
    bool skipOneInstr
) {
    s32 nestDepth = 0;

    for (s32 i = 0; i < 0x10000; i++) {
        u32 op, argc, arg0;
        op   = BYTECODE_GET_OPCODE(code[instrOffs]);
        argc = BYTECODE_GET_ARGC(code[instrOffs]);
        arg0 = BYTECODE_GET_ARG0(code[instrOffs]);

        if (
            nestDepth == 0 && (
                (op == elseOp && arg0 == elseArg0) ||
                (op == defaultOp && arg0 == defaultArg0) ||
                (op == endOp && arg0 == endArg0)
            )
        ) {
            if (skipOneInstr) {
                return instrOffs + 1 + argc; 
            }
            return instrOffs;
        }

        if (op == beginNestedBlockOp) {
            nestDepth++;
        }
        if (op == endNestedBlockOp) {
            nestDepth--;
        }

        instrOffs += 1 + argc;
    }

    return 0;
}


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
    mExecStackPos++;
    mCode = code;
    mNextInstructionPos = 0;
}

void CTickFlow::_14(void) {
    this->~CTickFlow();
}