#ifndef GAMEUTIL_SCENEMANAGER_HPP
#define GAMEUTIL_SCENEMANAGER_HPP

#include <revolution/types.h>

#include "Singleton.hpp"

#include "ExFlowDecl.hpp"

enum ESceneID { // TODO: move this somewhere else
    eScene_Menu = 0,
    eScene_Badminton,
    eScene_Baseball,
    eScene_Batting,
    eScene_Birds,
    eScene_Bun,
    eScene_Concons,
    eScene_Date,
    eScene_Fishing,
    eScene_Flea,
    eScene_Fork,
    eScene_Goma,
    eScene_Interview,
    eScene_Karateman,
    eScene_Muscle,
    eScene_Panel,
    eScene_Rap,
    eScene_Receive,
    eScene_Robot,
    eScene_Rocket,
    eScene_Rotation,
    eScene_Samurai,
    eScene_Seal,
    eScene_Seesaw,
    eScene_Shrimp,
    eScene_Sort,
    eScene_Tambourine,
    eScene_Tap,
    eScene_Watch,
    eScene_Alarm,
    eScene_Eatmen,
    eScene_Metroman,
    eScene_Shooting,
    eScene_Boat,
    eScene_Kungfu,
    eScene_Omochi,
    eScene_Sword,
    eScene_Basket,
    eScene_Tutorial,
    eScene_Frog,
    eScene_Choro,
    eScene_Hihat,
    eScene_Mail,
    eScene_RhythmFighterVS,
    eScene_AgbClap,
    eScene_AgbGhost,
    eScene_AgbKanji,
    eScene_AgbTap,
    eScene_NightWalk,
    eScene_Error,
    eScene_Strap,
    eScene_Logo,
    eScene_Title,
    eScene_GameSelect,
    eScene_BonusSelect,
    eScene_Prologue,
    eScene_Epilogue,
    eScene_Grading,
    eScene_Grading2P,
    eScene_Perfect,
    eScene_Navi,
    eScene_Cafe,
    eScene_CafeMenu,
    eScene_Reading,
    eScene_Credit,

    eScene_Count,
    eScene_Null = -1,
};

class CSceneManager : public TSingleton<CSceneManager> {
public:
    virtual void _08(void);
    virtual ~CSceneManager(void);
    virtual void _10(void);
    virtual void _14(void);
    virtual void _18(void);

    CSceneManager(void);

    void fn_80089FE0(ESceneID sceneID, const TickFlowCode *tickFlowCode);
    void fn_8008A4DC(ESceneID sceneID, s32 ver);
    void fn_8008A704(ESceneID sceneID);
    void fn_8008A82C(ESceneID sceneID);
    void fn_8008A8D8(void);
    u32 fn_8008B058(s32 idx);
    void fn_8008B068(void);
    bool fn_8008B0FC(s32 idx, u32 sceneID);
    bool fn_8008B118(ESceneID sceneID);
    bool fn_8008B27C(void);

    s32 findSceneInHist(ESceneID sceneID) {
        int i;

        for (i = 0; i < ARRAY_LENGTH(mSceneHist); i++) {
            if (mSceneHist[i] == sceneID) {
                return i;
            }
            else if (mSceneHist[i] == eScene_Null) {
                return -1;
            }
        }
        return -1;
    }

private:
    u32 mSceneHist[256];
    u8 unk404;
};

extern bool lbl_80320274;

extern CSceneManager *gSceneManager;


#endif