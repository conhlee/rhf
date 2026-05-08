#ifndef GAME_EXFLOW_HPP
#define GAME_EXFLOW_HPP

#include <revolution/types.h>

#include "TickFlow.hpp"

TFD_EXTERN(lbl_80255990)
TFD_EXTERN(lbl_802559B0)

class CExFlow : public CTickFlow {
public:
    virtual ~CExFlow(void);
    virtual void _14(void);
    virtual bool _1C(u32 opcode, u32 arg0, const s32 *args);

    CExFlow(const TickFlowCode *code, f32 initRest);

    TICKFLOW_DECL_CREATE_FN()

    static void fn_8008E724(bool);
};

#endif
