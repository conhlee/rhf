#ifndef GAME_PERFECT_MYFLOWDECL_HPP
#define GAME_PERFECT_MYFLOWDECL_HPP

#include <revolution/types.h>

#include "ExFlowDecl.hpp"

enum {
    /*   Logo MyFlow opcodes (0x200)  */

    TF_PERFECT_INPUT = 0x200,  ///< Redundant.
};

// command macros

#define TFC_PERFECT_INPUT() TFD_CMD(TF_PERFECT_INPUT, 0, 0),

#endif
