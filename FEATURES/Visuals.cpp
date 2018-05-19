#include "../includes.h"
#include "../UTILS/interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/ConVar.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/IViewRenderBeams.h"
#include "../FEATURES/Backtracking.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CBaseWeapon.h"
#include "../SDK/CTrace.h"	
#include "../FEATURES/Resolver.h"
#include "../SDK/CGlobalVars.h"
#include "../FEATURES/Visuals.h"
#include "../UTILS/render.h"
#include "../SDK/xor.h"
#include "../FEATURES/AutoWall.h"
#include "../SDK/IVDebugOverlay.h"
#include "../FEATURES/Aimbot.h"
#include "../SDK/ClientRecvProps.h"
#pragma comment(lib, "d3d9.lib")
#include <string.h>
#include <d3d9.h>

#pragma comment(lib, "Winmm.lib")

//--- Misc Variable Initalization ---//
int alpha[65];
CColor breaking;
CColor backtrack;
static bool bPerformed = false, bLastSetting;
float fade_alpha[65];
float dormant_time[65];
CColor main_color;
CColor ammo;
float flPlayerAlpha[64];
SDK::CBaseEntity *BombCarrier;



inline void CrossProduct(const Vector& a, const Vector& b, Vector& result)
{
	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
}

inline Vector CrossProduct(const Vector& a, const Vector& b)
{
	return Vector(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}

__forceinline float VecDotProduct(const Vector& a, const Vector& b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

void CVisuals::Draw()
{
	if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
	{
		for (int i = 1; i < 65; i++)
		{
			auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
			auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

			if (!entity)
				continue;

			if (!local_player)
				continue;

			bool is_local_player = entity == local_player;
			bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;

			if (SETTINGS::settings.selfesp)
			{
				if (!is_local_player)
					continue;
				else if (is_local_player)
					continue;
			}

			if (is_local_player)
				continue;

			if (is_teammate)
				continue;

			if (entity->GetHealth() <= 0)
				continue;

			if (SETTINGS::settings.localesp == false)
			{
				if (is_local_player)
					continue;
			}
			if (SETTINGS::settings.teamesp == false)
			{
				if (is_teammate)
					continue;
			}

			//--- Colors ---//
			int enemy_hp = entity->GetHealth();
			int hp_red = 255 - (enemy_hp * 2.55);
			int hp_green = enemy_hp * 2.55;
			CColor health_color = CColor(hp_red, hp_green, 1, alpha[entity->GetIndex()]);
			CColor dormant_color = CColor(100, 100, 100, alpha[entity->GetIndex()]);
			CColor box_color;
			CColor skeleton_color;
			CColor still_health;
			CColor alt_color;
			CColor zoom_color;
			CColor bomb_color;

			//--- Domant ESP Checks ---//
			if (entity->GetIsDormant())
			{
				if (alpha[entity->GetIndex()] > 0)
					alpha[entity->GetIndex()] -= 0.4;
				main_color = dormant_color;
				still_health = dormant_color;
				alt_color = dormant_color;
				zoom_color = dormant_color;
				breaking = dormant_color;
				backtrack = dormant_color;
				box_color = dormant_color;
				bomb_color = dormant_color;
				ammo = dormant_color;
			}
			else if (!entity->GetIsDormant())
			{
				alpha[entity->GetIndex()] = 255;
				main_color = CColor(255, 255, 255, alpha[entity->GetIndex()]); //heath_color
				still_health = health_color;
				alt_color = BLACK;
				zoom_color = CColor(150, 150, 220, 255);
				breaking = CColor(220, 150, 150);
				backtrack = CColor(155, 220, 150);
				box_color = SETTINGS::settings.box_col;
				skeleton_color = SETTINGS::settings.skeleton_col;
				bomb_color = CColor(244, 66, 66, 165);
				ammo = CColor(61, 135, 255, 165);
			}

			if (SETTINGS::settings.recoil_circle)
			{
				auto pLocal = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

				if (!pLocal)
					return;
				auto pWeapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
				if (!(pLocal->GetHealth()))
					return;


				int xs;
				int ys;
				INTERFACES::Engine->GetScreenSize(xs, ys);
				xs /= 2; ys /= 2;

				auto accuracy = pWeapon->GetInaccuracy() * 700.f; //3000

				RENDER::DrawCircle(xs, ys, accuracy, 60, SETTINGS::settings.spread_col);
			}

			if (!local_player)
				return;

			//DoEnemyCircle(entity->GetAbsOrigin());

			ClientClass* cClass = (ClientClass*)entity->GetClientClass();

			//--- Entity Related Rendering ---///
			if (SETTINGS::settings.xhair_type) DrawCrosshair();
			if (SETTINGS::settings.box_type) DrawBox(entity, box_color);
			if (SETTINGS::settings.name_bool) DrawName(entity, main_color, i);
			if (SETTINGS::settings.weap_bool) DrawWeapon(entity, main_color, i);
			if (SETTINGS::settings.info_bool) DrawInfo(entity, main_color, zoom_color); DrawDistance(entity, main_color);
			if (SETTINGS::settings.distance) DrawDistance(entity, main_color);
			if (SETTINGS::settings.health_bool) DrawHealth(entity, still_health, alt_color);
			if (SETTINGS::settings.drawammo) DrawAmmo(entity, ammo, alt_color);
			if (SETTINGS::settings.drawscopeinfo) DrawScopeInfo();
			if (SETTINGS::settings.offscreen_esp) DrawFovArrows(entity);
			if (SETTINGS::settings.drawpenetration) DrawPenetration(entity);
			if (SETTINGS::settings.drawc4timer)
			{
				if (cClass->m_ClassID == (int)SDK::ItemDefinitionIndex::WEAPON_CPlantedC4)
					DrawBombPlanted(entity, alt_color);

				if (cClass->m_ClassID == (int)SDK::ItemDefinitionIndex::WEAPON_CPlantedC4)
					BombTimer(entity, alt_color);

				if (cClass->m_ClassID == (int)SDK::ItemDefinitionIndex::WEAPON_C4)
					DrawBomb(entity, alt_color);
			}
		
			//if (SETTINGS::settings.drawc4timer) DrawBombPlanted(entity, main_color), BombTimer(entity, main_color), DrawBomb(entity, main_color);
		}
	}
}
void CVisuals::ClientDraw()
{
	DrawBorderLines();
	DrawIndicator();
	ModulateWorld();
}

std::string str_to_upper(std::string strToConvert)
{
	std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), ::toupper);

	return strToConvert;
}

