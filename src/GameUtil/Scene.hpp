#ifndef GAMEUTIL_SCENE_HPP
#define GAMEUTIL_SCENE_HPP

#include <revolution/types.h>

#include "Mem.hpp"

#define SCENE_DECL_CREATE_FN() static CScene *create(u16 heapGroup);
#define SCENE_IMPL_CREATE_FN(_className)        \
    CScene *_className::create(u16 heapGroup) { \
        fn_801D369C(heapGroup);                 \
        _className *scene = new _className;     \
        fn_801D3644();                          \
        scene->setHeapGroup(heapGroup);         \
        return scene;                           \
    }

class CScene {
public:
    typedef CScene *(*CreateFn)(u16 heapGroup);
    enum EState {
        eState_Unprepared = 0,
        eState_Preparing = 1,
        eState_Prepared = 2,
        eState_3 = 3,
        eState_4 = 4,
        eState_5 = 5,
        eState_Final = 6
    };

    virtual void _08(void); // deinit
    virtual ~CScene(void) {}
    virtual void _10(void) = 0; // loadAssets
    virtual void _14(void); // init
    virtual void _18(void);
    virtual void _1C(void); // draw
    virtual void _20(void); // unloadAssets
    virtual bool _24(void); // areAssetsReady

    CScene(void) {
        mState = eState_Unprepared;
        mUnk04 = true;
    }

    void fn_801D83BC(void);
    void fn_801D83DC(void);
    void fn_801D8554(void);
    void fn_801D8578(void);

    EState getState(void) const { return mState; }

    u16 getHeapGroup(void) const { return mHeapGroup; }
    void setHeapGroup(u16 heapGroup) { mHeapGroup = heapGroup; }

    bool getUnk0E(void) const { return mUnk0E; }
    void setUnk0E(bool flag) { mUnk0E = flag; }

private:
    bool mUnk04;
    EState mState;
    u16 mHeapGroup;
    bool mUnk0E;
};

#endif