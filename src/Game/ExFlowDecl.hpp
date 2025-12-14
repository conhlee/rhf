#ifndef GAME_EXFLOWDECL_HPP
#define GAME_EXFLOWDECL_HPP

#include <revolution/types.h>

#include "TickFlowDecl.hpp"

#include "SNDHeap.hpp"

#include "CellAnimRenderEff.hpp"

enum {
    /*   ExFlow opcodes (0x100 to 0x141)  */

    TF_SET_SCENE = 0x100,       ///< Set the current scene. Args: scene ID, TickFlowCode *
    TF_SET_SCENE_VER,           ///< Set a scene's version. Args: scene ID, version
    TF_SCENE_ASSETS,            ///< (Un)load a scene's assets (cellanim data & layouts). Arg0: 0 for load, 1 for unload; Args: scene ID
    TF_103,
    TF_GET_SCENE_RUNNING,       ///< Set condvar to 1 if the current scene is running, and 0 if not.
    TF_SET_PROLOGUE,            ///< Set the prologue. Arg0: is 2P; Args: name, version
    TF_SET_EPILOGUE,            ///< Set the epilogue. Args: name, version
    TF_SET_EPILOGUE_MSG,        ///< Set a epilogue message. Arg0: type: 0 for OK, 1 for Try Again, 2 for Superb; Args: mesg ID string
    TF_SET_GRADING_CAPTION,     ///< Set the grading caption. Args: mesg ID string
    TF_PROLOGUE_JINGLE,         ///< Set/get the prologue jingle. Arg0: 0 for set (arg will be jingle num), 1 for get (condvar will be jingle num).
    TF_FADE,                    ///< Fade the screen in/out. Arg0: 0 for fade out, 1 for fade in; Args: time is in frames, time (in ticks or frames).
    TF_10B,
    TF_UI_SKIP_POS,             ///< Set the "(-) Skip" graphic's position.
                                ///      - Arg0: position: 0 for top-left, 1 for top-right,
                                ///                        2 for bottom-left, 3 for bottom-right.
    TF_UI_SKIP_ANIM,            ///< Play the "(-) Skip" graphic's animation. Args: is practice
    TF_UI_PAUSE,                ///< Show/hide the "(+) Pause" graphic. Arg0: 0 for show, 1 for hide
    TF_10F,
    TF_110,
    TF_111,
    TF_MASK01_VISIBLE,          ///< Set mask01's visibility. Args: 0 for invisible, 1 for visible 
    TF_MASK01_OPACITY,          ///< Set mask01's opacity. Args: opacity (0-255)
    TF_MASK01_COLOR,            ///< Set mask01's color. Args: color R (0-255), color G (0-255), color B (0-255)
    TF_MASK01_LAYER,            ///< Set mask01's layer. Args: layer
    TF_MASK01_FADE,             ///< Fade mask01's opacity. Arg0: TODO; Args: target opacity, time in ticks
    TF_117,
    TF_CHECKPOINT_SUBMIT_CRITERIA,
    TF_CHECKPOINT_SET_CRITERIA,
    TF_CHECKPOINT_ENABLE_SCORING,
    TF_11B,
    TF_11C,
    TF_CHECKPOINT_GRADING,
    TF_11E,
    TF_11F,
    TF_120,
    TF_TUTORIAL,
    TF_122,
    TF_123,
    TF_MESG_PANE_SET_TEXT,      ///< Set a message pane's text. Arg0: access index; Args: mesg ID string
    TF_125,
    TF_126,
    TF_127,
    TF_128,
    TF_129,
    TF_12A,
    TF_12B,
    TF_LOAD_SND_GROUP,          ///< Start loading a sound group. Arg0: type (1 for music, 2 for sfx); Args: SID of group
    TF_UNLOAD_SND_GROUPS,       ///< Unload all groups of a specific type. Arg0: type (1 for music, 2 for sfx)
    TF_RESET_SND_GROUP_HEAP,    ///< Reset a sound group's heap. Arg0: type (1 for music, 2 for sfx)
                                ///      - Note: this doesn't unload any previously loaded data, but it will be
                                ///              overwritten by newly allocated data. TODO this explanation is totally wrong!!!!!!!!!!!!
    TF_12F,
    TF_130,
    TF_131,
    TF_CELLANIM_RENDER_EFF,
    TF_133,
    TF_134,
    TF_135,
    TF_136,
    TF_137,
    TF_138,
    TF_139,
    TF_13A,
    TF_13B,
    TF_13C,
    TF_13D,
    TF_13E,
    TF_REMIX8_MASK_MANAGE,      ///< Manage the Remix 8 mask. Arg0: operation (0: load, 1: unload, 2: enquire ready (sets condvar), 3: init, 4: set visible, 5: set layer); Args: is visible / layer
    TF_ENDLESS_MANAGE,          ///< Manage Endless Remix. Arg0: operation (0: load, 1: unload, 2: enquire ready (sets condvar), 3: init, 4: get random scene ID (sets condvar))
    TF_141
};

// command macros

