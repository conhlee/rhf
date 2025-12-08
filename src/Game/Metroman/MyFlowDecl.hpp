#ifndef GAME_METROMAN_MYFLOWDECL_HPP
#define GAME_METROMAN_MYFLOWDECL_HPP

#include <revolution/types.h>

#include "ExFlowDecl.hpp"

enum {
    /*   Metroman MyFlow opcodes (0x200)  */

    TF_METROMAN_INPUT = 0x200,
    TF_METROMAN_GUIDE_PULSE,
    TF_METROMAN_202,
    TF_METROMAN_203,
    TF_METROMAN_204,
    TF_METROMAN_205,
    TF_METROMAN_206,
    TF_METROMAN_207,
};

// command macros

#define TFC_METROMAN_INPUT() TFD_CMD(TF_METROMAN_INPUT, 0, 0),

#endif
