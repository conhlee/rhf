#ifndef RVL_SDK_HBM_HOMEBUTTON_ANM_CONTROLLER_HPP
#define RVL_SDK_HBM_HOMEBUTTON_ANM_CONTROLLER_HPP

#include "hbm/HBMFrameController.hpp"

// forward declarations
namespace nw4hbm { namespace lyt { class AnimTransform; class Group; }}

namespace homebutton {

class GroupAnmController : public FrameController {
public:
    GroupAnmController();
    virtual ~GroupAnmController();

    virtual void calc();

    void do_calc();

public: // exposed for HomeButton::create
    /* base FrameController */					// size 0x20, offset 0x00
    nw4hbm::lyt::Group			*mpGroup;		// size 0x04, offset 0x20
    nw4hbm::lyt::AnimTransform	*mpAnimGroup;	// size 0x04, offset 0x24
}; // size 0x28

} // namespace homebutton

#endif // RVL_SDK_HBM_HOMEBUTTON_ANM_CONTROLLER_HPP