void CVisuals::DrawBox(SDK::CBaseEntity* entity, CColor color)
{
	Vector Bot, Top;
	Vector bbmin; //another way of doing it, doesn't look gae tho
	Vector bbmax;
	Vector screen1;
	Vector screen2;
	entity->GetRenderBounds(bbmin, bbmax);
	Vector pos3D = entity->GetAbsOrigin() - Vector(0, 0, 10);
	Vector top3D = pos3D + Vector(0, 0, bbmax.z + 10);

	if (RENDER::WorldToScreen(pos3D, screen1) && RENDER::WorldToScreen(top3D, screen2))
	{
	float height = screen1.y - screen2.y;
	float width = height / 4;
	RENDER::DrawEmptyRect(screen2.x - width, screen2.y, screen1.x + width, screen1.y, color); //main
	RENDER::DrawEmptyRect(screen2.x - width - 1, screen2.y - 1, screen1.x + width + 1, screen1.y + 1, CColor(20, 20, 20, alpha[entity->GetIndex()])); //outline
	RENDER::DrawEmptyRect(screen2.x - width + 1, screen2.y + 1, screen1.x + width - 1, screen1.y - 1, CColor(20, 20, 20, alpha[entity->GetIndex()])); //inline
	}
}

void CVisuals::DrawCorners(SDK::CBaseEntity* entity, CColor color)
{
	Vector min, max;
	entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 10);

	if (RENDER::WorldToScreen(pos3D, pos) && RENDER::WorldToScreen(top3D, top))
	{
		int height = (pos.y - top.y);
		int width = height / 2;

		RENDER::DrawEdges(pos.x - width / 2, top.y, (pos.x - width / 2) + width, top.y + height, 8, color);
		RENDER::DrawEdges((pos.x - width / 2) + 1, top.y + 1, (pos.x - width / 2) + width - 1, top.y + height - 1, 8, CColor(20, 20, 20, alpha[entity->GetIndex()]));
		RENDER::DrawEdges((pos.x - width / 2) - 1, top.y - 1, (pos.x - width / 2) + width + 1, top.y + height + 1, 8, CColor(20, 20, 20, alpha[entity->GetIndex()]));
	}
}

void CVisuals::DrawName(SDK::CBaseEntity* entity, CColor color, int index)
{
	SDK::player_info_t ent_info;
	Vector min, max;
	entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 10);
	INTERFACES::Engine->GetPlayerInfo(index, &ent_info);

	if (RENDER::WorldToScreen(pos3D, pos) && RENDER::WorldToScreen(top3D, top))
	{
		wchar_t buffer[128];
		wsprintfW(buffer, L"%S", ent_info.name);
		if (MultiByteToWideChar(CP_UTF8, 0, ent_info.name, -1, buffer, 128) > 0)
		{
			int height = (pos.y - top.y);
			int width = height / 2;
			RENDER::DrawF(pos.x, top.y - 7, FONTS::visuals_name_font, true, true, color, ent_info.name); //numpad_menu_font
		}
	}
}

float CVisuals::resolve_distance(Vector src, Vector dest)
{
	Vector delta = src - dest;

	float fl_dist = ::sqrtf((delta.Length()));

	if (fl_dist < 1.0f)
		return 1.0f;

	return fl_dist;
}


void CVisuals::DrawDistance(SDK::CBaseEntity* entity, CColor color)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	SDK::player_info_t ent_info;
	Vector min, max;
	entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 10);
	Vector vecOrigin = entity->GetVecOrigin();
	Vector vecOriginLocal = local_player->GetVecOrigin();

	if (!SETTINGS::settings.distance) return;
	char dist_to[32];
	sprintf_s(dist_to, "%.0f ft", resolve_distance(vecOriginLocal, vecOrigin));

	if (RENDER::WorldToScreen(pos3D, pos) && RENDER::WorldToScreen(top3D, top))
	{
		int height = (pos.y - top.y);
		int width = height / 2;
		RENDER::DrawF(pos.x, pos.y + 8, FONTS::visuals_esp_font, true, true, color, dist_to);
	}
}

std::string fix_item_name(std::string name) {
	std::string cname = name;

	if (cname[0] == 'C')
		cname.erase(cname.begin());

	auto startOfWeap = cname.find("Weapon");
	if (startOfWeap != std::string::npos)
		cname.erase(cname.begin() + startOfWeap, cname.begin() + startOfWeap + 6);

	return cname;
}

