#pragma once
#include "../UTILS/CUtlVector.h"
namespace SDK
{
	class IMatRenderContext;
	class Ray_t;
	class ITraceFilter;

	struct CViewSetup;
	struct DrawModelState_t;
	struct ModelRenderInfo_t;
	struct trace_t;
}

class IEngineSound;
class IRecipientFilter;

namespace HOOKS
{
	typedef bool(__stdcall* CreateMoveFn)(float, SDK::CUserCmd*);
	typedef void(__thiscall* FrameStageNotifyFn)(void*, int);
	typedef void(__thiscall* PaintTraverseFn)(void*, unsigned int, bool, bool);
	typedef void(__thiscall* DrawModelExecuteFn)(void*, SDK::IMatRenderContext*, const SDK::DrawModelState_t&, const SDK::ModelRenderInfo_t&, matrix3x4_t*);
	typedef void(__thiscall* SceneEndFn)(void*);

	typedef void(__thiscall* TraceRayFn)(void *thisptr, const SDK::Ray_t &ray, unsigned int fMask, SDK::ITraceFilter* pTraceFilter, SDK::trace_t *pTrace);
	typedef void(__thiscall* OverrideViewFn)(void* ecx, SDK::CViewSetup* pSetup);
	typedef int(__thiscall* SendDatagramFn)(void*, void*);
	typedef void(__thiscall* SetMouseCodeState_t) (void*, ButtonCode_t, MouseCodeState_t);
	typedef void(__thiscall* SetKeyCodeState_t) (void*, ButtonCode_t, bool);
	using do_post_screen_space_effects_t = bool(__thiscall*)(void*, SDK::CViewSetup*);
	//typedef int(__thiscall* ToFirstPersonFn)(void* thisptr);
	typedef bool(__thiscall* SvCheatsGetBoolFn)(void*);
	//extern CreateMoveFn original_create_move;
	extern PaintTraverseFn original_paint_traverse;
	extern FrameStageNotifyFn original_frame_stage_notify;
	extern DrawModelExecuteFn original_draw_model_execute;
	extern SceneEndFn original_scene_end;
	extern TraceRayFn original_trace_ray;
	extern SendDatagramFn original_send_datagram;
	extern OverrideViewFn original_override_view;
	extern do_post_screen_space_effects_t glow;
	//extern ToFirstPersonFn original_to_firstperson;
	extern SvCheatsGetBoolFn original_get_bool;
	extern VMT::VMTHookManager iclient_hook_manager;
	extern VMT::VMTHookManager panel_hook_manager;
	extern VMT::VMTHookManager model_render_hook_manager;
	extern VMT::VMTHookManager render_view_hook_manager;
	extern VMT::VMTHookManager trace_hook_manager;
	extern VMT::VMTHookManager net_channel_hook_manager;
	extern VMT::VMTHookManager override_view_hook_manager;
	extern VMT::VMTHookManager input_table_manager;
	extern VMT::VMTHookManager get_bool_manager;
	extern VMT::VMTHookManager sound;
	void InitHooks();

	// virtual function hooks
	bool __stdcall HookedCreateMove(float sample_input_frametime, SDK::CUserCmd* cmd);
	void __fastcall HookedFrameStageNotify(void*, void*, int);
	void __stdcall HookedPaintTraverse(int VGUIPanel, bool ForceRepaint, bool AllowForce);
	void __fastcall HookedDrawModelExecute(void*, void*, SDK::IMatRenderContext*, const SDK::DrawModelState_t&, const SDK::ModelRenderInfo_t&, matrix3x4_t*);
	void __fastcall HookedSceneEnd(void*, void*);
	void __fastcall HookedTraceRay(void *thisptr, void*, const SDK::Ray_t &ray, unsigned int fMask, SDK::ITraceFilter *pTraceFilter, SDK::trace_t *pTrace);
	void __fastcall HookedSendDatagram(void* ecx, void* data);
	void __fastcall HookedOverrideView(void* ecx, void* edx, SDK::CViewSetup* pSetup);
	bool __fastcall HookedGetBool(void* pConVar, void* edx);
	bool _fastcall hkDoPostScreenSpaceEffects(void* ecx, void* edx, SDK::CViewSetup* pSetup);
	//int __fastcall hkSendDatagram(SDK::NetChannel* thisptr, void* edx, void* datagram);
	// netvar hooks
	void InitNetvarHooks();
}

