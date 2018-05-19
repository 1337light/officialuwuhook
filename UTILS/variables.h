#pragma once
#include "../MENU/menu_framework.h"
#include "../SDK/CInputSystem.h"


/*
global variables and other stuff that is probably too small to put in another 
file (and i'm too lazy to make files for all of them) would go in here.
Stuff like fonts and shit
*/

namespace GLOBAL
{
	extern bool Target;
	extern HWND csgo_hwnd;

	extern bool should_send_packet;
	extern bool is_fakewalking;
	extern bool Overriding;
	extern bool input_shouldListen;
	extern ButtonCode_t* input_receivedKeyval;
	extern int choke_amount;


	extern Vector real_angles;
	extern Vector lby_angle;
	extern Vector fake_angles;
	using msg_t = void(__cdecl*)(const char*, ...);
	extern msg_t		Msg;
}
namespace FONTS
{
 
	extern unsigned int menu_welcome_font;
	extern unsigned int menu_tab_font;
	extern unsigned int menu_checkbox_font;
	extern unsigned int menu_slider_font;
	extern unsigned int menu_groupbox_font;
	extern unsigned int menu_combobox_font;
	extern unsigned int menu_window_font;
	extern unsigned int numpad_menu_font;
	extern unsigned int visuals_font_event_log;
	extern unsigned int visuals_esp_font;
	extern unsigned int visuals_xhair_font;
	extern unsigned int visuals_side_font;
	extern unsigned int visuals_name_font;
	extern unsigned int visuals_lby_font;
	extern unsigned int visuals_weapon_icon_font;
	extern unsigned int visuals_watermark;
	extern unsigned int visuals_lby_lc_arrow_font;
	extern unsigned int supremacy;
								
	
	bool ShouldReloadFonts();
	void InitFonts();
}

namespace SETTINGS
{


	class CSettings
	{
	public:
		// returns true/false whether the function succeeds, usually returns false if file doesn't exist
		bool Save(std::string file_name);
		bool Load(std::string file_name);

		void CreateConfig(); // creates a blank config

		std::vector<std::string> GetConfigs();

		bool moonwalk;
		bool bhop_bool;
		bool strafe_bool;
		bool esp_bool;
		int chams_type;
		int ammo_mode;
		int xhair_type;
		bool tp_bool;
		bool enemydirection;
		bool aim_bool;
		int aim_type;
		float auto_chance_val;
		int override_key;
		bool recoil_circle;
		CColor spread_col;
		float auto_damage_val;
		float scout_damage_val;
		float scout_chance_val; 
		float other_damage_val;
		float zeus_damage_val;
		float other_chance_val;
		float fov = 90;
		bool baim_mode;
		bool automaticrevolver;
		bool automaticfire;
		bool servercrash;
		bool aa_bool;
		bool box_bool_local;
		bool fakewalk;
		bool astrowalk;
		int antiaimtype;
		int movingpitch_type;
		float CustomMovingPitch;
		int realmove_type;
		int fakemove_type;
		float AddMovingReal;
		float AddMovingFake;
		float MovingSpinSpeed = 50.0f;
		float MovingJitterRange = 15;
		bool rankreveal;
		bool damageindicator;
		bool killsound;
		bool eventlogs;
		bool dormantantiaim;
		bool baiminair;
		bool Freestanding;
		bool localplayereszkere;
		bool antiaimside;
		bool lbyindicator;
		bool fakeanglerotation;
		bool lowfpswarning;
		bool lcindicator;
		bool watermark;
		bool namespam;
		bool ragdollforce;
		bool fullbright;
		bool drawgray;
		bool lowresolution;
		bool show_impact;
		bool reversedepth;
		bool fakeanglechams;
		bool autozeus;
		bool nosky;
		bool removepostprocess;
		bool chatspam;
		bool headshotonly;
		bool damagelogs;
		int aa_type;
		int fakeaa;
		bool dormantindicator;
		int fakeyaw_type;
		int pitch_type;
		int buybot;
		bool hitmarker;
		bool purchaselogs;
		bool aa_autodir;
		float edge_distance = 25.f;
		int hitmarker_sound;
		int acc_type;
		bool autoscope_bool;
		bool current_cycle;
		bool current_cycle_local;
		int weapon;
		bool up_bool;
		bool misc_bool;
		bool clantag;
		int config_sel;
		bool showarmor;
		float misc_ServerCrasher_AmtPerTick;
		bool misc_ServerCrasher_On;
		bool beam_bool;
		bool stop_bool;
		CColor glowc;
		bool night_bool;
		float offscreenballsize;
		bool asuswalls;
		bool box_bool;
		bool name_bool;
		int wep_type;
		int droppedwep_type;
		bool weap_bool;
		bool health_bool;
		bool resolver;
		bool pitchresolver;
		bool drawscopeinfo;
		bool drawbaiminfo;
		bool resolveroverride;
		bool overridekey;
		bool DaAmmo;
		bool bodyaim;
		bool info_bool;
		bool skeleton;
		bool removescope;
		bool dmg_bool;
		bool draw_bt;
		bool back_bool;
		bool lag_bool;
		bool lag_compensation;
		bool box_type;
		int weapon_mode;
		bool selfesp;
		bool multi_bool;
		bool fakefix_bool;
		bool radarhack;
		bool offscreen_esp;
		bool drawpenetration;
		bool drawc4timer;
		bool drawdroppedweapons;
		bool distance;
		bool drawdirection;
		bool showammo;
		bool spectatorlist;
		bool showhp;
		bool othercirclestrafe;
		bool Othercirclekey;
		bool showweapononbox;
		bool showlbyupdate;
		bool drawammo;
		bool showzoom;
		bool showmoney;
		bool showfakeangles;
		bool showplayervelocity;
		bool showlbytimer;
		bool showresolverinfo;
		bool showlbydelta;
		bool realantiaimline;
		bool fakeantiaimline;
		bool ragebacktracking;
		int delay_shot;
		bool removerecoil;
		bool glow;
		bool BacktrackIndicator;
		bool AntiOBS;
		bool removevisualrecoil;
		bool wireframesmoke;
		bool removeflasheffect;
		bool metallic;