std::string revolver(std::string name) {
	std::string cname = name;
	std::string revolver = "R8 Revolver";

	if (cname[0] == 'C')
		cname.erase(cname.begin());

	auto startOfWeap = cname.find("");
	if (startOfWeap != std::string::npos)
		cname.erase(revolver.begin() + startOfWeap, revolver.begin() + startOfWeap + 6);

	return cname;
}
void CVisuals::DrawWeapon(SDK::CBaseEntity* entity, CColor color, int index) {
	SDK::player_info_t ent_info;
	Vector min, max;
	entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 10);
	INTERFACES::Engine->GetPlayerInfo(index, &ent_info);

	auto weapon = INTERFACES::ClientEntityList->GetClientEntity(entity->GetActiveWeaponIndex());
	auto c_baseweapon = reinterpret_cast< SDK::CBaseWeapon* >(INTERFACES::ClientEntityList->GetClientEntity(entity->GetActiveWeaponIndex()));

	if (!c_baseweapon)
		return;


	if (!weapon)
		return;

	if (RENDER::WorldToScreen(pos3D, pos) && RENDER::WorldToScreen(top3D, top)) {
		int height = (pos.y - top.y);
		int width = height / 2;

		switch (SETTINGS::settings.wep_type) {
		case 0:
			break;
		case 1:
			if (c_baseweapon->isrevolver()) {
				RENDER::DrawF(pos.x, pos.y + 16, FONTS::visuals_esp_font, true, true, color, "r8 revolver (" + std::to_string(c_baseweapon->GetLoadedAmmo()) + ")");
			}
			else {
				if (c_baseweapon->isknife_entity())
					RENDER::DrawF(pos.x, pos.y + 16, FONTS::visuals_esp_font, true, true, color, fix_item_name(weapon->GetClientClass()->m_pNetworkName));
				else
					RENDER::DrawF(pos.x, pos.y + 16, FONTS::visuals_esp_font, true, true, color, fix_item_name(weapon->GetClientClass()->m_pNetworkName)
						+ " (" + std::to_string(c_baseweapon->GetLoadedAmmo()) + ")");

			}
			break;
		case 2:
			RENDER::DrawF(pos.x, pos.y + 19, FONTS::visuals_weapon_icon_font, true, true, color, c_baseweapon->GetGunIcon());
			break;
		}
	}
}
	

void CVisuals::DrawSkeleton(SDK::CBaseEntity* entity, CColor color, VMatrix bone_matrix[128]) //wut hek
{
	SDK::studiohdr_t* pStudioModel = INTERFACES::ModelInfo->GetStudioModel(entity->GetModel());
	if (!pStudioModel)
		return;

	for (int i = 0; i < pStudioModel->numbones; i++) {
		SDK::mstudiobone_t* pBone = pStudioModel->pBone(i);
		if (!pBone || !(pBone->flags & 256) || pBone->parent == -1)
			continue;

		Vector vBonePos1;
		if (!RENDER::WorldToScreen(Vector(bone_matrix[i][0][3], bone_matrix[i][1][3], bone_matrix[i][2][3]), vBonePos1))
			continue;

		Vector vBonePos2;
		if (!RENDER::WorldToScreen(Vector(bone_matrix[pBone->parent][0][3], bone_matrix[pBone->parent][1][3], bone_matrix[pBone->parent][2][3]), vBonePos2))
			continue;

		RENDER::DrawLine((int)vBonePos1.x, (int)vBonePos1.y, (int)vBonePos2.x, (int)vBonePos2.y, color);
	}
}

void CVisuals::DrawBombPlanted(SDK::CBaseEntity* pEntity, CColor color)
{

	BombCarrier = nullptr;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetAbsOrigin();
	SDK::CCSBomb* Bomb = (SDK::CCSBomb*)pEntity;

	float flBlow = Bomb->GetC4BlowTime();
	float TimeRemaining = flBlow - (INTERFACES::Globals->interval_per_tick * local_player->GetTickBase());
	char buffer[64];
	sprintf_s(buffer, "%.1fs", TimeRemaining);
	float TimeRemaining2;
	bool exploded = true;
	if (TimeRemaining < 0)
	{
		!exploded;

		TimeRemaining2 = 0;
	}
	else
	{
		exploded = true;
		TimeRemaining2 = TimeRemaining;
	}
	if (exploded)
	{

		sprintf_s(buffer, "%.1f", TimeRemaining2);
	}
	else
	{

		sprintf_s(buffer, "", TimeRemaining2);
	}

	RENDER::DrawF(10, 45, FONTS::visuals_lby_font, false, false, CColor(0, 255, 0, 255), buffer);

}

void CVisuals::BombTimer(SDK::CBaseEntity* pEntity, CColor color)
{
	BombCarrier = nullptr;

	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetAbsAngles();
	SDK::CCSBomb* Bomb = (SDK::CCSBomb*)pEntity;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	if (RENDER::WorldToScreen(vOrig, vScreen))
	{
		float flBlow = Bomb->GetC4BlowTime();
		float TimeRemaining = flBlow - (INTERFACES::Globals->interval_per_tick * local_player->GetTickBase());
		float TimeRemaining2;
		bool exploded = true;
		if (TimeRemaining < 0)
		{
			!exploded;

			TimeRemaining2 = 0;
		}
		else
		{
			exploded = true;
			TimeRemaining2 = TimeRemaining;
		}
		char buffer[64];
		if (exploded)
		{
			;
			sprintf_s(buffer, "%.1f", TimeRemaining2);
		}
		else
		{

			sprintf_s(buffer, "FATAL", TimeRemaining2);
		}
		RENDER::DrawF(vScreen.x, vScreen.y, FONTS::supremacy, false, false, CColor(255, 255, 255, 255), buffer);
	}
}

void CVisuals::DrawBomb(SDK::CBaseEntity* pEntity, CColor color)
{

	BombCarrier = nullptr;
	SDK::CBaseWeapon *BombWeapon = (SDK::CBaseWeapon *)pEntity;
	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetAbsOrigin();
	bool adopted = true;
	HANDLE parent = BombWeapon->GetOwnerHandle();
	if (parent || (vOrig.x == 0 && vOrig.y == 0 && vOrig.z == 0))
	{
		SDK::CBaseEntity* pParentEnt = (INTERFACES::ClientEntityList->GetClientEntityFromHandle(parent));
		if (pParentEnt && pParentEnt->IsAlive())
		{

			BombCarrier = pParentEnt;
			adopted = false;
		}
	}

	if (adopted)
	{
		if (RENDER::WorldToScreen(vOrig, vScreen))
		{
			RENDER::DrawF(vScreen.x, vScreen.y, FONTS::supremacy, false, false, CColor(112, 230, 20, 255), "bomb");
		}
	}
}

