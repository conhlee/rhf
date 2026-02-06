#include <Runtime/MWCPlusLib.h>
#include <Runtime/NMWException.h>

DestructorChain *__global_destructor_chain;

void *__register_global_object(void* object, void* destructor, void* regmem) {
    ((DestructorChain*)regmem)->next = __global_destructor_chain;
    ((DestructorChain*)regmem)->destructor = destructor;
    ((DestructorChain*)regmem)->object = object;
    __global_destructor_chain = (DestructorChain*)regmem;

    return object;
}

void __destroy_global_chain(void) {
    DestructorChain *iter;

    while ((iter = __global_destructor_chain) != NULL) {
        __global_destructor_chain = iter->next;
        DTORCALL_COMPLETE(iter->destructor, iter->object);
    }
}

// NOTE: Unused
int __register_atexit(void (*func)(void)) {}

#pragma section ".dtors$10"
DECL_SECTION(".dtors$10") DECL_WEAK
    extern void * const __destroy_global_chain_reference = __destroy_global_chain;