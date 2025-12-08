#ifndef NW4R_UT_RESFONTBASE_H
#define NW4R_UT_RESFONTBASE_H

#include <revolution/types.h>
#include <nw4r/ut/Font.h>

namespace nw4r {
namespace ut {
namespace detail {

class ResFontBase : public Font {
public:
    ResFontBase();
    virtual ~ResFontBase();

private:
    u8 pad10[0xc];
};

} // namespace detail
} // namespace ut
} // namespace nw4r

#endif
