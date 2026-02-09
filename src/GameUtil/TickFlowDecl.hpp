#ifndef GAMEUTIL_TICKFLOWDECL_HPP
#define GAMEUTIL_TICKFLOWDECL_HPP

#include <revolution/types.h>

typedef u32 TickFlowCode;

enum {
    /*   TickFlow opcodes (0x000 to 0x050)   */

    TF_ASYNC_CALL = 0x000, ///< Args: TickFlowCode *, delay in ticks
    TF_CALL, ///< Args: TickFlowCode *
    TF_RETURN, // Return to the previous call.
    TF_STOP, // End execution.
    TF_CATEGORY, ///< Set category. Args: category to set
    TF_SET_CONDVAR, ///< Args: value to set condvar to
    TF_INCR_CONDVAR, ///< Args: value to increment condvar by
    TF_PUSH_CONDVAR,
    TF_POP_CONDVAR,
    TF_REST, ///< Arg0: ticks to rest for
    TF_00A,
    TF_REST_FRAMES, ///< Arg0: frames to rest for
    TF_UNREST, ///< Arg0: ticks to decrement the current rest by
    TF_RESET_TICK_PASS,
    TF_LABEL, ///< Arg0: label identifier
    TF_JUMP, ///< Arg0: identifier of label to jump to
    TF_IF, ///< TODO Multiple variations depending on arg0. Args: value to compare to condvar
    TF_ELSE,
    TF_ENDIF,
    TF_SWITCH_BEGIN,
    TF_SWITCH_CASE, ///< Arg0: value to compare to condvar
    TF_SWITCH_BREAK,
    TF_SWITCH_DEFAULT,
    TF_SWITCH_END,
    TF_018,
    TF_019,
    TF_TEMPO, ///< Set tempo. Arg0: BPM to set
    TF_TEMPO_SEQ, ///< Set tempo. Arg0: SID of sequence to reference BPM from
    TF_TEMPO_WAVE, ///< Set tempo. Arg0: SID of wave to reference BPM from
    TF_SPEED, ///< Set speed. Arg0: Speed value (fixed, 256)
    TF_01E,
    TF_01F,
    TF_020,
    TF_SPAWN_CELLANIM, ///< Create and display a new CellAnim. Arg0: TODO; Args: cell ID, anim ID, pos X, pos Y, display layer, scale X, scale Y
    TF_PLAY_SFX_VOL, ///< Play a SFX. Args: SID of SFX, volume (fixed, 256)
    TF_PLAY_SFX, ///< Play a SFX. Arg0: TODO; Args: SID of SFX, volume, pitch, pan, (UNUSED), start offset (all args are fixed, 256)
    TF_024,
    TF_025,
    TF_026,
    TF_027,
    TF_028,
    TF_029,
    TF_02A,
    TF_PREPARE_SEQ,
    TF_START_SEQ,
    TF_START_PREPARED_SEQ,
    TF_STOP_SEQ,
    TF_SET_SEQ_VOLUME,
    TF_030,
    TF_PREPARE_STRM,
    TF_START_STRM,
    TF_START_PREPARED_STRM,
    TF_STOP_STRM,
    TF_SET_STRM_VOLUME,
    TF_GET_STRM_PREPARED,
    TF_PREPARE_WAVE, ///< Prepare WSD (wave sound data) for playback. Arg0: SID of WSD
    TF_START_WAVE,
    TF_START_PREPARED_WAVE, ///< Start WSD (wave sound data) playback.
    TF_STOP_WAVE,
    TF_SET_WAVE_VOLUME,
    TF_GET_WAVE_PREPARED, ///< Set condvar to 1 if WSD (wave sound data) is ready, and 0 if not.
    TF_03D,
    TF_03E,
    TF_GET_GROUP_LOADING, ///< Set condvar to 1 if the current group is loading, and 0 if not.
    TF_MESG_PANE_VISIBLE, ///< Set a message pane's visibillity. Arg0: access index; Args: is visible
    TF_041,
    TF_SET_INPUT_ALLOW, ///< Set if inputs from the controller(s) should be allowed.
                        ///      - Arg0: 0 for all controllers (args will be: inputs allowed),
                        ///              1 for specific controller (args will be: controller index, inputs allowed).
    TF_043,
    TF_044,
    TF_045,
    TF_BUTTON_PROMPT,
    TF_047,
    TF_048,
    TF_SET_SKIPPABLE, ///< Set if the player is allowed to skip. Arg0: enabled
    TF_SET_SKIP_HANDLER, ///< Set the TickFlow that runs when a skip is triggered. Args: TickFlowCode *
    TF_04B,
    TF_04C,
    TF_ICI_CTRL,
    TF_04E,
    TF_RANDOM, ///< Set the condvar to a random value. Arg0: upper bound of random value

