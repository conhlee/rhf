#include "Layout.hpp"

#include "Mem.hpp"

#include "LayoutManager.hpp"

#include "Controller.hpp"

#include "Sound.hpp"

nw4r::math::VEC2 lbl_80320FA0 (-9999.0f, -9999.0f);

bool CBtnPane::fn_801D85BC(nw4r::lyt::Pane *pane, nw4r::lyt::DrawInfo *drawInfo) {
    if (!drawInfo->IsLocationAdjust()) {
        return false;
    }
    
    nw4r::lyt::Pane *parent = pane->GetParent();
    if (parent == NULL) {
        return pane->IsLocationAdjust();
    }
    
    return pane->IsLocationAdjust() || fn_801D85BC(parent, drawInfo);
}

nw4r::math::VEC2 CBtnPane::fn_801D86D0(nw4r::lyt::Pane *pane, nw4r::lyt::DrawInfo *drawInfo) {
    nw4r::math::VEC2 vec (0.0f, 0.0f);

    for (; pane != NULL; pane = pane->GetParent()) {
        nw4r::lyt::Pane *parent = pane->GetParent();
        if ((parent != NULL) && fn_801D85BC(parent, drawInfo)) {
            vec.x += pane->GetTranslate().x * drawInfo->GetLocationAdjustScale().x;
            vec.y += pane->GetTranslate().y;
        }
        else {
            vec.x += pane->GetTranslate().x;
            vec.y += pane->GetTranslate().y;
        }
    }
    
    return vec;
}

nw4r::math::VEC3 CBtnPane::fn_801D887C(nw4r::lyt::Pane *pane, nw4r::lyt::DrawInfo *drawInfo) {
    nw4r::math::VEC2 vec = fn_801D86D0(pane, drawInfo);
    return nw4r::math::VEC3(vec.x, vec.y, 0.0f);
}

f32 fn_801D8A5C(nw4r::lyt::TextBox *textBox, f32 fontWidth, f32 fontHeight) {
    nw4r::ut::WideTextWriter textWriter;

    textWriter.SetFont(*textBox->GetFont());
    textWriter.SetFontSize(fontWidth, fontHeight);
    textWriter.SetLineSpace(textBox->GetLineSpace());
    textWriter.SetCharSpace(textBox->GetCharSpace());

    f32 strWidth = textWriter.CalcStringWidth(textBox->GetString());

    f32 scaleH;
    if (strWidth > textBox->GetSize().width) {
        scaleH = (textBox->GetSize().width * 0.95f) / strWidth;
    }
    else {
        scaleH = 1.0f;
    }

    fontWidth *= scaleH;
    textBox->SetFontSize(nw4r::lyt::Size(fontWidth, fontHeight));

    return scaleH;
}


void CExBtnPane::fn_801D8FFC(nw4r::lyt::Pane *pane) {
    mPane = pane;
    mDefaultAnime = NULL;
    mFocusAnime = NULL;
    mActionAnime = NULL;
    mEnabled = true;
    mUnk25 = false;
    mUnk28 = true;
    _10(eBtnState_Default, true);
    mUnk18 = 0;
    mVibrateOnHover = true;
    mSelectSfxID = -1;
    mActionSfxID = -1;
    mControllerChan = 0;
    mUnk26 = false;
    mUnk27 = false;
}

