#include "GameManager.hpp"
#include "FileManager.hpp"
#include "GraphicManager.hpp"
#include "Controller.hpp"
#include "LayoutManager.hpp"
#include "CellAnimManager.hpp"
#include "HBMManager.hpp"
#include "MessageManager.hpp"
#include "Sound.hpp"
#include "TickFlowManager.hpp"
#include "InputCheckManager.hpp"
#include "CheckPointManager.hpp"
#include "SceneManager.hpp"
#include "SaveData.hpp"
#include "BackupManager.hpp"
#include "SDManager.hpp"
#include "VarParam.hpp"
#include "DebugPrint.hpp"
#include "MyCanvas.hpp"
#include "DummyManager.hpp"

// NOTE: The global instances of these classes are located elsewhere.
// #include "CafeTopic.hpp"
// #include "RFLManager.hpp"
// #include "PlayHistory.hpp"
// #include "DebugFont.hpp"

CMyCanvasManager *gMyCanvasManager;
CMessageManager *gMessageManager;
CBackupManager *gBackupManager;
CSaveData *gSaveData;
CHBMManager *gHBMManager;
CSceneManager *gSceneManager;
CDebugPrint *gDebugPrint;
CVarParamManager *gVarParamManager;
CSDManager *gSDManager;
CCheckPointManager *gCheckPointManager;
CInputCheckManager *gInputCheckManager;
CTickFlowManager *gTickFlowManager;
CCellAnimManager *gCellAnimManager;
CDummyManager *gDummyManager;
CGameManager *gGameManager;
CSoundManager *gSoundManager;
CLayoutManager *gLayoutManager;
CControllerManager *gControllerManager;
CGraphicManager *gGraphicManager;
CFileManager *gFileManager;
