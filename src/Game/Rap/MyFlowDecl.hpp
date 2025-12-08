#ifndef GAME_RAP_MYFLOWDECL_HPP
#define GAME_RAP_MYFLOWDECL_HPP

#include <revolution/types.h>

#include "ExFlowDecl.hpp"

// TODO: add naming, comments & macros ..

enum {
    /*   Rap MyFlow opcodes (0x200)  */

    TF_RAP_INPUT = 0x200,
    TF_RAP_201,
    TF_RAP_202,
    TF_RAP_203,
    TF_RAP_204,
    TF_RAP_205,
    TF_RAP_206,
    TF_RAP_207,
    TF_RAP_208,
    TF_RAP_209,
    TF_RAP_20A,
    TF_RAP_20B,
    TF_RAP_20C,
};

// command macros

#define TFC_RAP_INPUT() TFD_CMD(TF_RAP_INPUT, 0, 0),

#define TFC_RAP_201_0(a, b, c) TFD_CMD(TF_RAP_201, 3, 0), TFD_CAST(a), TFD_CAST(b), TFD_CAST(c),
#define TFC_RAP_201_1(a) TFD_CMD(TF_RAP_201, 1, 1), TFD_CAST(a),

#define TFC_RAP_20C(a) TFD_CMD(TF_RAP_20C, 0, (a)),

#endif