EBtnState CExBtnPane::fn_801D9090(nw4r::math::VEC2 *cursorPos, bool arg1, nw4r::lyt::DrawInfo *drawInfo) {
    mUnk26 = false;
    mUnk27 = false;

    if (!mEnabled) {
        return eBtnState_Default;
    }

    nw4r::math::VEC2 posVec = (cursorPos != NULL) ? *cursorPos : nw4r::math::VEC2(-9999.0f, -9999.0f);
    nw4r::ut::Rect paneRect = calcRect(drawInfo);

    bool hover = (paneRect.left <= posVec.x)   && (posVec.x <= paneRect.right) &&
                 (paneRect.bottom <= posVec.y) && (posVec.y <= paneRect.top);

    switch (mState) {
    case eBtnState_Default: {
        if (hover && !isFocusPlaying()) {
            _10(eBtnState_Selected, true);
            return eBtnState_Selected;
        }

        if ((mDefaultAnime != NULL) && (!mDefaultAnime->getIsPlaying())) {
            if (!isFocusPlaying()) {
                mDefaultAnime->playFromBeginning();
            }
        }
    } break;
    
    case eBtnState_Selected: {
        if (!hover && !isFocusPlaying()) {
            _10(eBtnState_Default, true);
            break;
        }
        if (hover && arg1 && !isActionPlaying()) {
            _10(eBtnState_Active, true);
            mUnk26 = true;
            return eBtnState_Active;
        }
    } break;
    
    case eBtnState_Active: {
        if (!isActionPlaying()) {
            _10(eBtnState_Unk03, true);
            mUnk27 = true;
            return eBtnState_Unk03;
        }
    } break;
    
    case eBtnState_Unk03: {
        if (hover && mUnk25) {
            s32 selectSfxhover;
            bool vibratehover;

            vibratehover = mVibrateOnHover;
            selectSfxhover = mSelectSfxID;
            
            mVibrateOnHover = false;
            mSelectSfxID = -1;
            _10(eBtnState_Selected, true);
            mVibrateOnHover = vibratehover;
            mSelectSfxID = selectSfxhover;
            return eBtnState_Selected;
        }
        _10(eBtnState_Default, true);
    } break;
    }

    return eBtnState_Default;
}

void CExBtnPane::_10(EBtnState state, bool arg1) {
    if (!mEnabled) {
        return;
    }

    if (!arg1 && (mState == state)) {
        return;
    }

    EBtnState prevState = mState;
    mState = state;

    switch (state) {
    case eBtnState_Default: {
        if ((prevState == eBtnState_Selected) && (mFocusAnime != NULL)) {
            if (!mFocusAnime->getIsPlaying()) {
                mFocusAnime->playFromBeginReverse();
            }
            else {
                mFocusAnime->fn_801DA09C(TRUE);
                mFocusAnime->setIsPlaying(true);
            }
        }
    } break;
    
    case eBtnState_Selected: {
        if (
            ((prevState == eBtnState_Default) || ((prevState == eBtnState_Unk03) && mUnk28)) && 
            (mFocusAnime != NULL)
        ) {
            if (mDefaultAnime != NULL) {
                mDefaultAnime->setIsPlaying(false);
            }
            mFocusAnime->playFromBeginForward();
            fn_801D9974();
        }
        if (mVibrateOnHover) {
            gControllerManager->fn_801D5FF0(mControllerChan)->_40("****--------", false);
        }
        if (mSelectSfxID != -1) {
            gSoundManager->play(mSelectSfxID);
        }
    } break;
    
    case eBtnState_Active: {
        if (mActionAnime != NULL) {
            mActionAnime->playFromBeginForward();
        }
        if (mActionSfxID != -1) {
            gSoundManager->play(mActionSfxID);
        }
    } break;

    case eBtnState_Unk03:
        break;
    }
}

CW_FORCE_STRINGS(GameUtil_Layout_cpp, "LYT_ANIM");

void CExBtnPane::_14(void) {
    _10(eBtnState_Default, true);
    if (mFocusAnime != NULL) {
        mFocusAnime->fn_801DA094(FALSE);
        mFocusAnime->setIsPlaying(false);
        mFocusAnime->fn_801DA20C();
    }
}

void CExBtnPane::fn_801D9974(void) {
    nw4r::lyt::Pane *pane = mPane;
    s32 unk18 = mUnk18;
    for (; (unk18 > 0) && (pane != NULL); pane = pane->GetParent()) {
        nw4r::lyt::Pane::PaneList &parentChildList = pane->GetParent()->GetChildList();
        parentChildList.Erase(pane);
        parentChildList.PushBack(pane);
        unk18--;
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
    C_MTXOrtho(projMtx,
        viewRect.top, viewRect.bottom, viewRect.left, viewRect.right,
        nearField, farField
    );

    GXSetProjection(projMtx, GX_ORTHOGRAPHIC);

    nw4r::math::MTX34 viewMtx;
    MTXIdentity(viewMtx.mtx);

    drawInfo->SetViewMtx(viewMtx);
    drawInfo->SetViewRect(viewRect);
}

void CLayout::fn_801D9E24(bool enable) {
    mApplyManagerScale = enable;
}
