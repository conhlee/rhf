#include "Layout.hpp"

#include "Mem.hpp"

#include "LayoutManager.hpp"

// TODO: match https://decomp.me/scratch/EEu7e
bool CBtnPane::fn_801D85BC(nw4r::lyt::Pane *pane, nw4r::lyt::DrawInfo *drawInfo) {
    if (!drawInfo->IsLocationAdjust()) {
        return false;
    }
    else {
        nw4r::lyt::Pane *parent = pane->GetParent();
        if (parent == NULL) {
            return pane->IsLocationAdjust();
        }
        else {
            if (!pane->IsLocationAdjust()) {
                return fn_801D85BC(parent, drawInfo);
            }
            return false;
        }
    }
}

CLayout::CLayout(void) {
    mAnimationCount = 0;
    mLayout = NULL;
    mAnimations = NULL;
}

void CLayout::_0C(void) {
    mLayout = new nw4r::lyt::Layout;
    mAnimations = new CLayoutAnimation[mAnimationCount];

    mUpdateEnable = true;
    mDrawEnable = true;
}

void CLayout::_10(void) {
    mApplyManagerScale = true;
    mScaleX = 1.0f;
    mScaleY = 1.0f;
}

void CLayout::_14(void) {
    if (mLayout != NULL) {
        delete mLayout;
    }
    if (mAnimations != NULL) {
        delete[] mAnimations;
    }
}

void CLayout::_18(nw4r::lyt::DrawInfo *drawInfo) {
    f32 scaleX = mScaleX;
    f32 scaleY = mScaleY;

    if (mApplyManagerScale) {
        scaleX *= gLayoutManager->fn_801D7188();
        scaleY *= gLayoutManager->fn_801D7190();
    }

    mLayout->GetRootPane()->SetScale(nw4r::math::VEC2(scaleX, scaleY));

    _20(drawInfo);

    for (s32 i = 0; i < mAnimationCount; i++) {
        mAnimations[i].fn_801DA354();
    }

    mLayout->Animate(0);
    mLayout->CalculateMtx(*drawInfo);

    for (s32 i = 0; i < mAnimationCount; i++) {
        mAnimations[i].fn_801DA590();
    }
}

void CLayout::_1C(nw4r::lyt::DrawInfo *drawInfo) {
    _20(drawInfo);

    mLayout->Draw(*drawInfo);
}

void CLayout::_20(nw4r::lyt::DrawInfo *drawInfo) {
    nw4r::ut::Rect viewRect = mLayout->GetLayoutRect();

    f32 nearField = 0.0f;
    f32 farField = 500.0f;
    
    Mtx44 projMtx;
    C_MTXOrtho(projMtx, viewRect.top, viewRect.bottom, viewRect.left, viewRect.right, nearField, farField);

    GXSetProjection(projMtx, GX_ORTHOGRAPHIC);

    nw4r::math::MTX34 viewMtx;
    PSMTXIdentity(viewMtx.mtx);

    drawInfo->SetViewMtx(viewMtx);
    drawInfo->SetViewRect(viewRect);
}

void CLayout::fn_801D9E24(bool enable) {
    mApplyManagerScale = enable;
}
