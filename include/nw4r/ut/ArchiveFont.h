#ifndef NW4R_UT_ARCHIVEFONT_H
#define NW4R_UT_ARCHIVEFONT_H

#include <revolution/types.h>
#include <nw4r/ut/ArchiveFontBase.h>

namespace nw4r {
namespace ut {

class ArchiveFont : public detail::ArchiveFontBase {
public:
    ArchiveFont();
    void Construct(void *, u32, const void *, const char *param_4 = LOAD_GLYPH_ALL);
    void *Destroy();

    static u32 GetRequireBufferSize(const void *, const char *param_2 = LOAD_GLYPH_ALL);

private:
};

} // namespace ut
} // namespace nw4r

#endif