void CVisuals::DrawHealth(SDK::CBaseEntity* entity, CColor color, CColor dormant)
{
	Vector min, max;
	entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 10);

	int enemy_hp = entity->GetHealth();
	int hp_red = 255 - (enemy_hp * 2.55);
	int hp_green = enemy_hp * 2.55;
	CColor health_color = CColor(hp_red, hp_green, 1, alpha[entity->GetIndex()]);

	if (RENDER::WorldToScreen(pos3D, pos) && RENDER::WorldToScreen(top3D, top))
	{
		int height = (pos.y - top.y);
		int width = height / 2;

		float offset = (height / 4.f) + 5;
		UINT hp = height - (UINT)((height * enemy_hp) / 100);

		RENDER::DrawEmptyRect((pos.x - width / 2) - 6, top.y - 1, (pos.x - width / 2) - 3, top.y + height + 1, dormant); //intense maths
		RENDER::DrawLine((pos.x - width / 2) - 4, top.y + hp, (pos.x - width / 2) - 4, top.y + height, color); //could have done a rect here,
		RENDER::DrawLine((pos.x - width / 2) - 5, top.y + hp, (pos.x - width / 2) - 5, top.y + height, color); //but fuck it

		if (entity->GetHealth() < 100)
			RENDER::DrawF((pos.x - width / 2) - 4, top.y + hp, FONTS::visuals_esp_font, true, true, main_color, std::to_string(enemy_hp));
	}
}
template <typename t> t xdd(t value, t min, t max) {
	if (value > max) {
		return max;
	}
	if (value < min) {
		return min;
	}
	return value;
}


void CVisuals::DrawPenetration(SDK::CBaseEntity* entity)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	Vector point;
	INTERFACES::Engine->GetViewAngles(point);

	int screen_x;
	int screen_y;
	INTERFACES::Engine->GetScreenSize(screen_x, screen_y);
	screen_x /= 2; screen_y /= 2;

	Vector local_position = local_player->GetVecOrigin() + local_player->GetViewOffset();

	if (autowall->CalculateDamage(local_position, point, local_player, entity).damage > SETTINGS::settings.damage_val > SETTINGS::settings.auto_damage_val > SETTINGS::settings.other_damage_val > SETTINGS::settings.scout_damage_val > SETTINGS::settings.zeus_damage_val)
		RENDER::DrawFilledRect(screen_x - 1, screen_y - 1, screen_x, screen_y, GREEN);
	else
		RENDER::DrawFilledRect(screen_x - 1, screen_y - 1, screen_x, screen_y, RED);
}

void CVisuals::DrawAmmo(SDK::CBaseEntity* entity, CColor color, CColor dormant)
{
	Vector min, max;
	entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 10);

	int enemy_hp = entity->GetHealth();
	int hp_red = 255 - (enemy_hp * 2.55);
	int hp_green = enemy_hp * 2.55;
	CColor health_color = CColor(hp_red, hp_green, 1, alpha[entity->GetIndex()]);

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	bool is_local_player = entity == local_player;
	bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;

	if (is_local_player)
		return;

	if (is_teammate)
		return;

	auto c_baseweapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(entity->GetActiveWeaponIndex()));

	if (!c_baseweapon)
		return;

	if (RENDER::WorldToScreen(pos3D, pos) && RENDER::WorldToScreen(top3D, top))
	{
		int height = (pos.y - top.y);

		float offset = (height / 4.f) + 5;
		UINT hp = height - (UINT)((height * 3) / 100);

		auto animLayer = entity->GetAnimOverlay(1);
		if (!animLayer.m_pOwner)
			return;

		auto activity = entity->GetSequenceActivity(animLayer.m_nSequence);

		int iClip = c_baseweapon->GetLoadedAmmo();
		int iClipMax = c_baseweapon->get_full_info()->iMaxClip1;

		float box_w = (float)fabs(height / 2);
		float width;
		if (activity == 967 && animLayer.m_flWeight != 0.f)
		{
			float cycle = animLayer.m_flCycle; // 1 = finished 0 = just started
			width = (((box_w * cycle) / 1.f));
		}
		else
			width = (((box_w * iClip) / iClipMax));

		RENDER::DrawFilledRect((pos.x - box_w / 2), top.y + height + 3, (pos.x - box_w / 2) + box_w + 2, top.y + height + 7, dormant); //outline
		RENDER::DrawFilledRect((pos.x - box_w / 2) + 1, top.y + height + 4, (pos.x - box_w / 2) + width + 1, top.y + height + 6, color); //ammo
	}
}
void CVisuals::BombPlanted(SDK::CBaseEntity* entity)
{
	BombCarrier = nullptr;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	Vector vOrig; Vector vScreen;
	vOrig = entity->GetVecOrigin();
	SDK::CCSBomb* Bomb = (SDK::CCSBomb*)entity;

	float flBlow = Bomb->GetC4BlowTime();
	float TimeRemaining = flBlow;// -(INTERFACES::Globals->interval_per_tick * local_player->GetTickBase());
	char buffer[64];
	sprintf_s(buffer, "B - %.1fs", TimeRemaining);
	RENDER::DrawF(10, 10, FONTS::visuals_lby_font, false, false, CColor(124, 195, 13, 255), buffer);
}

