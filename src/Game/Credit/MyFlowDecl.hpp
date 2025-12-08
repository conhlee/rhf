#ifndef GAME_CREDIT_MYFLOWDECL_HPP
#define GAME_CREDIT_MYFLOWDECL_HPP

#include <revolution/types.h>

#include "ExFlowDecl.hpp"

enum {
    /*   Credit MyFlow opcodes (0x200 to 0x208)  */

    TF_CREDIT_INPUT = 0x200,        ///< Redundant.
    TF_CREDIT_NEXT_CHARA,           ///< Advance the screen to the next character.
                                    ///      - Args: time in ticks
    TF_CREDIT_SET_TEXT_POS,         ///< Set a text line's X and Y position coordinates.
                                    ///      - Args: text line index, X position (pixels), Y position (pixels)
    TF_CREDIT_CHARA_BEAT,           ///< Play the current character's beat animation.
    TF_CREDIT_MASTER_ANIM,          ///< Control the master's animation.
                                    ///      - Arg0: is noticing the mascots;
                                    ///      - Args: type: 0 for beat with master & chara 0, 1 for beat with master, and 2 for beat with master looking to mascots
    TF_CREDIT_MASCOT_ANIM,          ///< Control a mascot's / all mascots' animation.
                                    ///      - Arg0: type: 0 for appear (args will be: mascot index, start Y, end Y, time in ticks),
                                    ///                    1 for beat (args will be: mascot index, times to play beat animation), and
                                    ///                    2 for all beat (no args).
    TF_CREDIT_WIPE_ANIM,            ///< Control the wipe overlay's animation.
                                    ///      - Arg0: type: 0 for show, 1 for hide and 2 for mascot speaking (args will be: times to repeat anim).
                                    ///      - Args: animation variation with mascot
    TF_CREDIT_CHECK_NIGHTWALK_VER,  ///< Check Night Walk's version (?) TODO: what and why?
    TF_CREDIT_TITLE_CLOSE,          ///< Fade the title card out.
};

// command macros

#define TFC_CREDIT_INPUT() TFD_CMD(TF_CREDIT_INPUT, 0, 0),

#define TFC_CREDIT_NEXT_CHARA(timeTicks) TFD_CMD(TF_CREDIT_NEXT_CHARA, 1, 0), TFD_CAST(timeTicks),

#define TFC_CREDIT_SET_TEXT_POS(line, x, y) TFD_CMD(TF_CREDIT_SET_TEXT_POS, 3, 0), \
                                            TFD_CAST(line), TFD_CAST(x), TFD_CAST(y),

#define TFC_CREDIT_CHARA_BEAT() TFD_CMD(TF_CREDIT_CHARA_BEAT, 0, 0),

#define TFC_CREDIT_MASTER_BEAT_FIRST() TFD_CMD(TF_CREDIT_MASTER_ANIM, 1, 0), 0,
#define TFC_CREDIT_MASTER_BEAT() TFD_CMD(TF_CREDIT_MASTER_ANIM, 1, 0), 1,
#define TFC_CREDIT_MASTER_NOTICE_MASCOT() TFD_CMD(TF_CREDIT_MASTER_ANIM, 0, 1),
#define TFC_CREDIT_MASTER_BEAT_LOOK_MASCOT() TFD_CMD(TF_CREDIT_MASTER_ANIM, 1, 0), 2,

#define TFC_CREDIT_MASCOT_APPEAR(mascotIndex, startY, endY, timeTicks) TFD_CMD(TF_CREDIT_MASCOT_ANIM, 4, 0), \
                                              TFD_CAST(mascotIndex), TFD_CAST(startY), TFD_CAST(endY), \
                                              TFD_CAST(timeTicks),
#define TFC_CREDIT_MASCOT_BEAT(mascotIndex, repeatCount) TFD_CMD(TF_CREDIT_MASCOT_ANIM, 2, 1), \
                                                         TFD_CAST(mascotIndex), TFD_CAST(repeatCount),
#define TFC_CREDIT_MASCOT_ALL_BEAT() TFD_CMD(TF_CREDIT_MASCOT_ANIM, 0, 2),

#define TFC_CREDIT_WIPE_SHOW() TFD_CMD(TF_CREDIT_WIPE_ANIM, 1, 0), 0,
#define TFC_CREDIT_WIPE_HIDE() TFD_CMD(TF_CREDIT_WIPE_ANIM, 1, 1), 0,

#define TFC_CREDIT_WIPE_SHOW_MASCOT() TFD_CMD(TF_CREDIT_WIPE_ANIM, 1, 0), 1,
#define TFC_CREDIT_WIPE_HIDE_MASCOT() TFD_CMD(TF_CREDIT_WIPE_ANIM, 1, 1), 1,
#define TFC_CREDIT_WIPE_SPEAK_MASCOT(repeatCount) TFD_CMD(TF_CREDIT_WIPE_ANIM, 1, 2), TFD_CAST(repeatCount),

#define TFC_CREDIT_CHECK_NIGHTWALK_VER() TFD_CMD(TF_CREDIT_CHECK_NIGHTWALK_VER, 0, 0),

#define TFC_CREDIT_TITLE_CLOSE() TFD_CMD(TF_CREDIT_TITLE_CLOSE, 0, 0),

#endif
