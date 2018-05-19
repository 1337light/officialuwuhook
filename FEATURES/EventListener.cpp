#include "../includes.h"

#include "../UTILS/interfaces.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/IEngine.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/ISurface.h"
#include "../SDK/ConVar.h"

#include "../FEATURES/Backtracking.h"
#include "../FEATURES/Resolver.h"
#include "../FEATURES/Visuals.h"
#include "../UTILS/render.h"
#include "EventListener.h"
#include <playsoundapi.h>
#pragma comment(lib, "Winmm.lib")


char* HitgroupToName(int hitgroup)
{
	switch (hitgroup)
	{
	case HITGROUP_HEAD:
		return "head";
	case HITGROUP_LEFTLEG:
		return "left leg";
	case HITGROUP_RIGHTLEG:
		return "right leg";
	case HITGROUP_STOMACH:
		return "stomach";
	case HITGROUP_CHEST:
		return "chest";
	case HITGROUP_LEFTARM:
		return "left arm";
	case HITGROUP_RIGHTARM:
		return "right arm";
	case HITGROUP_GENERIC:
		return "body";
	}
}

CGameEvents::PlayerHurtListener player_hurt_listener;
CGameEvents::BulletImpactListener bullet_impact_listener;
CGameEvents::RoundStartListener round_start_listener;
CGameEvents::PlayerDeathListener player_death_listener;

void CGameEvents::InitializeEventListeners()
{
	//Interfaces::GameEventManager->AddListener(&item_purchase_listener, "item_purchase", false);
	INTERFACES::GameEventManager->AddListener(&player_hurt_listener, "player_hurt", false);
	INTERFACES::GameEventManager->AddListener(&bullet_impact_listener, "bullet_impact", false);
	INTERFACES::GameEventManager->AddListener(&round_start_listener, "round_start", false);
	INTERFACES::GameEventManager->AddListener(&player_death_listener, "player_death", false);
}

SDK::player_info_t GetInfo(int Index) {
	SDK::player_info_t Info;
	INTERFACES::Engine->GetPlayerInfo(Index, &Info);
	return Info;
}

typedef void(__cdecl* MsgFn)(const char* msg, va_list);
void Msg(const char* msg, ...)
{

	if (msg == nullptr)
		return; //If no string was passed, or it was null then don't do anything 
	static MsgFn fn = (MsgFn)GetProcAddress(GetModuleHandle("tier0.dll"), "Msg"); //This gets the address of export "Msg" in the dll "tier0.dll". The static keyword means it's only called once and then isn't called again (but the variable is still there) 
	char buffer[989];
	va_list list; //Normal varargs stuff http://stackoverflow.com/questions/10482960/varargs-t.. 
	va_start(list, msg);

	vsprintf(buffer, msg, list);
	va_end(list);

	fn(buffer, list); //Calls the function, we got the address above. 
}

void CGameEvents::PlayerDeathListener::FireGameEvent(SDK::IGameEvent* game_event)
{
	if (!game_event)
		return;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player)
		return;

	auto entity = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("userid")));
	if (!entity)
		return;

	auto entity_attacker = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("attacker")));

	if (!entity_attacker)
		return;

	if (entity->GetTeam() == local_player->GetTeam())
		return;

	SDK::player_info_t player_info;
	if (!INTERFACES::Engine->GetPlayerInfo(entity->GetIndex(), &player_info))
		return;
}
void DrawHitmarker()
{
	if (!SETTINGS::settings.hitmarker) return;

	int W, H;
	INTERFACES::Engine->GetScreenSize(W, H);

	float r = 255.f;
	float g = 0.f;
	float b = 0.f;
	for (int i = 0; i < 1000; i++)
	{
		if (SETTINGS::settings.hitmarker)
		{
			RENDER::DrawLine(W / 2 - 10, H / 2 - 10, W / 2 - 5, H / 2 - 5, CColor(r, g, b, 255.f));
			RENDER::DrawLine(W / 2 - 10, H / 2 + 10, W / 2 - 5, H / 2 + 5, CColor(r, g, b, 255.f));
			RENDER::DrawLine(W / 2 + 10, H / 2 - 10, W / 2 + 5, H / 2 - 5, CColor(r, g, b, 255.f));
			RENDER::DrawLine(W / 2 + 10, H / 2 + 10, W / 2 + 5, H / 2 + 5, CColor(r, g, b, 255.f));

		}
		i++;
	}
}