void CVisuals::DrawInfo(SDK::CBaseEntity* entity, CColor color, CColor alt)
{
	std::vector<std::pair<std::string, CColor>> stored_info;
	Vector min, max;
	entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 10);

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	auto c_baseweapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(entity->GetActiveWeaponIndex()));

	if (!c_baseweapon)
		return;

	CColor Orange;
	CColor Blue;
	CColor Red;
	CColor White;

	CColor dormant_color = CColor(100, 100, 100, alpha[entity->GetIndex()]);
	if (entity->GetIsDormant())
	{
		if (alpha[entity->GetIndex()] > 0)
			alpha[entity->GetIndex()] -= 0.4;

		Orange = dormant_color;
		Blue = dormant_color;
		Red = dormant_color;
		White = dormant_color;
	}
	else if (!entity->GetIsDormant())
	{
		Orange = CColor(255, 165, 0, alpha[entity->GetIndex()]);
		Blue = CColor(0, 50, 225, alpha[entity->GetIndex()]);
		Red = CColor(200, 0, 0, alpha[entity->GetIndex()]);
		White = CColor(255, 255, 255, alpha[entity->GetIndex()]);
	}

		if (resolve_type[entity->GetIndex()] == 1)
			stored_info.push_back(std::pair<std::string, CColor>("MOVING", color));
		else if (resolve_type[entity->GetIndex()] == 2)
			stored_info.push_back(std::pair<std::string, CColor>("IN AIR", color));
		else if (resolve_type[entity->GetIndex()] == 3)
			stored_info.push_back(std::pair<std::string, CColor>("LAST LBY UPDATE", color));
		else if (resolve_type[entity->GetIndex()] == 4)
			stored_info.push_back(std::pair<std::string, CColor>("SHUFFLE", color));
		else if (resolve_type[entity->GetIndex()] == 5)
			stored_info.push_back(std::pair<std::string, CColor>("LAST MOVING", color));
		else if (resolve_type[entity->GetIndex()] == 7)
			stored_info.push_back(std::pair<std::string, CColor>("LBY BACKTRACK", color));

		if (using_fake_angles[entity->GetIndex()] && entity->GetVelocity().Length2D() < 0.1 || using_fake_angles[entity->GetIndex()] && !(entity->GetFlags() & FL_ONGROUND) || using_fake_angles[entity->GetIndex()] && !backtrack_tick[entity->GetIndex()])
			stored_info.push_back(std::pair<std::string, CColor>("FAKE", White));
		static float time[64];


		static float LBY[64];

		float lby = entity->GetLowerBodyYaw();

		int Index = entity->GetIndex();

		if (lby != LBY[Index] || (entity->GetVelocity().Length2D() > 0.1f && entity->GetFlags() & FL_ONGROUND)) {
			LBY[Index] = lby;
			time[Index] = INTERFACES::Globals->curtime;
		}

		if (entity->GetVelocity().Length2D() < 0.1);
			stored_info.push_back(std::pair<std::string, CColor>("last update: " + std::to_string(INTERFACES::Globals->interval_per_tick - time[entity->GetIndex()]), CColor(0, 175, 0, alpha[entity->GetIndex()])));

		if (entity->GetArmor() > 0)
			stored_info.push_back(std::pair<std::string, CColor>(entity->GetArmorName(), Blue));

		if (entity->GetIsScoped())
			stored_info.push_back(std::pair<std::string, CColor>("ZOOM", Blue));

		stored_info.push_back(std::pair<std::string, CColor>("$" + std::to_string(entity->GetMoney()), backtrack));
	

	if (RENDER::WorldToScreen(pos3D, pos) && RENDER::WorldToScreen(top3D, top))
	{
		int height = (pos.y - top.y);
		int width = height / 2;
		int i = 0;
		for (auto Text : stored_info)
		{
			RENDER::DrawF((pos.x + width / 2) + 5, top.y + i, FONTS::visuals_esp_font, false, false, Text.second, Text.first); //numpad_menu_font
			i += 8;
		}
	}
}
	

void CVisuals::DrawBulletBeams()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;
	//
	if (!INTERFACES::Engine->IsInGame() || !local_player)
	{
		Impacts.clear();
		return;
	}

	if (Impacts.size() > 30)
		Impacts.pop_back();

	for (int i = 0; i < Impacts.size(); i++)
	{
		auto current = Impacts.at(i);

		if (!current.pPlayer)
			continue;

		if (current.pPlayer->GetIsDormant())
			continue;

		bool is_local_player = current.pPlayer == local_player;
		bool is_teammate = local_player->GetTeam() == current.pPlayer->GetTeam() && !is_local_player;

		if (current.pPlayer == local_player)
			current.color = SETTINGS::settings.localbeam_col;
		else if (current.pPlayer != local_player && !is_teammate)
			current.color = SETTINGS::settings.enemybeam_col;
		else if (current.pPlayer != local_player && is_teammate)
			current.color = SETTINGS::settings.friendlybeam_col;

		SDK::BeamInfo_t beamInfo;
		beamInfo.m_nType = SDK::TE_BEAMPOINTS;
		beamInfo.m_pszModelName = "sprites/purplelaser1.vmt";
		beamInfo.m_nModelIndex = -1;
		beamInfo.m_flHaloScale = 0.0f;
		beamInfo.m_flLife = 3.0f;
		beamInfo.m_flWidth = 2.0f;
		beamInfo.m_flEndWidth = 2.0f;
		beamInfo.m_flFadeLength = 0.0f;
		beamInfo.m_flAmplitude = 2.0f;
		beamInfo.m_flBrightness = 255.f;
		beamInfo.m_flSpeed = 0.2f;
		beamInfo.m_nStartFrame = 0;
		beamInfo.m_flFrameRate = 0.f;
		beamInfo.m_flRed = current.color.RGBA[0];
		beamInfo.m_flGreen = current.color.RGBA[1];
		beamInfo.m_flBlue = current.color.RGBA[2];
		beamInfo.m_nSegments = 2;
		beamInfo.m_bRenderable = true;
		beamInfo.m_nFlags = SDK::FBEAM_ONLYNOISEONCE | SDK::FBEAM_NOTILE | SDK::FBEAM_HALOBEAM;

		beamInfo.m_vecStart = current.pPlayer->GetVecOrigin() + current.pPlayer->GetViewOffset();
		beamInfo.m_vecEnd = current.vecImpactPos;

		auto beam = INTERFACES::ViewRenderBeams->CreateBeamPoints(beamInfo);
		if (beam)
			INTERFACES::ViewRenderBeams->DrawBeam(beam);

		Impacts.erase(Impacts.begin() + i);
	}
}

