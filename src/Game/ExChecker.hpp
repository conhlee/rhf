#ifndef GAME_EXCHECKER_HPP
#define GAME_EXCHECKER_HPP

#include <revolution/types.h>

#include "InputChecker.hpp"

class CExChecker : public CInputChecker {
public:
    virtual ~CExChecker(void) {}
    virtual void _14(void);
    virtual u32 _18(void);
    virtual u32 _20(u8 &, u32, f32);

    CExChecker(void) {
        fn_8008EB08(0);
    }

    void fn_8008EB08(u8);
};


#endif
