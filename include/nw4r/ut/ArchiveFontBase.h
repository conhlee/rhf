#ifndef NW4R_UT_ARCHIVEFONTBASE_H
#define NW4R_UT_ARCHIVEFONTBASE_H

#include <revolution/types.h>
#include <nw4r/ut/ResFontBase.h>

namespace nw4r {
namespace ut {
namespace detail {

class ArchiveFontBase : public ResFontBase {
protected:
    static const char LOAD_GLYPH_ALL[1];

public:
    ArchiveFontBase();

private:
    u16 *mpGlyphIndexAdjustArray;
};

} // namespace detail
} // namespace ut
} // namespace nw4r

#endif
