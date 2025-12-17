#include "CursorLayout.hpp"

#include <cstdio>

#include "LayoutManager.hpp"

#include "HBMManager.hpp"

#include "Controller.hpp"

static const char layoutFile_P1_Def[] = "P1_Def.brlyt";

DECL_SECTION(".sdata") static const char * const layoutFileTable[] = { layoutFile_P1_Def, NULL };
// TODO: hack!

void CCursorLayout::_10(void) {
    static char paneNameBuf[16];

    nw4r::lyt::MultiArcResourceAccessor *resAccessor = gLayoutManager->getResAccessor();

    u32 layoutBinSize;
    void *layoutBin = resAccessor->GetResource(0, layoutFileTable[0], &layoutBinSize);

    buildLayout(layoutBin, resAccessor);

    for (s32 i = 0; i < CURSOR_COUNT; i++) {
        sprintf(paneNameBuf, "N%d_Trans", i + 1);
        mPaneTrans[i] = getLayout()->GetRootPane()->FindPaneByName(paneNameBuf);

        sprintf(paneNameBuf, "N%d_SRot", i + 1);
        mPaneSRot[i] = getLayout()->GetRootPane()->FindPaneByName(paneNameBuf);

        sprintf(paneNameBuf, "N%d_Rot", i + 1);
        mPaneRot[i] = getLayout()->GetRootPane()->FindPaneByName(paneNameBuf);
    }

    this->CLayout::_10();

    mUnk34[0] = 1; // 34
    mUnk34[1] = 1; // 35
    mUnk34[3] = 1; // 37
    mUnk34[5] = 1; // 39
    mUnk34[2] = 1; // 36
    mUnk34[4] = 1; // 38
    mUnk34[6] = 1; // 3A

    mPaneTrans[0]->SetVisible(true);
    for (s32 i = 1; i < CURSOR_COUNT; i++) {
        mPaneTrans[i]->SetVisible(false);
    }
}

extern Vec2 lbl_80320FA0;

void CCursorLayout::_18(nw4r::lyt::DrawInfo *drawInfo) {
    for (s32 i = 0; i < CURSOR_COUNT; i++) {
        if (mPaneTrans[i]->IsVisible()) {
            CController *controller = gControllerManager->fn_801D5FF0(i);

            if (mUnk34[1 + i]) {
                if (controller->fn_801D52D4()) {
                    mPaneTrans[i]->SetTranslate(nw4r::math::VEC2(
                        controller->fn_801D523C(this).x,
                        controller->fn_801D523C(this).y
                    ));
                }
                else {
                    mPaneTrans[i]->SetTranslate(nw4r::math::VEC2(
                        lbl_80320FA0.x, lbl_80320FA0.y
                    ));
                }
            }

            if (mUnk34[3 + i]) {
                if (controller->fn_801D52D4()) {
                    f32 rot = controller->fn_801D547C();

                    mPaneSRot[i]->SetRotate(nw4r::math::VEC3(
                        0.0f, 0.0f, -rot
                    ));
                    mPaneRot[i]->SetRotate(nw4r::math::VEC3(
                        0.0f, 0.0f, -rot
                    ));
                }
            }

            if (mUnk34[5 + i]) {
                mPaneTrans[i]->SetScale(nw4r::math::VEC2(1.0f, 1.0f));
            }
        }
    }

    this->CLayout::_18(drawInfo);
}

void CCursorLayout::_1C(nw4r::lyt::DrawInfo *drawInfo) {
    if (gHBMManager->getUnk414()) {
        return;
    }

    if (!mUnk34[0]) {
        return;
    }

    this->CLayout::_1C(drawInfo);
}

void CCursorLayout::fn_8003A898(s32 cursorIndex, bool visible) {
    mPaneTrans[(u8)cursorIndex]->SetVisible(visible);
}
