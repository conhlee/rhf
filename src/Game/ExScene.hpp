#ifndef GAME_EXSCENE_HPP
#define GAME_EXSCENE_HPP

#include <revolution/types.h>

#include "Scene.hpp"

#include "PauseLayout.hpp"
#include "CursorLayout.hpp"

#include "CellAnim.hpp"

class CExScene : public CScene {
public:
    virtual void _08(void);
    virtual ~CExScene(void) {}
    virtual void _10(void) = 0;
    virtual void _14(void);
    virtual void _18(void);
    virtual void _1C(void);
    virtual void _20(void);
    virtual void _28(void);

    CExScene(void);
    
    void fn_80008F68(bool update, u16 tempo);
    void fn_80008F70(bool update, u16 soundID);
    void fn_80008FC0(bool update, u16 soundID);

    bool fn_80009AA0(void);

    void fn_80009F24(s32 channel);

    void fn_8000A7AC(
        u8 animDataID, u16 gameOverAnimID, u16 hiScoreAnimID,
        s32 iconSelectLayoutID,
        s32 layer
    );
    void fn_8000A864(void);
    void fn_8000A8A4(void);
    void fn_8000A8C4(bool gameOver);
    void fn_8000A8CC(void);
    bool fn_8000A944(void);
    void fn_8000A94C(bool highScore, s32 score);
    bool fn_8000A958(void);
    void fn_8000A960(void);
    bool fn_8000A998(void);
    void fn_8000A9CC(s32 gameIndex, bool twoPlay);
    
    static void fn_80008DF8(u8, s32);
    static void fn_80008E68(u8, f32);

    static void fn_80008EF0(void);

    static void fn_80008EFC(void);

    static void fn_80009028(void);

    static void fn_8000966C(void);

    static void fn_800096EC(s32 x, s32 y);
    static void fn_8000973C(void);
    static bool fn_80009748(void);

    static void fn_8000975C(s32 pos);
    static void fn_800097C4(bool practice);

    static void fn_80009814(void);
    static void fn_80009820(void);
    static void fn_80009830(void);
    static bool fn_80009840(void);

    static void fn_8000984C(void);
    static void fn_80009894(void);
    static void fn_800098D0(void);

    static void fn_80009900(s32 x, s32 y);

    static void fn_80009950(s32 x, s32 y);
    static void fn_800099A0(s32 x, s32 y);
    static void fn_800099F0(s32 x, s32 y);
    static void fn_80009A40(bool enabled);
    static void fn_80009A4C(bool enabled);
    static void fn_80009A58(bool enabled);
    
    static void fn_80009A64(bool enabled);
    static void fn_80009A70(u8 opacity);
    static void fn_80009A7C(u8 r, u8 g, u8 b);
    static void fn_80009A90(s32 layer);

    static void fn_80009A9C(void);
    
    static void fn_80009ABC(void);

    static void fn_80009B54(s32 x, s32 y);
    static void fn_80009BA4(u16 frame, s32, s32);
    static void fn_80009BEC(void);
    static void fn_80009C28(s32);
    static void fn_80009D3C(void);

    static void fn_80009D60(void);
    static bool fn_80009D6C(void);

    static void fn_80009D74(bool);
    static bool fn_80009D7C(void);

    static void fn_80009D84(f32);

    static bool fn_80009D94(void);
    static void fn_80009DB8(bool);
    static bool fn_80009E1C(void);
    static void fn_80009E24(bool enabled);
    static void fn_80009E44(bool);
    static void fn_80009E58(void);
    static void fn_80009E98(u32);

    static void fn_80009F70(bool);
    static bool fn_80009FB4(void);
    static void fn_80009FBC(void);
    static void fn_8000A038(void);

    static void fn_8000A084(void);
    static void fn_8000A0A4(void);
    static bool fn_8000A0EC(void);
    static void fn_8000A0F8(void);
    static void fn_8000A1D4(bool enabled);
    static void fn_8000A1E0(s32 layer);

    static void fn_8000A1EC(void);
    static void fn_8000A20C(void);
    static bool fn_8000A254(void);
    static void fn_8000A260(void);
    static void fn_8000A2FC(void);

    static void fn_8000A334(bool);
    static bool fn_8000A370(void);

    static void fn_8000A378(void);

    static void fn_8000A564(bool, bool);
    static bool fn_8000A6B4(void);

    static void fn_8000A6BC(void);
    static void fn_8000A6C8(void);

    static void fn_8000A6D8(s32);
    static bool fn_8000A6E0(void);

    static void fn_8000A708(s32);
    static void fn_8000A74C(s32);

    static void fn_8000A790(s32 fadeFrames);

    static bool fn_8000A7A0(void);


protected:
    CPauseLayout *mPauseLayout;
    CCursorLayout *mCursorLayout;
    CLayout *mLyricLayout; // TODO: CLyricLayout
    CCellAnim *mEndlessGameOverAnim;
    CCellAnim *mEndlessHiScoreAnim;
    bool mEndlessGameOver;
    bool mEndlessGotHighScore;
    s32 mEndlessScore;
    u16 mEndlessGameOverAnimID;
    u16 mEndlessHiScoreAnimID;
    s32 mEndlessIconSelectLayoutID;
};

#endif