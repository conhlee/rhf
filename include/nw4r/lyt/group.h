#ifndef NW4R_LYT_GROUP_H
#define NW4R_LYT_GROUP_H

#include <revolution/types.h>
#include <nw4r/ut/LinkList.h>
#include <nw4r/lyt/pane.h>

namespace nw4r {
namespace lyt {

namespace detail {

struct PaneLink {
    ut::LinkListNode mLink;
    Pane *mTarget;
};

} // namespace detail

class Group {
public:
    ut::LinkListNode mLink;

protected:
    ut::LinkList<detail::PaneLink, offsetof(detail::PaneLink, mLink)> mPaneLinkList;
    char mName[16 + 1];
    u8 mbUserAllocated;
    u8 mPadding[2];
};

class GroupContainer {
protected:
    ut::LinkList<Group, offsetof(Group, mLink)> mGroupList;
};

} // namespace lyt
} // namespace nw4r

#endif