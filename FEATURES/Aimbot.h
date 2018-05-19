#pragma once

namespace SDK
{
	class CUserCmd;
	class CBaseEntity;
	class CBaseWeapon;
}

struct AimbotData_t
{
	AimbotData_t(SDK::CBaseEntity* player, const int& idx)
	{
		this->pPlayer = player;
		this->index = idx;
	}
	SDK::CBaseEntity*	pPlayer;
	int				index;
};

class CAimbot
{
public:
	void AutoZeus(SDK::CUserCmd * cmd);
	void select_target();
	void shoot_enemy(SDK::CUserCmd* cmd);
	bool good_backtrack_tick(int tick);
	void AstroWalk(SDK::CUserCmd * cmd);
	void lby_backtrack(SDK::CUserCmd * pCmd, SDK::CBaseEntity * pLocal, SDK::CBaseEntity * pEntity);
	int HitboxScan(SDK::CBaseEntity * entity);
	//static std::vector<Vector> GetMultiplePointsForHitbox(SDK::CBaseEntity * Local, SDK::CBaseEntity * pBaseEntity, int iHitbox, matrix3x4_t BoneMatrix[]);
	//bool ValidTick(LagRecord * targetRecord, LagRecord * prevRecord);
	//void GetBestTarget(SDK::CBaseEntity *& targetPlayer, Vector & targetVector, LagRecord *& targetRecord);
	float accepted_inaccuracy(SDK::CBaseWeapon * weapon);
	void auto_revolver(SDK::CUserCmd * cmd);
	//bool can_shoot(SDK::CUserCmd * cmd);
	bool can_shoot();
	void fix_recoil(SDK::CUserCmd* cmd);
	void rotate_movement(float yaw, SDK::CUserCmd * cmd);
	Vector get_hitbox_pos(SDK::CBaseEntity* entity, int hitbox_id);
	Vector head_multipoint(SDK::CBaseEntity * entity);
	Vector body_multipoint(SDK::CBaseEntity * entity);
	Vector full_multipoint(SDK::CBaseEntity * entity);
	Vector head_pointtt(SDK::CBaseEntity * entity);
	Vector body_point(SDK::CBaseEntity * entity);
	Vector full_point(SDK::CBaseEntity * entity);
	std::vector<AimbotData_t>	Entities;
private:
	std::vector<Vector> GetMultiplePointsForHitbox(SDK::CBaseEntity * local, SDK::CBaseEntity * entity, int iHitbox, VMatrix BoneMatrix[128]);
	void quick_stop(SDK::CUserCmd * cmd);
	void quick_stopv2(SDK::CBaseEntity * entity, SDK::CUserCmd * cmd);
	int scan_hitbox(SDK::CBaseEntity * entity); //int
	int SmartBaim(SDK::CBaseEntity * entity);

	SDK::mstudiobbox_t* get_hitbox(SDK::CBaseEntity* entity, int hitbox_index);
	bool meets_requirements(SDK::CBaseEntity* entity);
	int get_damage(Vector position);
	bool spread_limit(SDK::CBaseWeapon* weapon);
	float seedchance(Vector Point);
};

extern CAimbot* aimbot;