		float stand_lag;
		float move_lag;
		float jump_lag;

		bool debug_bool;

		CColor fov_col;
		CColor localbeam_col;
		CColor enemybeam_col;
		bool lbyupdate;
		bool fakelatency;
		float fakelatency_amt;
		CColor friendlybeam_col;
		CColor fake_angle_col = CColor(255, 255, 255);
		CColor vmodel_col;
		CColor imodel_col;
		float AmmoBar[3] = { 0.61f,1.35f,2.55f };
		CColor drawdirection_col;
		CColor box_col;
		CColor skeleton_col;
		CColor hk_col;
		CColor menu_col = CColor(117, 160, 13);
		//CColor menu_text = CColor(255, 75, 0);
		

		struct
		{
			bool esp;
			bool chams;
		}localplayer;

		float chance_val;
		float damage_val;
		float delta_val;
		float point_val;
		float body_val;
		bool localesp = false;
		bool teamesp = false;

		bool flip_bool;
		int aa_side;

	private:
	}; extern CSettings settings;
}

/*extern bool bhop_bool;
extern bool strafe_bool;
extern bool esp_bool;
extern int chams_type;
extern int xhair_type;
extern bool tp_bool;
extern bool aim_bool;
extern int aim_type;
extern bool aa_bool;
extern int aa_type;
extern int acc_type;
extern bool up_bool;
extern bool misc_bool;

extern float chance_val;
extern float damage_val;
extern float delta_val;

extern bool flip_bool;
extern int aa_side;
extern bool in_tp;*/

template< typename Function > Function call_vfunc(PVOID Base, DWORD Index)
{
	PDWORD* VTablePointer = (PDWORD*)Base;
	PDWORD VTableFunctionBase = *VTablePointer;
	DWORD dwAddress = VTableFunctionBase[Index];
	return (Function)(dwAddress);
}

extern bool using_fake_angles[65];

extern bool in_tp;
extern bool fake_walk;

extern int resolve_type[65];

extern int shots_fired[65];
extern int shots_hit[65];
extern int shots_missed[65];
extern int target;
extern float tick_to_back[65];
extern float lby_to_back[65];
extern bool backtrack_tick[65];

extern float lby_delta;
extern float update_time[65];

extern int hitmarker_time;

extern bool menu_hide;

extern int oldest_tick[65];
extern float compensate[65][12];
extern Vector backtrack_hitbox[65][20][12];
extern float backtrack_simtime[65][12];