void CGameEvents::PlayerHurtListener::FireGameEvent(SDK::IGameEvent* game_event)
{
	if (!game_event)
		return;
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player)
		return;

	auto entity = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("userid")));
	if (!entity)
		return;

	auto entity_attacker = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("attacker")));

	if (!entity_attacker)
		return;

	if (entity->GetTeam() == local_player->GetTeam())
		return;

	SDK::player_info_t player_info;
	if (!INTERFACES::Engine->GetPlayerInfo(entity->GetIndex(), &player_info))
		return;
	if (!SETTINGS::settings.hitmarker) return;

	if (entity_attacker == local_player)
	{
		if (!SETTINGS::settings.hitmarker) return;
		DrawHitmarker();
		hitmarker_time = 255;
		if (SETTINGS::settings.hitmarker_sound == 1)
		{
			INTERFACES::Surface->IPlaySound("buttons\\arena_switch_press_02.wav");
		}
		else if (SETTINGS::settings.hitmarker_sound == 2)
		{
			PlaySoundA(roblox, NULL, SND_ASYNC | SND_MEMORY);
		}
		else if (SETTINGS::settings.hitmarker_sound == 3)
		{
			PlaySoundA(bameware, NULL, SND_ASYNC | SND_MEMORY);
		}
		else if (SETTINGS::settings.hitmarker_sound == 4)
		{
			PlaySoundA(bubble, NULL, SND_ASYNC | SND_MEMORY);
		}

		shots_hit[entity->GetIndex()]++;
	}
	SDK::ConVar* Developer = INTERFACES::cvar->FindVar("developer");
	*(float*)((DWORD)&Developer->fnChangeCallback + 0xC) = NULL;
	Developer->SetValue("1");

	SDK::ConVar* con_filter_enable = INTERFACES::cvar->FindVar("con_filter_enable");
	*(float*)((DWORD)&con_filter_enable->fnChangeCallback + 0xC) = NULL;
	con_filter_enable->SetValue("2");

	SDK::ConVar* con_filter_text = INTERFACES::cvar->FindVar("con_filter_text");
	*(float*)((DWORD)&con_filter_text->fnChangeCallback + 0xC) = NULL;

	con_filter_text->SetValue("[uwuhook]");

	///////////////////////////////////////////////////ID STUFF///////////////////////////////////////////////////////////////////

	int attackerid = game_event->GetInt("attacker");
	int entityid = INTERFACES::Engine->GetPlayerForUserID(attackerid);

	int nUserID = game_event->GetInt("attacker");
	int nDead = game_event->GetInt("userid");

	SDK::player_info_t killed_info = GetInfo(INTERFACES::Engine->GetPlayerForUserID(nDead));
	SDK::player_info_t killer_info = GetInfo(INTERFACES::Engine->GetPlayerForUserID(nUserID));

	/////////////////////////////////////////////////////LOGS/////////////////////////////////////////////////////////////////////


	if (!strcmp(game_event->GetName(), "player_hurt"))
	{
		if (entityid == INTERFACES::Engine->GetLocalPlayer())
		{
			if (nUserID || nDead)
			{
				std::string before = ("[uwuhook] ");
				std::string two = ("Hit ");
				std::string three = killed_info.name;
				std::string hitone = " in ";
				std::string hittwo = HitgroupToName(game_event->GetInt("hitgroup"));
				std::string foura = " for ";
				std::string fivea = game_event->GetString("dmg_health");
				std::string damage = " damage";
				std::string fourb = " (";
				std::string fiveb = game_event->GetString("health");
				std::string six = " health remaining)";
				std::string newline = "\n";
				std::string missed = "Missed shot due spread";

				std::string hsonly = "head";
				std::string dmghsonly = game_event->GetString("dmg_health" + 50);

				if (SETTINGS::settings.damagelogs)
				{
					Msg((before + two + three + hitone + hittwo + foura + fivea + damage + fourb + fiveb + six + newline).c_str());
				}
				else if (SETTINGS::settings.headshotonly)
				{
					Msg((before + two + three + hitone + hsonly + foura + dmghsonly + damage + fourb + fiveb + six + newline).c_str());
				}
			}
		}
	}

	if (!strcmp(game_event->GetName(), "item_purchase"))
	{
		if (nUserID || nDead)
		{
			std::string before = ("[uwuhook] ");
			std::string one_purchase = killed_info.name;
			std::string two_purchase = (" bought ");
			std::string three_purchase = game_event->GetString("weapon");
			std::string six_purchase = "\n";

			if (SETTINGS::settings.purchaselogs)
			{
				Msg((before + one_purchase + two_purchase + three_purchase + six_purchase).c_str());
			}
		}
	}
}

void CGameEvents::RoundStartListener::FireGameEvent(SDK::IGameEvent* game_event)
{
	if (!game_event)
		return;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player)
		return;

	auto entity = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("userid")));
	if (!entity)
		return;

	auto entity_attacker = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("attacker")));

	if (!entity_attacker)
		return;

	if (entity->GetTeam() == local_player->GetTeam())
		return;

	if (SETTINGS::settings.buybot == 0)
	{
		// disabled
		return;
	}
	else if (SETTINGS::settings.buybot == 1)
	{
		INTERFACES::Engine->ClientCmd_Unrestricted("buy g3sg1; buy scar20; buy elite; buy vest; buy vesthelm; buy defuser; buy molotov; buy incgrenade; buy hegrenade; buy smokegrenade;");
		// auto
	}
	else if (SETTINGS::settings.buybot == 2)
	{
		INTERFACES::Engine->ClientCmd_Unrestricted("buy ssg08; buy elite; buy vest; buy vesthelm; buy defuser; buy molotov; buy incgrenade; buy hegrenade; buy smokegrenade;");
		// scout
	}
}

void CGameEvents::BulletImpactListener::FireGameEvent(SDK::IGameEvent* game_event)
{
	if (!game_event)
		return;

	int iUser = INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("userid"));

	auto entity = reinterpret_cast<SDK::CBaseEntity*>(INTERFACES::ClientEntityList->GetClientEntity(iUser));

	if (!entity)
		return;

	if (entity->GetIsDormant())
		return;

	float x, y, z;
	x = game_event->GetFloat("x");
	y = game_event->GetFloat("y");
	z = game_event->GetFloat("z");


	UTILS::BulletImpact_t impact(entity, Vector(x, y, z), INTERFACES::Globals->curtime, iUser == INTERFACES::Engine->GetLocalPlayer() ? GREEN : RED);

	visuals->Impacts.push_back(impact);
}