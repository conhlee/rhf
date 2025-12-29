#include "ModeChangeLayout.hpp"

#include "LayoutManager.hpp"

#include "MessageManager.hpp"

const char layoutFile_mode_change[] = "mode_change.brlyt";

static const char *layoutFileTable[] = { layoutFile_mode_change, NULL };

void CModeChangeLayout::_10(void) {
    nw4r::lyt::MultiArcResourceAccessor *resAccessor = gLayoutManager->getResAccessor();

    u32 layoutBinSize;
    void *layoutBin = resAccessor->GetResource(0, layoutFileTable[0], &layoutBinSize);

    buildLayout(layoutBin, resAccessor);

    nw4r::lyt::Pane *paneChangeKey = getLayout()->GetRootPane()->FindPaneByName("N_change_key_00");
    nw4r::lyt::Pane *paneChangeDPD = getLayout()->GetRootPane()->FindPaneByName("N_change_DPD_00");

    getAnimation(0)->fn_801D9E70(getLayout(), paneChangeKey, "mode_change_change_key", resAccessor);
    getAnimation(1)->fn_801D9E70(getLayout(), paneChangeDPD, "mode_change_change_DPD", resAccessor);

    gMessageManager->fn_80088474(getLayout()->GetRootPane());

    this->CLayout::_10();
}

void CModeChangeLayout::fn_800833E4(void) {
    getAnimation(0)->playFromBeginForward();

    if (getAnimation(1)->getIsPlaying()) {
        getAnimation(1)->playFromBeginReverse();
        getAnimation(1)->fn_801DA770();
    }
}

void CModeChangeLayout::fn_8008347C(void) {
    getAnimation(1)->playFromBeginForward();

    if (getAnimation(0)->getIsPlaying()) {
        getAnimation(0)->playFromBeginReverse();
        getAnimation(0)->fn_801DA770();
    }
}

void CModeChangeLayout::_14(void) {
    this->CLayout::_14();
}

CModeChangeLayout::~CModeChangeLayout(void) {
    _14();
}