#define TFC_SET_SCENE(sceneId, tfLabel) TFD_CMD(TF_SET_SCENE, 2, 0), TFD_CAST(sceneId), TFD_PCAST(tfLabel),
#define TFC_100_1() TFD_CMD(TF_SET_SCENE, 0, 1), // TODO: rename

#define TFC_SET_SCENE_VER(sceneId, ver) TFD_CMD(TF_SET_SCENE_VER, 2, 0), TFD_CAST(sceneId), TFD_CAST(ver),

#define TFC_LOAD_SCENE_ASSETS(sceneId) TFD_CMD(TF_SCENE_ASSETS, 1, 0), TFD_CAST(sceneId),

#define TFC_GET_SCENE_RUNNING() TFD_CMD(TF_GET_SCENE_RUNNING, 0, 0),

#define TFC_SET_PROLOGUE(nameStr, ver) TFD_CMD(TF_SET_PROLOGUE, 2, 0), TFD_PCAST(nameStr), TFD_CAST(ver),

#define TFC_SET_EPILOGUE(nameStr, ver) TFD_CMD(TF_SET_EPILOGUE, 2, 0), TFD_PCAST(nameStr), TFD_CAST(ver),

#define TFC_SET_EPILOGUE_MSG_OK(mesgIdStr) TFD_CMD(TF_SET_EPILOGUE_MSG, 1, 0), TFD_PCAST(mesgIdStr),
#define TFC_SET_EPILOGUE_MSG_TRYAGAIN(mesgIdStr) TFD_CMD(TF_SET_EPILOGUE_MSG, 1, 1), TFD_PCAST(mesgIdStr),
#define TFC_SET_EPILOGUE_MSG_SUPERB(mesgIdStr) TFD_CMD(TF_SET_EPILOGUE_MSG, 1, 2), TFD_PCAST(mesgIdStr),

#define TFC_SET_GRADING_CAPTION(mesgIdStr) TFD_CMD(TF_SET_GRADING_CAPTION, 1, 0), TFD_PCAST(mesgIdStr),

#define TFC_SET_PROLOGUE_JINGLE(jingleNum) TFD_CMD(TF_PROLOGUE_JINGLE, 1, 0), TFD_CAST(jingleNum),
#define TFC_GET_PROLOGUE_JINGLE() TFD_CMD(TF_PROLOGUE_JINGLE, 0, 1),

#define TFC_UI_PAUSE_SHOW() TFD_CMD(TF_UI_PAUSE, 0, 0),
#define TFC_UI_PAUSE_HIDE() TFD_CMD(TF_UI_PAUSE, 0, 1),

#define TFC_FADE_IN_FRAMES(frames) TFD_CMD(TF_FADE, 2, 1), TFD_CAST(1), TFD_CAST(frames),
#define TFC_FADE_OUT_FRAMES(frames) TFD_CMD(TF_FADE, 2, 0), TFD_CAST(1), TFD_CAST(frames),

#define TFC_FADE_IN_TICKS(frames) TFD_CMD(TF_FADE, 2, 1), TFD_CAST(0), TFD_CAST(frames),
#define TFC_FADE_OUT_TICKS(frames) TFD_CMD(TF_FADE, 2, 0), TFD_CAST(0), TFD_CAST(frames),

#define TFC_UI_SKIP_POS(pos) TFD_CMD(TF_UI_SKIP_POS, 0, pos),

#define TFC_UI_SKIP_ANIM() TFD_CMD(TF_UI_SKIP_ANIM, 1, 0), TFD_CAST(0),
#define TFC_UI_SKIP_ANIM_PRACTICE() TFD_CMD(TF_UI_SKIP_ANIM, 1, 0), TFD_CAST(1),

#define TFC_MASK01_VISIBLE(isVisible) TFD_CMD(TF_MASK01_VISIBLE, 1, 0), TFD_CAST((isVisible) ? 1 : 0),
#define TFC_MASK01_OPACITY(opacity) TFD_CMD(TF_MASK01_OPACITY, 1, 0), TFD_CAST(opacity),
#define TFC_MASK01_COLOR(r, g, b) TFD_CMD(TF_MASK01_COLOR, 3, 0), TFD_CAST(r), TFD_CAST(g), TFD_CAST(b),
#define TFC_MASK01_LAYER(layer) TFD_CMD(TF_MASK01_LAYER, 1, 0), TFD_CAST(layer),
#define TFC_MASK01_FADE(targetOpacity, ticks) TFD_CMD(TF_MASK01_FADE, 2, 1), TFD_CAST(targetOpacity), TFD_CAST(ticks),

#define TFC_CHECKPOINT_SUBMIT_CRITERIA_DIRECTMESG(criteriaList) TFD_CMD(TF_CHECKPOINT_SUBMIT_CRITERIA, 1, 0), TFD_PCAST(criteriaList),

