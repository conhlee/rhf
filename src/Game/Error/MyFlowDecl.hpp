#ifndef GAME_ERROR_MYFLOWDECL_HPP
#define GAME_ERROR_MYFLOWDECL_HPP

#include <revolution/types.h>

#include "ExFlowDecl.hpp"

enum {
    /*   Error MyFlow opcodes (0x200)  */

    TF_ERROR_INPUT = 0x200,  ///< Redundant.
};

// command macros

#define TFC_ERROR_INPUT() TFD_CMD(TF_ERROR_INPUT, 0, 0),

#endif
