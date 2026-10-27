#ifndef GAMEUTIL_VARPARAMMANAGER_HPP
#define GAMEUTIL_VARPARAMMANAGER_HPP

#include <revolution/types.h>

#include <revolution/MEM.h>

#include "List.hpp"
#include "Singleton.hpp"

class CVarParam : public CList {
public:
    CVarParam();
    ~CVarParam();

    void fn_801ECEC4(s32 x, s32 y);

private:
    char mTitle[0x20];
    u32 unk2C;
    u32 mType;
    u64 &mUlongVal;
    u32 &mUintVal;
    u16 &mUshortVal;
    u8 &mUcharVal;
    s64 &mLongVal;
    s32 &mIntVal;
    s16 &mShortVal;
    s8 &mCharVal;
    f32 &mFloatVal;
    u8 unk58[0xa8 - 0x58];
};

class CVarParamManager : public TSingleton<CVarParamManager> {
public:
    virtual void _08(void);
    virtual ~CVarParamManager(void);
    virtual void _10(s32 paramCount);
    virtual void _14(void);

    CVarParamManager(void);

    void fn_801ED2AC(void);

private:
    static void fn_801ED44C(void);

    CVarParam *mVarParamHead;
    s32 mDisplayMode;
    u8 mUnk0C;

    u8 *mHeapStart;
    MEMiHeapHead *mHeap;
};

extern CVarParamManager *gVarParamManager;

#endif
