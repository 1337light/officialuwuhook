#pragma once

namespace SDK
{
	class CUserCmd;
}

class CMovement
{
public:
	void bunnyhop(SDK::CUserCmd* cmd);
	void autostrafer(SDK::CUserCmd* cmd);
	void RotateMovement(SDK::CUserCmd * cmd, float rotation);
	void quick_stop(SDK::CBaseEntity * entity, SDK::CUserCmd * cmd);
	bool CircularStrafe(SDK::CUserCmd * cmd, Vector & OriginalView);
	int CircleFactor = 0;
};

extern CMovement* movement;