void CVisuals::DrawCrosshair()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	if (local_player->GetHealth() <= 0)
		return;

	auto weapon = INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex());

	if (!weapon)
		return;

	//--- Screen Positioning ---//
	static int screen_x;
	static int screen_y;
	if (screen_x == 0)
	{
		INTERFACES::Engine->GetScreenSize(screen_x, screen_y);
		screen_x /= 2; screen_y /= 2;
	}

	//--- Calculating Recoil ---//

	Vector view_angles;
	INTERFACES::Engine->GetViewAngles(view_angles);
	view_angles += local_player->GetPunchAngles() * 2.f;
	Vector factor_vec;
	MATH::AngleVectors(view_angles, &factor_vec);
	factor_vec = factor_vec * 10000.f;
	Vector start = local_player->GetVecOrigin() + local_player->GetViewOffset();
	Vector end = start + factor_vec, recoil_to_screen;
	//--- Calculating Spread ---//

		auto cone = weapon->GetSpread() + weapon->GetInaccuracy();//*500.f;
		auto spread = weapon->GetSpread();
		if (!spread)
			return;
		if (!cone)
			return;
		//int spread;
	RENDER::DrawFilledCircle(recoil_to_screen.x, recoil_to_screen.y, spread, 50, CColor(0, 0, 0, 100));
	//--- Rendering Crosshairs ---//
	switch (SETTINGS::settings.xhair_type % 3)
	{
	case 0:
		break;
	case 1:
		RENDER::DrawF(screen_x, screen_y, FONTS::visuals_xhair_font, true, true, WHITE, "+");
		break;
	case 2:
		if (RENDER::WorldToScreen(end, recoil_to_screen) && local_player->GetHealth() > 0)
			RENDER::DrawF(recoil_to_screen.x, recoil_to_screen.y, FONTS::visuals_xhair_font, true, true, WHITE, "+");
		break;
	}
}
/*(void CVisuals::SpecList()
{

	RECT scrn = RENDER::GetViewport();
	int ayy = 0;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	// Loop through all active entitys
	for (int i = 0; i < INTERFACES::ClientEntityList->GetHighestEntityIndex(); i++)
	{

		// Get the entity
		SDK::CBaseEntity *pEntity = INTERFACES::ClientEntityList->GetClientEntity(i);
		SDK::player_info_t pinfo;

		// The entity isn't some laggy peice of shit or something
		if (pEntity &&  pEntity != local_player)
		{
			if (INTERFACES::Engine->GetPlayerInfo(i, &pinfo) && !pEntity->IsAlive() && !pEntity->IsDormant())
			{
				HANDLE obs = pEntity->GetClientEntityFromHandle();

				if (obs)
				{
					SDK::CBaseEntity *pTarget = INTERFACES::ClientEntityList->GetClientEntityFromHandle(obs);
					SDK::player_info_t pinfo2;
					if (pTarget)
					{

						if (INTERFACES::Engine->GetPlayerInfo(pTarget->GetIndex(), &pinfo2))
						{
							if (strlen(pinfo.name) > 16)
							{

								pinfo.name[12] = 0;
								strcat(pinfo.name, "...");
								RECT TextSize = RENDER::GetTextSize2(FONTS::visuals_esp_font, pinfo.name);
								RECT nameSize = RENDER::GetTextSize2(FONTS::visuals_esp_font, pinfo.name);
								RENDER::TextRenderXD(scrn.right - TextSize.right - 4, (scrn.top / 4) + (16 * ayy), pTarget->GetIndex() == local_player->GetIndex() ? CColor(255, 255, 255, 255) : CColor(255, 255, 255, 255), FONTS::visuals_esp_font, pinfo.name);
								ayy++;
							}
						}

					}
				}

			}
		}
	}

}*/

