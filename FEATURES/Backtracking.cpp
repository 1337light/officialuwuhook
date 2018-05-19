#include "../../includes.h"
#include "../../UTILS/interfaces.h"
#include "../../sdk/IEngine.h"
#include "../../sdk/CUserCmd.h"
#include "../../sdk/CBaseEntity.h"
#include "../../sdk/CClientEntityList.h"
#include "../../sdk/NetChannel.h"
#include "../../sdk/CTrace.h"
#include "../../sdk/CBaseWeapon.h"
#include "../../sdk/CGlobalVars.h"
#include "../../sdk/ConVar.h"
#include "../features/AutoWall.h"
#include "../features/Aimbot.h"
#include "Backtracking.h"

#define TICK_INTERVAL			( g_pGlobalVarsBase->interval_per_tick )

#define ROUND_TO_TICKS( t )		( TICK_INTERVAL * TIME_TO_TICKS( t ) )

template<class T> const T&
clamp(const T& x, const T& upper, const T& lower) { return min(upper, max(x, lower)); }
static std::deque<CBacktrack::SequenceData>sequences;
void CBacktrack::AddLatencyToNetchan(SDK::NetChannel *netchan, float Latency) {
	for (auto& seq : sequences) {
		if (INTERFACES::Globals->realtime - seq.Realtime >= Latency) {
			netchan->m_nInReliableState = seq.inreliablestate;
			netchan->m_nInSequenceNr = seq.sequencenr;
			break;
		}
	}
}
int lastincomingsequencenumber = 0;
void CBacktrack::UpdateIncomingSequences() {
	DWORD ClientState = (DWORD)*(DWORD*)INTERFACES::client_state;
	if (ClientState) {
		SDK::NetChannel *netchan = *reinterpret_cast<SDK::NetChannel**>(reinterpret_cast<std::uintptr_t>(INTERFACES::client_state) + 0x9C);
		if (netchan) {
			if (netchan->m_nInSequenceNr > lastincomingsequencenumber) {
				lastincomingsequencenumber = netchan->m_nInSequenceNr;
				sequences.push_front(CBacktrack::SequenceData(netchan->m_nInReliableState, netchan->m_nOutReliableState, netchan->m_nInSequenceNr, INTERFACES::Globals->realtime));
			}
			if (sequences.size() > 2048)
				sequences.pop_back();
		}
	}
}

void CBacktrack::backtrack_player(SDK::CUserCmd* cmd)
{
	for (int i = 1; i < 65; ++i)
	{

		auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

		if (!entity)
			continue;

		if (!local_player)
			continue;

		bool is_local_player = entity == local_player;
		bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;

		if (is_local_player)
			continue;

		if (is_teammate)
			continue;

		if (entity->GetHealth() <= 0)
			continue;

		if (local_player->GetHealth() <= 0)
			continue;

		if (entity->GetImmunity())
			continue;

		int index = entity->GetIndex();

		for (int i = 0; i < 20; i++)
		{
			backtrack_hitbox[index][i][cmd->command_number % 12] = aimbot->get_hitbox_pos(entity, i);
		}

		static auto sv_unlag = INTERFACES::cvar->FindVar("sv_unlag");
		if (!sv_unlag->GetBool())
			return;

		backtrack_simtime[index][cmd->command_number % 12] = entity->GetSimTime();

		for (int i = 0; i < 12; i++)
		{
			if (backtrack_simtime[index][i] != compensate[index][i])
			{
				if (i > 0 && i != 11)
				{
					oldest_tick[index] = i + 2;
				}
				else
				{
					oldest_tick[index] = 1;
				}
				compensate[index][i] = backtrack_simtime[index][i];
			}
		}
	}
}

inline Vector CBacktrack::angle_vector(Vector meme)
{
	auto sy = sin(meme.y / 180.f * static_cast<float>(M_PI));
	auto cy = cos(meme.y / 180.f * static_cast<float>(M_PI));

	auto sp = sin(meme.x / 180.f * static_cast<float>(M_PI));
	auto cp = cos(meme.x / 180.f* static_cast<float>(M_PI));

	return Vector(cp*cy, cp*sy, -sp);
}

inline float CBacktrack::point_to_line(Vector Point, Vector LineOrigin, Vector Dir)
{
	auto PointDir = Point - LineOrigin;

	auto TempOffset = PointDir.Dot(Dir) / (Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z);
	if (TempOffset < 0.000001f)
		return FLT_MAX;

	auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

	return (Point - PerpendicularPoint).Length();
}

bool CBacktrack::IsValid(SDK::CBaseEntity * player)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return false;

	if (!player)
		return false;

	if (player->GetIsDormant() || player->GetHealth() == 0 || player->GetFlags() & FL_FROZEN)
		return false;

	if (player->GetTeam() == local_player->GetTeam())
		return false;

	if (player->GetClientClass()->m_ClassID != 35)
		return false;

	if (player == local_player)
		return false;

	if (player->GetImmunity())
		return false;

	return true;
}

CBacktrack* backtracking = new CBacktrack();
legit_backtrackdata headPositions[64][12];