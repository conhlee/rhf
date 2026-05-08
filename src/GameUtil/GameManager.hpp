#ifndef GAMEUTIL_GAMEMANAGER_HPP
#define GAMEUTIL_GAMEMANAGER_HPP

#include <revolution/types.h>
#include <revolution/macros.h>

#include "Singleton.hpp"

#include "Scene.hpp"

#include "FaderFlash.hpp"

#include "TickFlowDecl.hpp"

class CGameManager : public TSingleton<CGameManager> {
public:
    virtual void _08(void);
    virtual ~CGameManager(void);
    virtual void _10(void);
    virtual void _14(void);
    virtual void _18(void);
    virtual void _1C(CScene::CreateFn sceneCreateFn, CFaderFlash *fader, u16 sceneMemGroup);
    virtual void _20(CScene::CreateFn sceneCreateFn, u16 sceneMemGroup);
    virtual void _24(CFaderFlash *fader);
    virtual void _28(void);
    virtual bool _2C(void);
    virtual bool _30(void) {
        return mFader->getState() == CFaderFlash::eState_Idle;   
    }
    virtual void _34(const TickFlowCode *tickFlowCode) {
        mNextTickFlowCode = tickFlowCode;
    }

    CGameManager(void);

    template <typename T>
    void startMainLoop(void) {
        CFaderFlash *fader = CFaderFlash::fn_80007C28();
        _1C(T::create, fader, eMemGroup_Scene);
    }

    CScene *getCurrentScene(void) const { return mCurrentScene; }

    template <typename T>
    T *getCurrentScene(void) const {
        return static_cast<T *>(getCurrentScene());
    }

    CFaderFlash *getFader(void) const { return mFader; }

    static void fn_801D7538(s32 driveStatus);

private:
    CScene *mCurrentScene;
    CScene::CreateFn mNextSceneCreateFunc;
    u16 mNextSceneMemGroup;
    CFaderFlash *mFader;
    const TickFlowCode *mNextTickFlowCode;
};

extern CGameManager *gGameManager;

#endif 