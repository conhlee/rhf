#ifndef RVL_SDK_HBM_COMMON_H
#define RVL_SDK_HBM_COMMON_H
#include <revolution/types.h>
#include <revolution/MEM.h>
#include <revolution/KPAD.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum HBMSelectBtnNum
{
	HBM_SELECT_NULL		= -1,

	HBM_SELECT_HOMEBTN,
	HBM_SELECT_BTN1,
	HBM_SELECT_BTN2,
	HBM_SELECT_BTN3,
	HBM_SELECT_BTN4,

	HBM_SELECT_MAX
} HBMSelectBtnNum;

typedef int HBMSoundCallback(int evt, int num);

typedef struct HBMDataInfo
{
	void				*layoutBuf;			// size 0x04, offset 0x00
	void				*spkSeBuf;			// size 0x04, offset 0x04
	void				*msgBuf;			// size 0x04, offset 0x08
	void				*configBuf;			// size 0x04, offset 0x0c
	void				*mem;				// size 0x04, offset 0x10
	HBMSoundCallback	*sound_callback;	// size 0x04, offset 0x14
	int					backFlag;			// size 0x04, offset 0x18
	int					region;				// size 0x04, offset 0x1c
	int					cursor;				// size 0x04, offset 0x20
	int					messageFlag;		// size 0x04, offset 0x24
	u32					configBufSize;		// size 0x04, offset 0x28
	u32					memSize;			// size 0x04, offset 0x2c
	f32					frameDelta;			// size 0x04, offset 0x30
	Vec2				adjust;				// size 0x08, offset 0x34
	MEMAllocator		*pAllocator;		// size 0x04, offset 0x3c
} HBMDataInfo; // size 0x40

typedef struct HBMKPadData
{
	KPADStatus	*kpad;			// size 0x04, offset 0x00
	Vec2		pos;			// size 0x08, offset 0x04
	u32			use_devtype;	// size 0x04, offset 0x0c
} HBMKPadData; // size 0x10

typedef struct HBMControllerData
{
	HBMKPadData	wiiCon[WPAD_MAX_CONTROLLERS];	// size 0x40, offset 0x00
} HBMControllerData; // size 0x40

void HBMCreate(const HBMDataInfo *pHBInfo);
void HBMDelete(void);
void HBMInit(void);
HBMSelectBtnNum HBMCalc(const HBMControllerData *pController);
void HBMDraw(void);
HBMSelectBtnNum HBMGetSelectBtnNum(void);
void HBMSetAdjustFlag(BOOL flag);
void HBMStartBlackOut(void);
BOOL HBMIsReassignedControllers(void);

void HBMCreateSound(void *soundData, void *memBuf, u32 memSize);
void HBMDeleteSound(void);
void HBMUpdateSound(void);

#ifdef __cplusplus
}
#endif
#endif
