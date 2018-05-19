#pragma once
namespace SDK
{
	class CUserCmd;
	class CBaseEntity;
}

class CAntiAim
{
public:
	void freestand(SDK::CUserCmd * cmd);
	void do_antiaim(SDK::CUserCmd* cmd);
	//void do_antiaim(SDK::CUserCmd * cmd, SDK::CBaseEntity * entity);
	void fix_movement(SDK::CUserCmd* cmd);
private:
	void staticyaw(SDK::CUserCmd* cmd);
	void legit(SDK::CUserCmd* cmd);
	void sideways(SDK::CUserCmd* cmd);
	void testantiaim(SDK::CUserCmd * cmd);
	void lowerbody(SDK::CUserCmd* cmd);
	void checkforfakeaa(SDK::CUserCmd * cmd);
	void backjitter(SDK::CUserCmd* cmd);
	void nospreadaimware(SDK::CUserCmd * cmd);
	void freestanding(SDK::CUserCmd * cmd, float yaw);
};

extern CAntiAim* antiaim;