#define TFC_CHECKPOINT_SUBMIT_CRITERIA(criteriaList) TFD_CMD(TF_CHECKPOINT_SUBMIT_CRITERIA, 1, 1), TFD_PCAST(criteriaList),
#define TFC_CHECKPOINT_SUBMIT_CRITERIA_END(criteriaList, nameStr, ver) \
    TFD_CMD(TF_CHECKPOINT_SUBMIT_CRITERIA, 3, 2), TFD_PCAST(criteriaList), TFD_PCAST(nameStr), TFD_CAST(ver),

#define TFC_CHECKPOINT_SET_CRITERIA(criteriaIndex) TFD_CMD(TF_CHECKPOINT_SET_CRITERIA, 1, 0), TFD_CAST(criteriaIndex),

#define TFC_CHECKPOINT_SCORING_START() TFD_CMD(TF_CHECKPOINT_ENABLE_SCORING, 1, 0), TFD_CAST(1),
#define TFC_CHECKPOINT_SCORING_STOP() TFD_CMD(TF_CHECKPOINT_ENABLE_SCORING, 1, 0), TFD_CAST(0),

#define TFC_CHECKPOINT_DO_GRADING() TFD_CMD(TF_CHECKPOINT_GRADING, 0, 0),
#define TFC_CHECKPOINT_GET_GRADE() TFD_CMD(TF_CHECKPOINT_GRADING, 0, 1),

#define TFC_TUTORIAL_RESET_COUNTER() TFD_CMD(TF_TUTORIAL, 0, 0),
#define TFC_TUTORIAL_RESET_MISSED() TFD_CMD(TF_TUTORIAL, 0, 1),
#define TFC_TUTORIAL_INCR_COUNTER_IF_NOT_MISSED() TFD_CMD(TF_TUTORIAL, 0, 2),
#define TFC_TUTORIAL_GET_COUNTER() TFD_CMD(TF_TUTORIAL, 0, 3),
#define TFC_TUTORIAL_SIGNAL_MISS() TFD_CMD(TF_TUTORIAL, 0, 4),
#define TFC_TUTORIAL_SEAMLESS_FINISH() TFD_CMD(TF_TUTORIAL, 0, 5),
#define TFC_TUTORIAL_GET_SEAMLESS_FINISHED() TFD_CMD(TF_TUTORIAL, 0, 6),
#define TFC_TUTORIAL_GET_MISSED() TFD_CMD(TF_TUTORIAL, 0, 7),

#define TFC_MESG_PANE_SET_TEXT(accessIdx, mesgIdStr) TFD_CMD(TF_MESG_PANE_SET_TEXT, 1, (accessIdx)), TFD_PCAST(mesgIdStr),

#define TFC_LOAD_SND_GROUP_0(sid) TFD_CMD(TF_LOAD_SND_GROUP, 1, eSoundHeap_0), TFD_CAST(sid),
#define TFC_LOAD_SND_GROUP_1(sid) TFD_CMD(TF_LOAD_SND_GROUP, 1, eSoundHeap_1), TFD_CAST(sid),
#define TFC_LOAD_SND_GROUP_2(sid) TFD_CMD(TF_LOAD_SND_GROUP, 1, eSoundHeap_2), TFD_CAST(sid),
#define TFC_LOAD_SND_GROUP_3(sid) TFD_CMD(TF_LOAD_SND_GROUP, 1, eSoundHeap_3), TFD_CAST(sid),

#define TFC_RESET_SND_GROUP_HEAP_0() TFD_CMD(TF_RESET_SND_GROUP_HEAP, 0, eSoundHeap_0),
#define TFC_RESET_SND_GROUP_HEAP_1() TFD_CMD(TF_RESET_SND_GROUP_HEAP, 0, eSoundHeap_1),
#define TFC_RESET_SND_GROUP_HEAP_2() TFD_CMD(TF_RESET_SND_GROUP_HEAP, 0, eSoundHeap_2),
#define TFC_RESET_SND_GROUP_HEAP_3() TFD_CMD(TF_RESET_SND_GROUP_HEAP, 0, eSoundHeap_3),

// TODO
#define TFC_12F_BEGIN() TFD_CMD(TF_12F, 1, 0), TFD_CAST(1),
#define TFC_12F_END() TFD_CMD(TF_12F, 1, 0), TFD_CAST(0),

#define TFC_CELLANIM_RENDER_EFF_RESET() TFD_CMD(TF_CELLANIM_RENDER_EFF, 1, 0), TFD_CAST(eCellAnimRenderEff_None),
#define TFC_CELLANIM_RENDER_EFF_GRAYSCALE() TFD_CMD(TF_CELLANIM_RENDER_EFF, 1, 0), TFD_CAST(eCellAnimRenderEff_Grayscale),
#define TFC_CELLANIM_RENDER_EFF_INVERT() TFD_CMD(TF_CELLANIM_RENDER_EFF, 1, 0), TFD_CAST(eCellAnimRenderEff_Invert),
#define TFC_CELLANIM_RENDER_EFF_SHOW_EDGE() TFD_CMD(TF_CELLANIM_RENDER_EFF, 1, 0), TFD_CAST(eCellAnimRenderEff_ShowEdge),

#endif
