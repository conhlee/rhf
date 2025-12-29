#include "Credit/MyLayout.hpp"

#include "LayoutManager.hpp"

#include "MessageManager.hpp"

#include "TickFlow.hpp"

#include <cstdio>

const char layoutFile_credit[] = "credit.brlyt";

DECL_SECTION(".sdata") const char * const layoutFileTable[] = { layoutFile_credit, NULL };
// TODO: ^^ hack!

char sPaneNameBuf[16];

void Credit::CMyLayout::_10(void) {
    nw4r::lyt::MultiArcResourceAccessor *resAccessor = gLayoutManager->getResAccessor();

    u32 layoutBinSize;
    void *layoutBin = gLayoutManager->getResAccessor()->GetResource(0, layoutFileTable[0], &layoutBinSize);

    buildLayout(layoutBin, resAccessor);

    gMessageManager->fn_80088474(getLayout()->GetRootPane());

    for (u8 i = 0; i < 4; i++) {
        sprintf(sPaneNameBuf, "N_name_%02d", i);
        mPanesN[i] = getLayout()->GetRootPane()->FindPaneByName(sPaneNameBuf);

        sprintf(sPaneNameBuf, "T_name_%02d", i);
        mPanesT[i] = static_cast<nw4r::lyt::TextBox *>(
            getLayout()->GetRootPane()->FindPaneByName(sPaneNameBuf)
        );

        CTickFlow::fn_801DEF58(i + 1, mPanesT[i], NULL);
    }

    this->CLayout::_10();
}

void Credit::CMyLayout::_14(void) {
    for (u8 i = 0; i < 4; i++) {
        CTickFlow::fn_801DEF58(i + 1, NULL, NULL);
    }
    this->CLayout::_14();
}

void Credit::CMyLayout::fn_800C416C(s32 index, f32 x, f32 y) {
    mPanesN[index]->SetTranslate(nw4r::math::VEC2(x, -y));
}

Credit::CMyLayout::~CMyLayout(void) {
    _14();
}
