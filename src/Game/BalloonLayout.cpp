#include "BalloonLayout.hpp"

#include "LayoutManager.hpp"

#include "MessageManager.hpp"

#include <revolution/SC.h>

const char layoutFile_message[] = "message.brlyt";

const char *layoutFileTable[] = { layoutFile_message, NULL };

void CBalloonLayout::fn_800399E4(void) {
    nw4r::lyt::MultiArcResourceAccessor *resAccessor = gLayoutManager->getResAccessor();

    u32 layoutBinSize;
    void *layoutBin = resAccessor->GetResource(0, layoutFileTable[0], &layoutBinSize);

    buildLayout(layoutBin, resAccessor);

    mBoxParent = getLayout()->GetRootPane()->FindPaneByName("N_message_00");
    mBoxTriangleStraight = getLayout()->GetRootPane()->FindPaneByName("P_triangle_00");
    mBoxTriangleCurve = getLayout()->GetRootPane()->FindPaneByName("P_triangle_01");
    mBoxWindow = getLayout()->GetRootPane()->FindPaneByName("W_message_00");
    mBoxTextbox = static_cast<nw4r::lyt::TextBox *>(
        getLayout()->GetRootPane()->FindPaneByName("T_message_00", true)
    );
    mBoxTextbox->SetTextPositionH(nw4r::lyt::HORIZONTALPOSITION_LEFT);

    mBoxTextTagProcessor = new CMyTagProcessor;
    mBoxTextbox->SetTagProcessor(mBoxTextTagProcessor);

    mOverlayParent = getLayout()->GetRootPane()->FindPaneByName("N_message_01");
    mOverlayPane = getLayout()->GetRootPane()->FindPaneByName("P_message_01");
    mOverlayTextbox = static_cast<nw4r::lyt::TextBox *>(
        getLayout()->GetRootPane()->FindPaneByName("T_message_01")
    );
    mOverlayTextbox->SetTextPositionH(nw4r::lyt::HORIZONTALPOSITION_LEFT);

    mOverlayTextTagProcessor = new CMyTagProcessor;
    mOverlayTextbox->SetTagProcessor(mOverlayTextTagProcessor);

    gMessageManager->fn_80088474(getLayout()->GetRootPane());
    
    this->CLayout::_10();

    mBoxPaneAccessIdx = -1;
    mOverlayPaneAccessIdx = -1;

    mBoxParent->SetTranslate(nw4r::math::VEC2(getScaledX(0.0f), 100.0f));
    mBoxWindow->SetSize(nw4r::lyt::Size(400.0f, 160.0f));
    mBoxTextbox->SetSize(nw4r::lyt::Size(400.0f - 60.0f, 160.0f));

    mOverlayPane->SetVisible(false);
    mOverlayTextbox->SetVisible(false);

    mOverlayParent->SetTranslate(nw4r::math::VEC2(0.0f, 100.0f));
}