    /*   ExFlow opcodes (0x100 to 0x141) are found in Game/ExFlowDecl.hpp */
};

// decl macros

#define TFD_BEGIN(name) const TickFlowCode name[] = {
#define TFD_END() \
    }             \
    ;

#define TFD_EXTERN(name) extern const TickFlowCode name[];

#define TFD_CMD(command, argc, arg0) (                        \
    (((TickFlowCode)(command) & 0x3FFu)  <<  0) | /* 10bit */ \
    (((TickFlowCode)(argc)    & 0xFu)    << 10) | /* 4bit  */ \
    (((TickFlowCode)(arg0)    & 0x3FFFu) << 14)   /* 14bit */ \
)

#define TFD_STOP()         \
    TFD_CMD(TF_STOP, 0, 0) \
    TFD_END()
#define TFD_RETURN()         \
    TFD_CMD(TF_RETURN, 0, 0) \
    TFD_END()

#define TFD_CAST(value) ((TickFlowCode)(value))
#define TFD_PCAST(ptr) ((TickFlowCode)(const void *)(ptr))

// command macros

#define TFC_ASYNC_CALL(tfLabel, delayTicks) TFD_CMD(TF_ASYNC_CALL, 2, 0), TFD_PCAST(tfLabel), TFD_CAST(delayTicks),
#define TFC_CALL(tfLabel) TFD_CMD(TF_CALL, 1, 0), TFD_PCAST(tfLabel),

#define TFC_CATEGORY(category) TFD_CMD(TF_CATEGORY, 1, 0), TFD_CAST(category),

#define TFC_REST(ticks) TFD_CMD(TF_REST, 0, (ticks)),
#define TFC_REST_FRAMES(frames) TFD_CMD(TF_REST_FRAMES, 0, (frames)),

#define TFC_RESET_TICK_PASS() TFD_CMD(TF_RESET_TICK_PASS, 0, 0),

#define TFC_LABEL(labelID) TFD_CMD(TF_LABEL, 0, (labelID)),
#define TFC_JUMP(labelID) TFD_CMD(TF_JUMP, 0, (labelID)),

#define TFC_IF_EQU(value) TFD_CMD(TF_IF, 1, 0), TFD_CAST(value),
#define TFC_IF_NEQ(value) TFD_CMD(TF_IF, 1, 1), TFD_CAST(value),
#define TFC_IF_LT(value) TFD_CMD(TF_IF, 1, 2), TFD_CAST(value),
#define TFC_IF_LEQ(value) TFD_CMD(TF_IF, 1, 3), TFD_CAST(value),
#define TFC_IF_GT(value) TFD_CMD(TF_IF, 1, 4), TFD_CAST(value),
#define TFC_IF_GEQ(value) TFD_CMD(TF_IF, 1, 5), TFD_CAST(value),

#define TFC_ELSE() TFD_CMD(TF_ELSE, 0, 0),

#define TFC_ENDIF() TFD_CMD(TF_ENDIF, 0, 0),

