#ifndef GAME_AGBGHOST_MYFLOWDECL_HPP
#define GAME_AGBGHOST_MYFLOWDECL_HPP

#include <revolution/types.h>

#include "ExFlowDecl.hpp"

enum {
    /*   AgbGhost MyFlow opcodes (0x200)  */

    TF_AGBGHOST_INPUT = 0x200,  ///< Register an input in 1 beat.
                                ///      - Args: leniancy: 0 for normal (-10, -6, 6, 10),
                                ///                        1 for relaxed (-14, -8, 8, 14),
                                ///                        2 for very relaxed (-18, -12, 12, 18),
                                ///                        3 for precise (-10, -4, 0, 0).
    TF_AGBGHOST_BOWARROW,       ///< Control the bow & arrow.
                                ///      - Arg0: operation: 0 for drawing the bow & arrow,
                                ///                         1 for sliding it in from offscreen (args will be: start X position, time in ticks).
    TF_AGBGHOST_GHOST_ANIM,     ///< Control the ghost's animation.
                                ///      - Arg0: animation: 0 for step (args will be: position),
                                ///                         1 for timing instruction, 2 for dash.
    TF_AGBGHOST_DOOR_ANIM,      ///< Control the door's animation.
                                ///      - Arg0: animation: 0 for close.
    TF_AGBGHOST_GHOST_HEIGHT,   ///< TODO
    TF_AGBGHOST_RAIN,           ///< TODO
};

// command macros

// TODO: better naming
#define TFC_AGBGHOST_INPUT_NORMAL() TFD_CMD(TF_AGBGHOST_INPUT, 1, 0), TFD_CAST(0),
#define TFC_AGBGHOST_INPUT_RELAXED() TFD_CMD(TF_AGBGHOST_INPUT, 1, 0), TFD_CAST(1),
#define TFC_AGBGHOST_INPUT_VERYRELAXED() TFD_CMD(TF_AGBGHOST_INPUT, 1, 0), TFD_CAST(2),
#define TFC_AGBGHOST_INPUT_PRECISE() TFD_CMD(TF_AGBGHOST_INPUT, 1, 0), TFD_CAST(3),

#define TFC_AGBGHOST_BOWARROW_DRAW() TFD_CMD(TF_AGBGHOST_BOWARROW, 0, 0),
#define TFC_AGBGHOST_BOWARROW_SLIDE_IN(startX, ticks) TFD_CMD(TF_AGBGHOST_BOWARROW, 2, 1), TFD_CAST(startX), TFD_CAST(ticks),

#define TFC_AGBGHOST_GHOST_ANIM_STEP(pos) TFD_CMD(TF_AGBGHOST_GHOST_ANIM, 1, 0), TFD_CAST(pos),
#define TFC_AGBGHOST_GHOST_ANIM_TIMING_INST() TFD_CMD(TF_AGBGHOST_GHOST_ANIM, 0, 1),
#define TFC_AGBGHOST_GHOST_ANIM_TIMING_DASH() TFD_CMD(TF_AGBGHOST_GHOST_ANIM, 0, 2),

#define TFC_AGBGHOST_DOOR_ANIM_CLOSE() TFD_CMD(TF_AGBGHOST_DOOR_ANIM, 0, 0),

#define TFC_AGBGHOST_GHOST_HEIGHT(height) TFD_CMD(TF_AGBGHOST_GHOST_HEIGHT, 1, 0), TFD_CAST(height),
#define TFC_AGBGHOST_GHOST_HEIGHT_RESET() TFC_AGBGHOST_GHOST_HEIGHT(256)

#define TFC_AGBGHOST_RAIN_SOUND_PLAY() TFD_CMD(TF_AGBGHOST_RAIN, 1, 0), TFD_CAST(0),
#define TFC_AGBGHOST_RAIN_SOUND_PAUSE() TFD_CMD(TF_AGBGHOST_RAIN, 1, 0), TFD_CAST(1),

#define TFC_AGBGHOST_RAIN_FALL_NORMAL() TFD_CMD(TF_AGBGHOST_RAIN, 1, 1), TFD_CAST(0),
#define TFC_AGBGHOST_RAIN_FALL_SLOW() TFD_CMD(TF_AGBGHOST_RAIN, 1, 1), TFD_CAST(1),

#define TFC_AGBGHOST_RAIN_SET_PAUSE_ON_SLOW() TFD_CMD(TF_AGBGHOST_RAIN, 1, 2), TFD_CAST(0),
#define TFC_AGBGHOST_RAIN_SET_MOTION_ON_SLOW() TFD_CMD(TF_AGBGHOST_RAIN, 1, 2), TFD_CAST(1),

#endif
