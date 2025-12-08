#ifndef NW4R_UT_RUNTIME_TYPEINFO_H
#define NW4R_UT_RUNTIME_TYPEINFO_H

#include <revolution/types.h>

namespace nw4r {
namespace ut {

#define NW4R_UT_RUNTIME_TYPEINFO \
    virtual const nw4r::ut::detail::RuntimeTypeInfo *GetRuntimeTypeInfo() const { return &typeInfo; } \
    static const nw4r::ut::detail::RuntimeTypeInfo typeInfo

namespace detail {

struct RuntimeTypeInfo {
    RuntimeTypeInfo(const RuntimeTypeInfo *parent) : mParentTypeInfo(parent) {}

    bool IsDerivedFrom(const RuntimeTypeInfo *parent) const {
        for (const RuntimeTypeInfo *it = this; it != NULL; it = it->mParentTypeInfo) {
            if (it == parent) {
                return true;
            }
        }

        return false;
    }

    const RuntimeTypeInfo *mParentTypeInfo;
};

template <typename T>
inline const RuntimeTypeInfo *GetTypeInfoFromPtr_(T *ptr) {
    return &ptr->typeInfo;
}

} // namespace detail

template <typename TDerived, typename TBase>
inline TDerived DynamicCast(TBase *ptr) {
    // Derived type info
    const detail::RuntimeTypeInfo *derivedTypeInfo = detail::GetTypeInfoFromPtr_(static_cast<TDerived>(NULL));
    // Downcast if possible
    // NB Wii/1.6 used in SND seems to be very sensitive to the way this
    // is structured while Wii/1.5 and below don't seem to care at all
    if (ptr != NULL) {
        if (ptr->GetRuntimeTypeInfo()->IsDerivedFrom(derivedTypeInfo)) {
            return static_cast<TDerived>(ptr);
        }
    }

    return NULL;
}

} // namespace ut
} // namespace nw4r

#endif
