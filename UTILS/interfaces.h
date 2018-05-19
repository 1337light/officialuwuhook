#pragma once

namespace SDK
{
	class IClient;
	class CInput;
	class CPanel;
	class ISurface;
	class IEngine;
	class CClientEntityList;
	class CGlobalVars;
	class IVDebugOverlay;
	class CTrace;
	class CModelInfo;
	class ICVar;
	class CInputSystem;
	class CModelRender;
	class CRenderView;
	class IMaterialSystem;
	class IPhysicsProps;
	class COverrideView;
	class IGameEventManager;
	class IViewRenderBeams;
	class CGlowObjectManager;
	class IGameMovement;
	class IMoveHelper;
	class CClientState;
}



class IEngineSound;
class IMoveHelper;
class CPrediction;
class CGameMovement;

namespace INTERFACES
{
	extern SDK::IClient* Client;
	extern SDK::CInput* Input;
	extern SDK::CPanel* Panel;
	extern SDK::ISurface* Surface;
	extern SDK::IEngine* Engine;
	extern SDK::CClientEntityList* ClientEntityList;
	extern SDK::CGlobalVars* Globals;
	extern SDK::IVDebugOverlay* DebugOverlay;
	extern SDK::CTrace* Trace;
	extern SDK::CModelInfo* ModelInfo;
	extern SDK::ICVar* cvar;
	extern SDK::CInputSystem* InputSystem;
	extern SDK::CModelRender* ModelRender;
	extern SDK::CRenderView* RenderView;
	extern SDK::IMaterialSystem* MaterialSystem;
	extern SDK::IPhysicsProps* PhysicsProps;
	extern SDK::COverrideView* ClientMode;
	extern SDK::IGameEventManager* GameEventManager;
	extern SDK::IViewRenderBeams* ViewRenderBeams;
	extern IEngineSound* enginesound;
	extern	CPrediction* g_pPred;
	extern SDK::CGlowObjectManager* g_GlowObjManager;
	extern 	IMoveHelper* g_pMoveHelper;
	extern 	CGameMovement* g_pGameMovement;
	extern SDK::CClientState** client_state;
	extern SDK::IGameMovement* pMovement;
	extern SDK::IMoveHelper* pMoveHelper;
	void InitInterfaces();

	typedef void* (*CreateInterfaceFn)(const char *Name, int *ReturnCode);
	void* FindInterface(const char* Module, const char* InterfaceName);
	void* FindInterfaceEx(const char* Module, const char* InterfaceName);
}