#ifndef NW4R_SND_TASK_H
#define NW4R_SND_TASK_H
#include <nw4r/types_nw4r.h>

#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

class Task : private ut::NonCopyable {
	friend class TaskManager;

public:
	Task() :
		mBusyFlag(false)
	{}
	virtual ~Task();

	virtual void Execute() = 0;
	virtual void Cancel() = 0;
	virtual void OnCancel() = 0;

private:
	ut::LinkListNode	mTaskLink;
	bool				mBusyFlag;
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif // NW4R_SND_TASK_H
