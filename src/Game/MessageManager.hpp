#ifndef GAME_MESSAGEMANAGER_HPP
#define GAME_MESSAGEMANAGER_HPP

#include <revolution/types.h>
#include <revolution/macros.h>

#include <nw4r/lyt.h>

#include "Singleton.hpp"

class CMessageManager; // Forward-declaration
class CUnkMessage; // Forward-declaration

struct BMGFileHeader; // Forward-declaration
struct BMGMessageInfo; // Forward-declaration
struct BMGMessageData; // Forward-declaration
struct BMGStringAttributeInfo; // Forward-declaration
struct BMGMessageIDInfo; // Forward-declaration

enum { MESG_ID_INVALID = 10000001 };

enum EMesgSource {
    eMesgSource_Main,       // riq_X.bmg
    eMesgSource_Title,      // riq_title_X.bmg
    eMesgSource_Comment,    // riq_comment_X.bmg
    eMesgSource_Bonus,      // riq_bonus_X.bmg
    eMesgSource_Cafe,       // riq_cafe_X.bmg

    eMesgSource_Count
};

class CBMGRes {
    friend class CMessageManager;
    friend class CUnkMessage;

public:
    CBMGRes(void);
    ~CBMGRes(void);

private:
    void init(u32 arcIndex, const char *pathInArc);

    const wchar_t *getString(u32 id);
    u32 getStringSize(u32 id);
    u16 getFontScale(u32 id);
    u16 getFontScaleOrig(u32 id);
    u8 getTextAlign(u32 id);

    u8 fn_80087854(u32 id);
    u16 fn_80087A30(u32 id, u8);
    const wchar_t *fn_80087CB0(u32 id, u8, wchar_t *);

    u32 mesgIDToIndex(u32 id);

private:
    void *mData;

    BMGFileHeader *mFileHeader;
    BMGMessageInfo *mMessageInfo;
    BMGMessageData *mMessageData;
    BMGStringAttributeInfo *mStrAttributeInfo;
    BMGMessageIDInfo *mMessageIDInfo;
};

class CMessageManager : public TSingleton<CMessageManager> {
    friend class CUnkMessage;

public:
    virtual ~CMessageManager(void);

    CMessageManager(void);

    void fn_80088030(void);
    void fn_80088034(void);
    void fn_8008807C(void);
    void fn_80088088(void);
    void fn_800883F4(void);
    void fn_80088474(nw4r::lyt::Pane *);
    void fn_800888E0(nw4r::lyt::TextBox *textBox, const wchar_t *src, s32 size, f32 scale, s32 textAlign);
    void fn_80088FEC(nw4r::lyt::TextBox *, s32, s32);
    u32 fn_800892A8(const wchar_t *src, wchar_t *dest, s32 srcSize);

    void setupTextBox(nw4r::lyt::TextBox *textBox, const char *mesgIDStr, s32 sourceID) {
        s32 mesgID = fn_80087708(mesgIDStr);
        fn_80088FEC(textBox, mesgID, sourceID);
    }

    void setFormatIDTitle(const char *mesgIDStr) {
        mFormatIDTitle = fn_80087708(mesgIDStr);
    }
    void setFormatIDBonus(const char *mesgIDStr) {
        mFormatIDBonus = fn_80087708(mesgIDStr);
    }

    const wchar_t *getString(s32 mesgID, s32 sourceID) {
        return mBMG[sourceID].getString(mesgID);
    }
    const wchar_t *getString(const char *mesgIDStr, s32 sourceID) {
        return getString(fn_80087708(mesgIDStr), sourceID);
    }

    static s32 fn_80087708(const char *mesgIDStr) DONT_INLINE;

private:
    CBMGRes mBMG[eMesgSource_Count];
    s32 mFormatIDTitle;
    s32 mFormatIDBonus;
    u8 mMedalCount;
    u16 mUnk86;
    u16 mUnk88;
};

class CUnkMessage {
public:
    CUnkMessage(const char *mesgIDStr, s32 sourceID, BOOL);
    ~CUnkMessage(void);

    u32 fn_80089CEC(void);
    u8 fn_80089CF4(void);
    u16 fn_80089CFC(s32 index);
    wchar_t *fn_80089D0C(s32 index);
    bool fn_80089D20(s32 index);
    s32 fn_80089D40(s32 index);
    u16 fn_80089D88(s32, s32);
    wchar_t *fn_80089DA0(s32, s32);
    u32 fn_80089DBC(s32, s32);
    u16 fn_80089DD8(void);
    u16 fn_80089DE0(void);
    u32 fn_80089DE8(void);

private:
    void fn_80089934(s32 mesgID, s32 sourceID, BOOL);

private:
    struct Sub {
        u16 dataSize;
        wchar_t *unk04;
        wchar_t unk08;
        u32 unk0C;
    };

private:
    u32 mMessageID;
    u8 mSubCount;
    Sub *mSub;
    u16 mUnk0C;
    u16 mUnk0E;
    u32 mUnk10;
};

extern CMessageManager *gMessageManager;

#endif