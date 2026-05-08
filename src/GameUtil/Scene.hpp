#ifndef GAMEUTIL_SCENE_HPP
#define GAMEUTIL_SCENE_HPP

#include <revolution/types.h>

#include "Mem.hpp"

#define SCENE_DECL_CREATE_FN() static CScene *create(u16 memGroup);
#define SCENE_IMPL_CREATE_FN(_className)        \
    CScene *_className::create(u16 memGroup) {  \
        memPushGroup(memGroup);                 \
        _className *scene = new _className;     \
        memPopGroup();                          \
        scene->setMemGroup(memGroup);           \
        return scene;                           \
    }

class CScene {
public:
    typedef CScene *(*CreateFn)(u16 memGroup);
    enum EState {
        eState_Initial,         // Scene object was just created
        eState_Loading,         // Scene object is busy loading assets
        eState_Ready,           // Scene object has finished loading and is ready to become active
        eState_Active,          // Scene object is active (running)
        eState_ScheduleDown,    // Scene object has been requested to go down
        eState_Down,            // Scene object is down, busy deconstructing
        eState_Dead             // Scene object has been deconstructed, will now be freed
    };

public:
    virtual void _08(void); // deinit
    virtual ~CScene(void) {}
    virtual void _10(void) = 0; // loadAssets
    virtual void _14(void); // init
    virtual void _18(void);
    virtual void _1C(void); // draw
    virtual void _20(void); // unloadAssets
    virtual bool _24(void); // areAssetsReady

    CScene(void) {
        mState = eState_Initial;
        mUnk04 = true;
    }

    void fn_801D83BC(void); // destroy
    void fn_801D83DC(void); // doUpdate
    void fn_801D8554(void); // doDraw
    void fn_801D8578(void); // startDown

    EState getState(void) const { return mState; }

    u16 getMemGroup(void) const { return mMemGroup; }
    void setMemGroup(u16 memGroup) { mMemGroup = memGroup; }

    bool getUnk0E(void) const { return mUnk0E; }
    void setUnk0E(bool flag) { mUnk0E = flag; }

private:
    bool mUnk04;
    EState mState;
    u16 mMemGroup;
    bool mUnk0E;
};

#endif