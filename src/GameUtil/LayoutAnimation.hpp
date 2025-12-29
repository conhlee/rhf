#ifndef GAMEUTIL_LAYOUTANIMATION_HPP
#define GAMEUTIL_LAYOUTANIMATION_HPP

#include <revolution/types.h>

#include <nw4r/lyt.h>

class CLayoutAnimation {
public:
    virtual ~CLayoutAnimation(void) {}

    CLayoutAnimation(void);

    void fn_801D9E70(
        nw4r::lyt::Layout *layout, nw4r::lyt::Pane *pane,
        const char *animName, nw4r::lyt::ResourceAccessor *resAccessor,
        bool recurseBind = true
    );

    void fn_801DA094(BOOL reverse);
    void fn_801DA09C(BOOL reverse);

    void fn_801DA20C(void);

    void fn_801DA2FC(bool enable);

    void fn_801DA354(void);

    void fn_801DA590(void);

    f32 fn_801DA600(void);
    f32 fn_801DA650(void);

    void fn_801DA66C(f32 frame);

    void fn_801DA770(void);

    bool getIsPlaying(void) const { return mIsPlaying; }
    void setIsPlaying(bool isPlaying) { mIsPlaying = isPlaying; }

    void setSkipNextUpdate(bool skipNextUpdate) { mSkipNextUpdate = skipNextUpdate; }

    f32 getSpeed(void) const { return mSpeed; }
    void setSpeed(f32 speed) { mSpeed = speed; }

    bool getLoop(void) const { return mLoop; }
    void setLoop(bool loop) { mLoop = loop; }

    void playFromBeginning(void) {
        fn_801DA20C();
        setIsPlaying(true);
        setSkipNextUpdate(true);
    }

    void playFromBeginForward(void) {
        fn_801DA094(FALSE);
        playFromBeginning();
    }
    void playFromBeginReverse(void) {
        fn_801DA094(TRUE);
        playFromBeginning();
    }

private:
    nw4r::lyt::Layout *mLayout;
    nw4r::lyt::Pane *mPane;
    nw4r::lyt::AnimTransform *mAnimTransform;
    f32 mFrame;
    f32 mSpeed;
    bool mIsPlaying;
    bool mLoop;
    bool mAnimationEnable;
    bool mSkipNextUpdate;
    BOOL mReversePlayback;
};

#endif