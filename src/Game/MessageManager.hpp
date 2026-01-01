#ifndef GAME_MESSAGEMANAGER_HPP
#define GAME_MESSAGEMANAGER_HPP

#include <revolution/types.h>

#include <nw4r/lyt.h>

#include "Singleton.hpp"

enum { MESG_ID_INVALID = 10000001 };

class CMessageManager; // Forward-declaration

struct BMGFileHeader; // Forward-declaration
struct BMGMessageInfo; // Forward-declaration
struct BMGMessageData; // Forward-declaration
struct BMGStringAttributeInfo; // Forward-declaration
struct BMGMessageIDInfo; // Forward-declaration

class CBMGRes {
    friend class CMessageManager;

public:
    CBMGRes(void);
    ~CBMGRes(void);

private:
    void init(u32 arcIndex, const char *pathInArc);
    wchar_t *getString(u32 id);

private:
    void *mData;

    BMGFileHeader *mFileHeader;
    BMGMessageInfo *mMessageInfo;
    BMGMessageData *mMessageData;
    BMGStringAttributeInfo *mStrAttributeInfo;
    BMGMessageIDInfo *mMessageIDInfo;
};

enum EMesgSource {
    eMesgSource_Main,       // riq_X.bmg
    eMesgSource_Title,      // riq_title_X.bmg
    eMesgSource_Comment,    // riq_comment_X.bmg
    eMesgSource_Bonus,      // riq_bonus_X.bmg
    eMesgSource_Cafe,       // riq_cafe_X.bmg

    eMesgSource_Count
};

class CMessageManager : public TSingleton<CMessageManager> {
public:
    virtual ~CMessageManager(void);

    CMessageManager(void);

    void fn_80088030(void);
    void fn_80088034(void);
    void fn_8008807C(void);
    void fn_80088088(void);
    void fn_800883F4(void);
    void fn_80088474(nw4r::lyt::Pane *);
    void fn_80088FEC(nw4r::lyt::TextBox *, s32, s32);

    void setupTextBox(nw4r::lyt::TextBox *textBox, const char *mesgIDStr, s32 bmgType) {
        s32 mesgID = fn_80087708(mesgIDStr);
        fn_80088FEC(textBox, mesgID, bmgType);
    }

    void setFormatMesgID(const char *mesgIDStr) {
        mFormatMesgID = fn_80087708(mesgIDStr);
    }

    static s32 fn_80087708(const char *mesgIDStr);

private:
    CBMGRes mBMG[eMesgSource_Count];
    s32 mUnk7C;
    s32 mFormatMesgID;
    u8 mMedalCount;
    u16 mUnk86;
    u16 mUnk88;
};

extern CMessageManager *gMessageManager;

#endif