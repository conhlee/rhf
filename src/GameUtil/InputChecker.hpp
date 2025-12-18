#ifndef GAMEUTIL_INPUTCHECKER_HPP
#define GAMEUTIL_INPUTCHECKER_HPP

#include <revolution/types.h>
#include "List.hpp"

class CInputChecker : public CList {
public:
    CInputChecker *getNext(void) const {
        return static_cast<CInputChecker *>(CList::getNext());
    }
    CInputChecker *getPrev(void) const {
        return static_cast<CInputChecker *>(CList::getPrev());
    }

    virtual ~CInputChecker(void);
    virtual void finalInsert(void);
    virtual void finalDestroy(void);
    virtual void _14(void);
    virtual u32 _18(void);
    virtual u32 _1C(void); // weak
    virtual u32 _20(u8 &, u32, f32);
    virtual u32 _24(u32);
    virtual u32 _28(u32);
    virtual u32 _2C(void);
    virtual u32 _30(u32);
    virtual void _34(void); // weak

    CInputChecker(void);

    // TODO

    void fn_801E7D5C(u32);
    void fn_801E7D6C(u32);
    void fn_801E7D8C(void);
    void fn_801E7D98(u32);
    void fn_801E7DA8(void);
    void fn_801E7DB4(u32);
    void fn_801E7DBC(f32 tickJust, f32 rangeMissF, f32 rangeJustF, f32 rangeJustB, f32 rangeMissB);
    void fn_801E7DD4(f32 *tickJust, f32 *rangeMissF, f32 *rangeJustF, f32 *rangeJustB, f32 *rangeMissB);
    bool fn_801E7E28(u32);
    bool fn_801E7E5C(u32);
    bool fn_801E7E70(u32);
    bool fn_801E8018(f32, f32);

    static void fn_801E80BC(f32 &rangeMissF, f32 &rangeJustF, f32 &rangeJustB, f32 &rangeMissB);

    u8 getUnk50(void) {
        return unk50;
    }
    u8 getUnk70(void) {
        return unk70;
    }
    u8 getUnk71(void) {
        return unk71;
    }
    u8 getUnk72(void) {
        return unk72;
    }
    u32 getUnk74(void) {
        return unk74;
    }
    u8 getUnk78(void) {
        return unk78;
    }

private:
    u32 unk0C[8];
    u8 unk2C;
    u32 unk30[8];
    u8 unk50;
    u8 unk51;
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    f32 unk64;
    f32 unk68;
    f32 unk6C;
    u8 unk70;
    u8 unk71;
    bool unk72;
    u32 unk74;
    u8 unk78;
    u8 unk79;
};

#endif
