#include "LayoutAnimation.hpp"

#include <cstdio>

#include "TickFlowManager.hpp"

CLayoutAnimation::CLayoutAnimation(void) {
    mLayout = NULL;
    mPane = NULL;
    mAnimTransform = NULL;

    mIsPlaying = false;
    mAnimationEnable = true;
    mSkipNextUpdate = false;
}

void CLayoutAnimation::fn_801D9E70(
    nw4r::lyt::Layout *layout, nw4r::lyt::Pane *pane,
    const char *animName, nw4r::lyt::ResourceAccessor *resAccessor,
    bool recurseBind
) {
    mLayout = layout;
    if (pane != NULL) {
        mPane = pane;
    }

    mSpeed = 1.0f;
    mReversePlayback = FALSE;

    char animResName[256];
    sprintf(animResName, "%s.brlan", animName);

    const void *animRes = resAccessor->GetResource(0, animResName, NULL);

    mAnimTransform = mLayout->CreateAnimTransform(animRes, resAccessor);
    if (pane != NULL) {
        pane->BindAnimation(mAnimTransform, recurseBind);
    }
    else {
        mLayout->BindAnimation(mAnimTransform);
    }

    mLoop = false;

    fn_801DA66C(0.0f);

    fn_801DA2FC(false);
}

void CLayoutAnimation::fn_801DA094(BOOL reverse) {
    mReversePlayback = reverse;
}

void CLayoutAnimation::fn_801DA09C(BOOL reverse) {
    if (mReversePlayback != reverse) {
        f32 frame = fn_801DA600() - fn_801DA650() - 1.0f;
        fn_801DA66C(frame);
    }

    mReversePlayback = reverse;
}

void CLayoutAnimation::fn_801DA20C(void) {
    fn_801DA66C(0.0f);
}

void CLayoutAnimation::fn_801DA2FC(bool enable) {
    if (mAnimTransform != NULL) {
        mAnimationEnable = enable;
        if (mPane != NULL) {
            mPane->SetAnimationEnable(mAnimTransform, enable, true);
        }
        else {
            mLayout->SetAnimationEnable(mAnimTransform, enable);
        }
    }
}

void CLayoutAnimation::fn_801DA354(void) {
    if (mIsPlaying && (mAnimTransform != NULL)) {
        if (mSkipNextUpdate) {
            mSkipNextUpdate = false;
        }
        else {
            f32 frame;
            if (gTickFlowManager->fn_801E4178()) {
                frame = mFrame + mSpeed;
            }
            else {
                frame = mFrame + (60.0f / 50.0f) * mSpeed;
            }

            while (frame >= fn_801DA600()) {
                if (mLoop) {
                    frame = frame - fn_801DA600();
                }
                else {
                    frame = fn_801DA600() - 1.0;
                    mIsPlaying = false;
                }
            }
            fn_801DA66C(frame);
        }
    }
}

void CLayoutAnimation::fn_801DA590(void) {
    if (mIsPlaying) {
        return;
    }
    if (!mAnimationEnable) {
        return;
    }

    fn_801DA2FC(false);
}

f32 CLayoutAnimation::fn_801DA600(void) {
    if (mAnimTransform != NULL) {
        return mAnimTransform->GetFrameMax();
    }
    else {
        return 0.0f;
    }
}

f32 CLayoutAnimation::fn_801DA650(void) {
    if (mAnimTransform != NULL) {
        return mFrame;
    }
    else {
        return 0.0f;
    }
}

void CLayoutAnimation::fn_801DA66C(f32 frame) {
    if (mAnimTransform != NULL) {
        fn_801DA2FC(true);

        mFrame = frame;

        f32 trueFrame;
        if (mReversePlayback == FALSE) {
            trueFrame = static_cast<f64>(frame);
        }
        else {
            trueFrame = mAnimTransform->GetFrameMax() - mFrame - 1.0;
        }

        mAnimTransform->SetFrame(MAX(0.0f, trueFrame));
    }
}

void CLayoutAnimation::fn_801DA770(void) {
    fn_801DA66C(fn_801DA600());
}
