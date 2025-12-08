#ifndef NW4R_UT_RESFONT_H
#define NW4R_UT_RESFONT_H

#include <revolution/types.h>
#include <nw4r/ut/ResFontBase.h>

namespace nw4r {
namespace ut {

class ResFont : public detail::ResFontBase {
public:
    ResFont();
    virtual ~ResFont();

    bool SetResource(void *);
    void *RemoveResource();

private:
};

} // namespace ut
} // namespace nw4r

#endif
