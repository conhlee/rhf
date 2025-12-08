#ifndef NW4R_LYT_RESOURCEACCESSOR_H
#define NW4R_LYT_RESOURCEACCESSOR_H

#include <revolution/types.h>

namespace nw4r {
namespace lyt {

class ResourceAccessor {
public:
    ResourceAccessor();
    virtual ~ResourceAccessor();
    virtual void *GetResource(u32, const char *, u32 *) = 0;
    virtual void *GetFont(const char *);
};

} // namespace lyt
} // namespace nw4r

#endif
