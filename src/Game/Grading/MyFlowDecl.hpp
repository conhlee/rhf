#ifndef GAME_GRADING_MYFLOWDECL_HPP
#define GAME_GRADING_MYFLOWDECL_HPP

#include <revolution/types.h>

#include "ExFlowDecl.hpp"

enum {
    /*   Grading MyFlow opcodes (0x200 to 0x204)  */

    TF_GRADING_SHOW_CAPTION = 0x200,    ///< Display the caption.
    TF_GRADING_SHOW_COMMENT,            ///< Display the comment.
    TF_GRADING_SHOW_GRADE,              ///< Display the grade & play the associated sound effect.
    TF_GRADING_CHECK_FOLLOW,            ///< Set condvar to TRUE if follow text exists, and FALSE if not.
    TF_GRADING_SHOW_FOLLOW,             ///< Display the follow text.
};

#define TFC_GRADING_SHOW_CAPTION() TFD_CMD(TF_GRADING_SHOW_CAPTION, 0, 0),
#define TFC_GRADING_SHOW_COMMENT() TFD_CMD(TF_GRADING_SHOW_COMMENT, 0, 0),
#define TFC_GRADING_SHOW_GRADE() TFD_CMD(TF_GRADING_SHOW_GRADE, 0, 0),
#define TFC_GRADING_CHECK_FOLLOW() TFD_CMD(TF_GRADING_CHECK_FOLLOW, 0, 0),
#define TFC_GRADING_SHOW_FOLLOW() TFD_CMD(TF_GRADING_SHOW_FOLLOW, 0, 0),

#endif
