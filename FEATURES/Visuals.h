#pragma once

namespace SDK
{
	class CUserCmd;
	class CBaseEntity;
}

class CVisuals
{
public:
	void Draw();
	void ClientDraw();
	void DrawBulletBeams();
private:
	void DrawBox(SDK::CBaseEntity* entity, CColor color);
	void DrawCorners(SDK::CBaseEntity * entity, CColor color);
	void DrawName(SDK::CBaseEntity* entity, CColor color, int index);
	void DrawWeapon(SDK::CBaseEntity * entity, CColor color, int index);
	void DrawHealth(SDK::CBaseEntity * entity, CColor color, CColor dormant);
	void DrawPenetration(SDK::CBaseEntity * entity);
	void DrawAmmo(SDK::CBaseEntity * entity, CColor color, CColor dormant);
	void BombPlanted(SDK::CBaseEntity * entity);
	float resolve_distance(Vector src, Vector dest);
	void DrawDistance(SDK::CBaseEntity * entity, CColor color);
	void DrawInfo(SDK::CBaseEntity * entity, CColor color, CColor alt);
	void DrawSkeleton(SDK::CBaseEntity * entity, CColor color, VMatrix bone_matrix[128]);
	void DrawBombPlanted(SDK::CBaseEntity * pEntity, CColor color);
	void BombTimer(SDK::CBaseEntity * pEntity, CColor color);
	void DrawBomb(SDK::CBaseEntity * pEntity, CColor color);
	void DrawCrosshair();
	void SpecList();
	void DrawIndicator();
	void ModulateWorld();
	void DrawFovArrows(SDK::CBaseEntity * entity);
	void DrawAntiAimSides();
	void DoEnemyCircle(const Vector & origin);
	void DrawBorderLines();
	void drawbaiminfo();
	void DrawScopeInfo();
public:
	std::vector<std::pair<int, float>>				Entities;
	std::deque<UTILS::BulletImpact_t>				Impacts;
};

extern CVisuals* visuals;