void CVisuals::DrawIndicator()
{
	//--- LBY Indication Stuff ---//
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	if (local_player->GetHealth() <= 0)
		return;

	if (menu_hide)
		return;
	float breaking_lby_fraction = fabs(MATH::NormalizeYaw(GLOBAL::real_angles.y - local_player->GetLowerBodyYaw())) / 180.f;
	float lby_delta = abs(MATH::NormalizeYaw(GLOBAL::real_angles.y - local_player->GetLowerBodyYaw()));

	int screen_width, screen_height;
	INTERFACES::Engine->GetScreenSize(screen_width, screen_height);

	if (SETTINGS::settings.showlbydelta && SETTINGS::settings.aa_type == 2 || SETTINGS::settings.aa_type == 4)
	{
		RENDER::DrawF(10, screen_height / 2, FONTS::numpad_menu_font, false, false, CColor((1.f - breaking_lby_fraction) * 255.f, breaking_lby_fraction * 255.f, 0), "lby delta: " + std::to_string(lby_delta));
	}

	if (SETTINGS::settings.aa_type == 2 || SETTINGS::settings.aa_type == 4)
	{
		if (SETTINGS::settings.antiaimside)
		{
			if (SETTINGS::settings.flip_bool)
			{
				RENDER::DrawF(10, screen_height - 130, FONTS::visuals_lby_lc_arrow_font, false, false, CColor(137, 170, 42), "LEFT"); //green
			}
			else
			{
				RENDER::DrawF(10, screen_height - 130, FONTS::visuals_lby_lc_arrow_font, false, false, CColor(137, 170, 42), "RIGHT"); //green
			}
		}
	}

	if (SETTINGS::settings.lbyindicator)
	{
		static CColor lby;

		if (local_player->GetVelocity().Length2D() > 0.1f && !fake_walk && local_player->GetFlags() & FL_ONGROUND) //fixes moving delay with fast moving angles, thanks red
		{
			lby = CColor(255, 0, 0);
		}
		else
		{
			if (lby_delta > 35)
			{
				lby = CColor(137, 170, 42);
			}
			else
			{
				lby = CColor(255, 0, 0);
			}
		}

		RENDER::DrawF(10, screen_height - 88, FONTS::visuals_lby_lc_arrow_font, false, false, lby, "LBY");
	}

	if (SETTINGS::settings.lowfpswarning)
	{
		Vector pre_position;
		Vector origin_delta;
		if (local_player->GetVecOrigin() != pre_position)
		{
			origin_delta = local_player->GetVecOrigin() - pre_position;
			pre_position = local_player->GetVecOrigin();
		}
		const auto breaking_lc = origin_delta.Length() > 64;
		if (local_player->GetVelocity().Length2D() >= 300)
		{
			if (breaking_lc)
				RENDER::DrawF(10, screen_height - 120, FONTS::visuals_lby_lc_arrow_font, false, false, CColor(137, 170, 42), "");
			else
				RENDER::DrawF(10, screen_height - 120, FONTS::visuals_lby_lc_arrow_font, false, false, CColor(255, 0, 0), "");
		}
		if (INTERFACES::Globals->frametime <= 50)
		{
			if (breaking_lc)
				RENDER::DrawF(10, screen_height - 120, FONTS::visuals_lby_lc_arrow_font, false, false, CColor(255, 0, 0), "LC");
		}

		if (!SETTINGS::settings.aa_bool)
			return;
	}



	if (SETTINGS::settings.lcindicator)
	{
		Vector pre_position;
		Vector origin_delta;
		if (local_player->GetVecOrigin() != pre_position)
		{
		origin_delta = local_player->GetVecOrigin() - pre_position;
		pre_position = local_player->GetVecOrigin();
		}
		const auto breaking_lc = origin_delta.Length() > 64;
		if (local_player->GetVelocity().Length2D() >= 300)
		{
			if (breaking_lc)
				RENDER::DrawF(10, screen_height - 120, FONTS::visuals_lby_lc_arrow_font, false, false, CColor(137, 170, 42), "LC");
			else
				RENDER::DrawF(10, screen_height - 120, FONTS::visuals_lby_lc_arrow_font, false, false, CColor(255, 0, 0), "LC");
		}
		if (local_player->GetVelocity().Length2D() <= 299)
		{
			if (breaking_lc)
				RENDER::DrawF(10, screen_height - 120, FONTS::visuals_lby_lc_arrow_font, false, false, CColor(255, 0, 0), "LC");
		}

		if (!SETTINGS::settings.aa_bool)
			return;
	}
	if (SETTINGS::settings.fakeanglerotation)
	{
		static const auto fake_color = CColor(255, 255, 0);


		auto client_viewangles = Vector();
		INTERFACES::Engine->GetViewAngles(client_viewangles);

		constexpr auto radius = 80.f;

		const auto screen_center = Vector2D(screen_width / 2.f, screen_height / 2.f);
		const auto fake_rot = DEG2RAD(client_viewangles.y - GLOBAL::fake_angles.y - 90);

		auto draw_arrow = [&](float rot, CColor color) -> void
		{
			std::vector<SDK::Vertex_t> vertices;
			vertices.push_back(SDK::Vertex_t(Vector2D(screen_center.x + cosf(rot) * radius, screen_center.y + sinf(rot) * radius)));
			vertices.push_back(SDK::Vertex_t(Vector2D(screen_center.x + cosf(rot + DEG2RAD(10)) * (radius - 15.f), screen_center.y + sinf(rot + DEG2RAD(10)) * (radius - 15.f)))); //25
			vertices.push_back(SDK::Vertex_t(Vector2D(screen_center.x + cosf(rot - DEG2RAD(10)) * (radius - 15.f), screen_center.y + sinf(rot - DEG2RAD(10)) * (radius - 15.f)))); //25
			RENDER::TexturedPolygon(3, vertices, color);
		};

		draw_arrow(fake_rot, fake_color);
	}
}


void CVisuals::ModulateWorld() //credits to my nigga monarch
{
	static bool nightmode_performed = false, nightmode_lastsetting;

	if (!INTERFACES::Engine->IsConnected() || !INTERFACES::Engine->IsInGame())
	{
		if (nightmode_performed)
			nightmode_performed = false;
		return;
	}

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	if (!local_player)
	{
		if (nightmode_performed)
			nightmode_performed = false;
		return;
	}

	if (nightmode_lastsetting != SETTINGS::settings.night_bool)
	{
		nightmode_lastsetting = SETTINGS::settings.night_bool;
		nightmode_performed = false;
	}

	if (!nightmode_performed)
	{
		static SDK::ConVar* r_DrawSpecificStaticProp = INTERFACES::cvar->FindVar("r_DrawSpecificStaticProp");
		r_DrawSpecificStaticProp->nFlags &= ~FCVAR_CHEAT;

		static SDK::ConVar* sv_skyname = INTERFACES::cvar->FindVar("sv_skyname");
		sv_skyname->nFlags &= ~FCVAR_CHEAT;

		for (SDK::MaterialHandle_t i = INTERFACES::MaterialSystem->FirstMaterial(); i != INTERFACES::MaterialSystem->InvalidMaterial(); i = INTERFACES::MaterialSystem->NextMaterial(i))
		{
			SDK::IMaterial *pMaterial = INTERFACES::MaterialSystem->GetMaterial(i);

			if (!pMaterial)
				continue;

			if (strstr(pMaterial->GetTextureGroupName(), "World"))
			{
				if (SETTINGS::settings.night_bool)
					pMaterial->ColorModulate(0.08, 0.08, 0.05);
				else
					pMaterial->ColorModulate(1, 1, 1);

				if (SETTINGS::settings.night_bool)
				{
					sv_skyname->SetValue("sky_csgo_night02");
					pMaterial->SetMaterialVarFlag(SDK::MATERIAL_VAR_TRANSLUCENT, false);
					pMaterial->ColorModulate(0.05, 0.05, 0.05);
				}
				else
				{
					sv_skyname->SetValue("vertigoblue_hdr");
					pMaterial->ColorModulate(1.00, 1.00, 1.00);
				}
			}
			else if (strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
			{
				if (SETTINGS::settings.night_bool)
					pMaterial->ColorModulate(0.28, 0.28, 0.28);
				else
					pMaterial->ColorModulate(1, 1, 1);
			}
		}
		nightmode_performed = true;
	}
}

void CVisuals::DrawFovArrows(SDK::CBaseEntity* entity)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	if (entity->GetIsDormant())
		return;

	Vector screenPos;
	if (UTILS::IsOnScreen(aimbot->get_hitbox_pos(entity, SDK::HitboxList::HITBOX_BODY), screenPos)) return;

	auto client_viewangles = Vector();
	auto screen_width = 0, screen_height = 0;

	INTERFACES::Engine->GetViewAngles(client_viewangles);
	INTERFACES::Engine->GetScreenSize(screen_width, screen_height);

	auto radius = 450.f;
	Vector local_position = local_player->GetVecOrigin() + local_player->GetViewOffset();

	const auto screen_center = Vector(screen_width / 2.f, screen_height / 2.f, 0);
	const auto rot = DEG2RAD(client_viewangles.y - UTILS::CalcAngle(local_position, aimbot->get_hitbox_pos(entity, SDK::HitboxList::HITBOX_BODY)).y - 90);

	std::vector<SDK::Vertex_t> vertices;

	vertices.push_back(SDK::Vertex_t(Vector2D(screen_center.x + cosf(rot) * radius, screen_center.y + sinf(rot) * radius)));
	vertices.push_back(SDK::Vertex_t(Vector2D(screen_center.x + cosf(rot + DEG2RAD(2)) * (radius - 16), screen_center.y + sinf(rot + DEG2RAD(2)) * (radius - 16))));
	vertices.push_back(SDK::Vertex_t(Vector2D(screen_center.x + cosf(rot - DEG2RAD(2)) * (radius - 16), screen_center.y + sinf(rot - DEG2RAD(2)) * (radius - 16))));

	RENDER::TexturedPolygon(3, vertices, SETTINGS::settings.fov_col); //255, 40, 230
}

