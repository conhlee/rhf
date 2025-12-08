#ifndef NW4R_UT_FONT_H
#define NW4R_UT_FONT_H

#include <revolution/types.h>

namespace nw4r {
namespace ut {

// TODO: fill up
class Font {
public:

    enum Type {
        TYPE_NULL = 0,
        TYPE_ROM = 1,
        TYPE_RESOURCE = 2,
        TYPE_PAIR = 3,
    };

    Font() {}
    virtual ~Font();

private:
    
    u8 pad04[0xc];
};

} // namespace ut
} // namespace nw4r

#endif