#define TFC_SWITCH_BEGIN() TFD_CMD(TF_SWITCH_BEGIN, 0, 0),
#define TFC_SWITCH_CASE(value) TFD_CMD(TF_SWITCH_CASE, 0, (value)),
#define TFC_SWITCH_BREAK() TFD_CMD(TF_SWITCH_BREAK, 0, 0),
#define TFC_SWITCH_DEFAULT() TFD_CMD(TF_SWITCH_DEFAULT, 0, 0),
#define TFC_SWITCH_END() TFD_CMD(TF_SWITCH_END, 0, 0),

#define TFC_TEMPO(tempo) TFD_CMD(TF_TEMPO, 0, (tempo)),

#define TFC_TEMPO_WAVE(soundID) TFD_CMD(TF_TEMPO_WAVE, 0, (soundID)),

#define TFC_SPEED(speed) TFD_CMD(TF_SPEED, 0, (speed)),

#define TFC_PLAY_SFX(soundID) TFD_CMD(TF_PLAY_SFX, 1, 0), TFD_CAST(soundID),

#define TFC_PREPARE_WAVE(soundID) TFD_CMD(TF_PREPARE_WAVE, 0, (soundID)),
#define TFC_START_PREPARED_WAVE() TFD_CMD(TF_START_PREPARED_WAVE, 0, 0),
#define TFC_STOP_WAVE(fadeFrames) TFD_CMD(TF_STOP_WAVE, 0, (fadeFrames)),
#define TFC_SET_WAVE_VOLUME(volume, fadeFrames) TFD_CMD(TF_SET_WAVE_VOLUME, 2, 0), TFD_CAST(volume), TFD_CAST(fadeFrames),
#define TFC_GET_WAVE_PREPARED() TFD_CMD(TF_GET_WAVE_PREPARED, 0, 0),

#define TFC_GET_GROUP_LOADING() TFD_CMD(TF_GET_GROUP_LOADING, 0, 0),

#define TFC_MESG_PANE_VISIBLE(accessIdx, isVisible) TFD_CMD(TF_MESG_PANE_VISIBLE, 1, (accessIdx)), TFD_CAST((isVisible) ? TRUE : FALSE),

#define TFC_SET_INPUT_ALLOW_ALL(allowed) TFD_CMD(TF_SET_INPUT_ALLOW, 1, 0), TFD_CAST((allowed) ? TRUE : FALSE),
#define TFC_SET_INPUT_ALLOW(controllerIndex, allowed) TFD_CMD(TF_SET_INPUT_ALLOW, 2, 1), TFD_CAST(controllerIndex), TFD_CAST((allowed) ? TRUE : FALSE),

#define TFC_BUTTON_PROMPT_SET_CONTROLLER(controllerIndex), TFD_CMD(TF_BUTTON_PROMPT, 1, 0), TFD_CAST(controllerIndex),
#define TFC_BUTTON_PROMPT_SETUP(button, pressSFX, releaseSFX), TFD_CMD(TF_BUTTON_PROMPT, 4, 1), \
    TFD_CAST(button), TFD_CAST(pressSFX), TFD_CAST(releaseSFX),
#define TFC_BUTTON_PROMPT_GET_FINISHED() TFD_CMD(TF_BUTTON_PROMPT, 0, 2),

#define TFC_SET_SKIPPABLE(isSkippable) TFD_CMD(TF_SET_SKIPPABLE, 0, (isSkippable) ? TRUE : FALSE),

#define TFC_SET_SKIP_HANDLER(tfLabel) TFD_CMD(TF_SET_SKIP_HANDLER, 1, 0), TFD_PCAST(tfLabel),

#define TFC_ICI_START() TFD_CMD(TF_ICI_CTRL, 0, 0),
#define TFC_ICI_END(nameStr, ver) TFD_CMD(TF_ICI_CTRL, 2, 1), TFD_PCAST(nameStr), TFD_CAST(ver),


#define TFC_NOP() TFC_REST(0)

#endif
