#ifndef RVL_SDK_OS_RESET_H
#define RVL_SDK_OS_RESET_H
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

void OSShutdownSystem(void);
void OSReturnToMenu(void);
void OSRestart(u32 resetCode);

#ifdef __cplusplus
}
#endif
#endif