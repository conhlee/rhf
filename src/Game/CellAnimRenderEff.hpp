#ifndef GAME_CELLANIM_RENDER_EFF_HPP
#define GAME_CELLANIM_RENDER_EFF_HPP

#include <revolution/types.h>

#include "CellAnimManager.hpp"

enum ECellAnimRenderEff {
    eCellAnimRenderEff_None,
    eCellAnimRenderEff_Grayscale,
    eCellAnimRenderEff_Invert,
    eCellAnimRenderEff_ShowEdge,

    eCellAnimRenderEff_5 = 5 // ???
};

ECellAnimRenderEff fn_8009E1B4(ECellAnimRenderEff type);
ECellAnimRenderEff fn_8009E1C4(void); // set eCellAnimRenderEff_None
ECellAnimRenderEff fn_8009E1D4(void); // set eCellAnimRenderEff_Grayscale
ECellAnimRenderEff fn_8009E1E4(void); // set eCellAnimRenderEff_Invert
ECellAnimRenderEff fn_8009E1F4(void); // set eCellAnimRenderEff_ShowEdge

CCellAnimManager::DrawSetupFn fn_8009E204(void);
CCellAnimManager::DrawRestoreFn fn_8009E260(void);

#endif