void CVisuals::DrawAntiAimSides()
{
	int screen_x;
	int screen_y;
	INTERFACES::Engine->GetScreenSize(screen_x, screen_y);
	screen_x /= 2; screen_y /= 2;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player)
		return;

	if (local_player->GetHealth() <= 0)
		return;

	if (!SETTINGS::settings.aa_type == 2 || !SETTINGS::settings.aa_type == 4)
		return;

	if (SETTINGS::settings.aa_side == 0)
		RENDER::DrawWF(screen_x - 25, screen_y, FONTS::visuals_side_font, CColor(0, 255, 0, 255), L"\u25c4");
	if (SETTINGS::settings.aa_side == 1)
		RENDER::DrawWF(screen_x + 25, screen_y, FONTS::visuals_side_font, CColor(0, 255, 0, 255), L"\u25ba");
}

void CVisuals::DoEnemyCircle(const Vector& origin)
{
	float * flRotation;
	float flRadius = 360.0f;
	int iScreenWidth, iScreenHeight;
	INTERFACES::Engine->GetScreenSize(iScreenWidth, iScreenHeight);

	Vector vRealAngles;
	INTERFACES::Engine->GetViewAngles(vRealAngles);

	Vector vForward, vRight, vUp(0.0f, 0.0f, 1.0f);

	MATH::AngleVectors(vRealAngles, &vForward);

	vForward.z = 0.0f;
	MATH::NormalizeAngle(vForward);
	CrossProduct(vUp, vForward, vRight);
	float flFront = MATH::DotProduct(origin, vForward);
	float flSide = MATH::DotProduct(origin, vRight);
	float flXPosition = flRadius * -flSide;
	float flYPosition = flRadius * -flFront;

	float rotation = INTERFACES::Globals->user_cmd->viewangles.y + 180;

	rotation = atan2(flXPosition, flYPosition) + M_PI;
	rotation *= 180.0f / M_PI;

	float flYawRadians = -(rotation) * M_PI / 180.0f;
	float flCosYaw = cos(flYawRadians);
	float flSinYaw = sin(flYawRadians);

	flXPosition = (int)((iScreenWidth / 2.0f) + (flRadius * flSinYaw));
	flYPosition = (int)((iScreenHeight / 2.0f) - (flRadius * flCosYaw));

	RENDER::DrawFilledRect(flXPosition, flYPosition, 10, 10, RED);
}

void CVisuals::DrawBorderLines()
{
	if (SETTINGS::settings.removescope)
	{
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

		if (!local_player)
			return;

		auto weapon = INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex());

		if (!weapon)
			return;

		//--- Screen Positioning ---//
		int screen_x;
		int screen_y;
		int center_x;
		int center_y;
		INTERFACES::Engine->GetScreenSize(screen_x, screen_y);
		INTERFACES::Engine->GetScreenSize(center_x, center_y);
		center_x /= 2; center_y /= 2;

		//--- Rendering Scope Lines ---//
		if (local_player->GetIsScoped())
		{
			RENDER::DrawLine(0, center_y, screen_x, center_y, CColor(0, 0, 0, 255));
			RENDER::DrawLine(center_x, 0, center_x, screen_y, CColor(0, 0, 0, 255));
		}
	}
}

void CVisuals::drawbaiminfo()
{
	if (SETTINGS::settings.drawbaiminfo)
	{
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

		if (!local_player)
			return;

		auto weapon = INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex());

		if (!weapon)
			return;



		int screen_width, screen_height;
		INTERFACES::Engine->GetScreenSize(screen_width, screen_height);

		if (!SETTINGS::settings.aa_bool)
			return;


		//--- Screen Positioning ---//

		//--- Rendering Scope Lines ---//
		//if (SETTINGS::settings.bodyaim && GetAsyncKeyState(0x4E))
		//{
		//	RENDER::DrawF((screen_width / 2) + 60, screen_height / 2, FONTS::visuals_side_font, true, true, CColor(10, 145, 190, 255), "| BODYAIMING |"); //green
		//}
	}
}

void CVisuals::DrawScopeInfo()
{
	if (SETTINGS::settings.drawscopeinfo)
	{
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

		if (!local_player)
			return;

		auto weapon = INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex());

		if (!weapon)
			return;



		int screen_width, screen_height;
		INTERFACES::Engine->GetScreenSize(screen_width, screen_height);

		if (!SETTINGS::settings.aa_bool)
			return;

	
		//--- Screen Positioning ---//

		//--- Rendering Scope Lines ---//
		if (local_player->GetIsScoped())
		{
			RENDER::DrawF((screen_width / 2) + 40, screen_height / 2, FONTS::visuals_side_font, true, true, CColor(10, 145, 190, 255), "| SCOPED |"); //green
		}
	}
}
	
CVisuals* visuals = new CVisuals();
