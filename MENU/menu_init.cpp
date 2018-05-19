#include "../includes.h"
#include "../UTILS/interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CInputSystem.h"
#include "../UTILS/render.h"
#include "Components.h"

#include "../MENU/menu_framework.h"
#define menu_color CColor(255, 140, 35);
#define font_color CColor(55, 140, 35)


namespace MENU
{
	void InitColors()
	{
		using namespace PPGUI_PP_GUI;
		colors[WINDOW_BODY] = CColor(40, 40, 40);
		colors[WINDOW_TITLE_BAR] = CColor(60, 60, 60); //255, 75, 0
		colors[WINDOW_TEXT] = WHITE;

		colors[GROUPBOX_BODY] = CColor(40, 40, 40, 0);
		colors[GROUPBOX_OUTLINE] = CColor(60, 60, 60, 255);
		colors[GROUPBOX_TEXT] = WHITE;

		colors[SCROLLBAR_BODY] = SETTINGS::settings.menu_col;

		colors[SEPARATOR_TEXT] = WHITE;
		colors[SEPARATOR_LINE] = CColor(60, 60, 60, 255);

		colors[CHECKBOX_CLICKED] = CColor(159, 209, 20, 255);
		colors[CHECKBOX_NOT_CLICKED] = CColor(55, 55, 55, 255);
		colors[CHECKBOX_TEXT] = WHITE;

		colors[BUTTON_BODY] = CColor(40, 40, 40, 255);
		colors[BUTTON_TEXT] = WHITE;

		colors[COMBOBOX_TEXT] = LIGHTGREY;
		colors[COMBOBOX_SELECTED] = CColor(159, 209, 20, 255);
		colors[COMBOBOX_SELECTED_TEXT] = CColor(45, 45, 45, 255);
		colors[COMBOBOX_ITEM] = CColor(20, 20, 20, 255);
		colors[COMBOBOX_ITEM_TEXT] = LIGHTGREY;

		colors[SLIDER_BODY] = CColor(40, 40, 40, 255);
		colors[SLIDER_VALUE] = menu_color
			colors[SLIDER_TEXT] = WHITE;

		colors[TAB_BODY] = CColor(20, 20, 20, 120);
		colors[TAB_TEXT] = LIGHTGREY;
		colors[TAB_BODY_SELECTED] = CColor(40, 40, 40, 150); //HOTPINK
		colors[TAB_TEXT_SELECTED] = WHITE;

		colors[VERTICAL_TAB_BODY] = CColor(255, 40, 40, 255);
		colors[VERTICAL_TAB_TEXT] = WHITE;
		colors[VERTICAL_TAB_OUTLINE] = CColor(0, 0, 0, 100);
		colors[VERTICAL_TAB_BODY_SELECTED] = CColor(255, 20, 20, 255);

		colors[COLOR_PICKER_BODY] = CColor(50, 50, 50, 0);
		colors[COLOR_PICKER_TEXT] = WHITE;
		colors[COLOR_PICKER_OUTLINE] = CColor(0, 0, 0, 0);
	}
	void Do() {
		static bool menu_open = false;

		if (UTILS::INPUT::input_handler.GetKeyState(VK_INSERT) & 1)
		{
			menu_open = !menu_open;
			INTERFACES::Engine->ClientCmd(menu_open ? "cl_mouseenable 0" :
				"cl_mouseenable 1");
			INTERFACES::InputSystem->EnableInput(!menu_open);
		}

		if (UTILS::INPUT::input_handler.GetKeyState(VK_END) & 1)
		{
			menu_hide = !menu_hide;
		}

		//--- Flip Bool Stuff ---//
		if (UTILS::INPUT::input_handler.GetKeyState(86) & 1)
			SETTINGS::settings.flip_bool = !SETTINGS::settings.flip_bool;


		//--- New Menu Rendering System --//
		if (menu_hide)
		{
			//menu is hidden entirely, you can do anything here i guess
		}
		else
		{
				if (menu_open)
				{
					using namespace PPGUI_PP_GUI;

					// if menu not open
					if (!menu_open)
						return;

					DrawMouse();

				SetFont(FONTS::menu_welcome_font);
				SetFont(FONTS::menu_welcome_font);
				WindowBegin("UWUhook for Counter-Strike: Global Offensive (BETA VERSION)", Vector2D(200, 200), Vector2D(600, 500));
				{
					//std::vector<std::string> tabs = { "Rage", "Visuals", "Misc" };
					std::vector<std::string> tabs = { 
						"rage",
						"visuals",
						"misc" };

					std::vector<std::string> aim_mode = { "Rage" };
					std::vector<std::string> acc_mode = { "Head", "Body", "All" };
					std::vector<std::string> chams_mode = { "Off", "visible", "visible & xqz" };
					std::vector<std::string> ammo_mode = { "Off", "Bar" };
					std::vector<std::string> xhair_type = { "Off", "Static", "Recoil", "Spread", "Both" };
					std::vector<std::string> aa_mode = { "Off", "Static", "Lowerbody", "Fake lowerbody", "Jitter fake lowerbody", "Spinbot", "180z", "Fake lowerbody z", "Automatic" };
					std::vector<std::string> fakeaa = { "Off", "Static", "Lowerbody", "Fake lowerbody", "Jitter fake lowerbody", "Spinbot", "180z", "Fake lowerbody z" };
					std::vector<std::string> pitch_type = { "Off", "Down", "Fake down", "Fake up", "Fake zero" };
					std::vector<std::string> configs = { "1", "2", "3", "4", "5", "6" };
					std::vector<std::string> wep_types = { "Off", "Text", "Icon" };
					std::vector<std::string> droppedwep_types = { "Off", "Text", "Icon" };
					std::vector<std::string> hitmarkers = { "Off", "Gamesense", "Roblox", "Bameware", "Bubble" };
					std::vector<std::string> hcweapon = { "SCAR-20/G3SG1", "SSG-08/AWP", "OTHER", "ZEUS X27" };
					std::vector<std::string> buybot_mode = { "Off", "Auto", "Scout" };
					std::vector<std::string> anti_aim_type = { "Standing", "Moving", "In air" };
					std::vector<std::string> movingpitch_mode = { "Off", "Emotion", "Up", "Zero", "Custom" };
					std::vector<std::string> movingaa_realx = { "Off", "Static", "Rotate", "Jitter", "Lowerbody", "180z" };
					std::vector<std::string> movingaa_fakex = { "Off", "Static", "Rotate", "Jitter", "180z" };
					std::vector<std::string> delay_shot = { "Off", "Accurate Unlag" };
					std::string config;
					SetFont(FONTS::menu_window_font);
					{

						switch (Tab("main", tabs, OBJECT_FLAGS::FLAG_NONE))
						{
						case 0:
						{
							Checkbox("enable aimbot", SETTINGS::settings.aim_bool);
							if (SETTINGS::settings.aim_bool)
							{
								Combobox("aimbot mode", aim_mode, SETTINGS::settings.aim_type);
								if (SETTINGS::settings.aim_type == 0)
								{
									GroupboxBegin("main", 150);
									{
										if (SETTINGS::settings.aim_type == 0)
										{
											Combobox("Target Hitbox", acc_mode, SETTINGS::settings.acc_type);
											Checkbox("Automatic fire", SETTINGS::settings.automaticfire);
											Checkbox("Automatic revolver", SETTINGS::settings.automaticrevolver);
											Checkbox("Automatic scope", SETTINGS::settings.autoscope_bool);
											//Checkbox("remove recoil", SETTINGS::settings.removerecoil);
											Checkbox("Anti-aim resolver", SETTINGS::settings.resolver);
											Checkbox("Anti-aim resolver override [Z]", SETTINGS::settings.resolveroverride);
											Checkbox("Anti-aim pitch resolver", SETTINGS::settings.pitchresolver);
											Combobox("Weapon", hcweapon, SETTINGS::settings.weapon);
											if (SETTINGS::settings.weapon == 0)
											{
												Slider("Hitchance", 0, 100, SETTINGS::settings.auto_chance_val);
												Slider("Minimum damage", 1, 100, SETTINGS::settings.auto_damage_val);
											}
											else if (SETTINGS::settings.weapon == 1)
											{
												Slider("Hitchance", 0, 100, SETTINGS::settings.scout_chance_val);
												Slider("Minimum damage", 1, 100, SETTINGS::settings.scout_damage_val);
											}
											else if (SETTINGS::settings.weapon == 2)
											{
												Slider("Hitchance", 0, 100, SETTINGS::settings.other_chance_val);
												Slider("Minimum damage", 1, 100, SETTINGS::settings.other_damage_val);
											}
											else if (SETTINGS::settings.weapon == 3)
											{
												Slider("Minimum damage", 1, 100, SETTINGS::settings.zeus_damage_val);
											}
											//Checkbox("position adjustment", SETTINGS::settings.ragebacktracking);
											//Checkbox("fake lag fix", SETTINGS::settings.fakefix_bool);
											Checkbox("Automatic Stop", SETTINGS::settings.stop_bool);
										}
									}
									GroupboxEnd();
									GroupboxBegin("multi-point", 125);
									{
										Checkbox("Multi-point", SETTINGS::settings.multi_bool);
										if (SETTINGS::settings.multi_bool)
										{
											if (SETTINGS::settings.multi_bool)
											{
												Slider("Head hitbox scale", 0, 1, SETTINGS::settings.point_val);
												Slider("Stomach hitbox scale", 0, 1, SETTINGS::settings.body_val);
											}
										}
									}
									GroupboxEnd();
									GroupboxBegin("anti-aim", 125);
									{
										Checkbox("enable antiaims", SETTINGS::settings.aa_bool);
										if (SETTINGS::settings.aa_bool)
										{
											if (SETTINGS::settings.aa_bool)
												Combobox("Pitch", pitch_type, SETTINGS::settings.pitch_type);
											Combobox("Yaw", aa_mode, SETTINGS::settings.aa_type);
											Combobox("Fake Yaw", fakeaa, SETTINGS::settings.fakeaa);
											Combobox("Moving Real", movingaa_realx, SETTINGS::settings.realmove_type);
											Slider("Add Moving Real", -180, 180, SETTINGS::settings.AddMovingReal);
											Combobox("Moving Fake", movingaa_fakex, SETTINGS::settings.fakemove_type);
											Slider("Add Moving Fake", -180, 180, SETTINGS::settings.AddMovingFake);
											Slider("delta", 0, 180, SETTINGS::settings.delta_val);
											Slider("Moving Jitter", -180.f, 180.f, SETTINGS::settings.MovingJitterRange);
											//Checkbox("disable when dormant", SETTINGS::settings.dormantantiaim);
											Checkbox("Fake walk [SHIFT]", SETTINGS::settings.fakewalk);
										}
									}
									GroupboxEnd();
									GroupboxBegin("other", 105);
									{
										//Checkbox("baim when x", SETTINGS::settings.baim_mode);
										if (SETTINGS::settings.aim_bool)
										{
											if (SETTINGS::settings.aim_bool)
											{
												Checkbox("Position adjustment", SETTINGS::settings.ragebacktracking);
												Checkbox("Fake-lag fix", SETTINGS::settings.fakefix_bool);
												Checkbox("Remove recoil", SETTINGS::settings.removerecoil);
												//Checkbox("Body aim [N]", SETTINGS::settings.bodyaim);
												Combobox("Delay shot", delay_shot, SETTINGS::settings.delay_shot);
											}
										}
									}
									GroupboxEnd();
									GroupboxBegin("fake-lag", 105);
									{
										Checkbox("fakelag", SETTINGS::settings.lag_bool);
										if (SETTINGS::settings.lag_bool)
										{
											Slider("Moving fake-lag", 1, 15, SETTINGS::settings.move_lag);
											Slider("Jumping fake-lag", 1, 15, SETTINGS::settings.jump_lag);
											//Checkbox("Break lag Compensation", SETTINGS::settings.lag_compensation);
										}
									}
								}
								GroupboxEnd();
							}
						}
						break;
						case 1:
						{
							Checkbox("enable visuals", SETTINGS::settings.esp_bool);
							if (SETTINGS::settings.esp_bool)
							{

								GroupboxBegin("misc", 150);
								{
									if (SETTINGS::settings.esp_bool)
									{
										Checkbox("Bounding Box", SETTINGS::settings.box_type);
										if (SETTINGS::settings.box_type)
											ColorPicker("Box", SETTINGS::settings.box_col);

										Checkbox("Draw name", SETTINGS::settings.name_bool);
										Checkbox("Draw weapon", SETTINGS::settings.weap_bool);
										if (SETTINGS::settings.weap_bool)
										{
											Combobox("weapon type", wep_types, SETTINGS::settings.wep_type);
										}
										Checkbox("Draw ammo", SETTINGS::settings.drawammo);
										Checkbox("Draw health", SETTINGS::settings.health_bool);
										Checkbox("Draw info", SETTINGS::settings.info_bool);
										Checkbox("Draw distance", SETTINGS::settings.distance);
										Checkbox("Draw scoped", SETTINGS::settings.drawscopeinfo);
									    Checkbox("Draw penetration dot", SETTINGS::settings.drawpenetration);
										//Checkbox("Draw backtrack", SETTINGS::settings.draw_bt);
										Checkbox("Draw fov arrows", SETTINGS::settings.offscreen_esp);
										if (SETTINGS::settings.offscreen_esp)
											ColorPicker("arrow color", SETTINGS::settings.fov_col);
										//Slider("Field Of View", -179, 179, SETTINGS::settings.fov);
									}
								}
								GroupboxEnd();
								GroupboxBegin("other", 125);
								{
									if (SETTINGS::settings.esp_bool)
									{
										Checkbox("Glow", SETTINGS::settings.glow);
										if (SETTINGS::settings.glow) {
											ColorPicker("enemy", SETTINGS::settings.glowc);
										}
										Checkbox("LBY Indicator", SETTINGS::settings.lbyindicator);
										Checkbox("LC Indicator", SETTINGS::settings.lcindicator);
										Checkbox("LBY delta", SETTINGS::settings.showlbydelta);
										Checkbox("Anti-aim arrow", SETTINGS::settings.antiaimside);
										Checkbox("LBY Update", SETTINGS::settings.lbyupdate);
										Checkbox("C4 Timer", SETTINGS::settings.drawc4timer);
										//Checkbox("zoom/scoped", SETTINGS::settings.showzoom);
										//Checkbox("money", SETTINGS::settings.showmoney);
										Combobox("Crosshair", xhair_type, SETTINGS::settings.xhair_type);
										Checkbox("Wireframe smoke", SETTINGS::settings.wireframesmoke);
										//Checkbox("Remove flashbang effect", SETTINGS::settings.removeflasheffect);
										Checkbox("Bullet tracers", SETTINGS::settings.beam_bool);
										if (SETTINGS::settings.beam_bool)
										{
											ColorPicker("local bullet tracer color", SETTINGS::settings.localbeam_col);
											ColorPicker("enemy bullet tracer color", SETTINGS::settings.enemybeam_col);
											ColorPicker("team bullet tracer color", SETTINGS::settings.friendlybeam_col);
										}
										Checkbox("Remove scope", SETTINGS::settings.removescope);
										Checkbox("Third-person [UP ARROW]", SETTINGS::settings.tp_bool);
										Checkbox("Fake angle rotation", SETTINGS::settings.fakeanglerotation);
										//Checkbox("player velocity", SETTINGS::settings.showplayervelocity);
										Checkbox("Night mode", SETTINGS::settings.night_bool);
										Checkbox("Full bright", SETTINGS::settings.fullbright);
										Checkbox("Draw gray", SETTINGS::settings.drawgray);
										Checkbox("Low resolution", SETTINGS::settings.lowresolution);
										Checkbox("Reverse depth", SETTINGS::settings.reversedepth);
										Checkbox("Remove sky", SETTINGS::settings.nosky);
										Checkbox("Remove postprocess", SETTINGS::settings.removepostprocess);
										Checkbox("Remove visual recoil", SETTINGS::settings.removevisualrecoil);
										Checkbox("Remove flashbang effect", SETTINGS::settings.removeflasheffect);
										Slider("Field Of View", -179, 179, SETTINGS::settings.fov);
									//	Checkbox("spectator list", SETTINGS::settings.spectatorlist);
										Checkbox("Show spread inaccuracy", SETTINGS::settings.recoil_circle);
										if (SETTINGS::settings.recoil_circle)
										{
											ColorPicker("inaccuracy color", SETTINGS::settings.spread_col);
										}
									}
								}
								GroupboxEnd();
								GroupboxBegin("chams", 90);
								{
									if (SETTINGS::settings.esp_bool)
									{
										Combobox("Type", chams_mode, SETTINGS::settings.chams_type);
										if (SETTINGS::settings.chams_type > 0)
											ColorPicker("Visible", SETTINGS::settings.vmodel_col);
										if (SETTINGS::settings.chams_type > 1)
											ColorPicker("Xqz", SETTINGS::settings.imodel_col);
										Checkbox("Metallic", SETTINGS::settings.metallic);
									//	Checkbox("Fake Angle Chams", SETTINGS::settings.fakeanglechams);
										//if (SETTINGS::settings.fakeanglechams)
										//{
											//ColorPicker("Fake Angle Chams: color", SETTINGS::settings.fake_angle_col);
										//}
									}
								}
								GroupboxEnd();
							}
						}
						break;
						case 2:
						{
							GroupboxBegin("main", 125);
							{
								Checkbox("enable misc", SETTINGS::settings.misc_bool);
								if (SETTINGS::settings.misc_bool)
								{
									Checkbox("Automatic bunny-hop", SETTINGS::settings.bhop_bool);
									Checkbox("Automatic strafer", SETTINGS::settings.strafe_bool);
									Checkbox("Circle strafe", SETTINGS::settings.othercirclestrafe);
									Checkbox("Hitmarker", SETTINGS::settings.hitmarker);
									if (SETTINGS::settings.hitmarker)
									{
										Combobox("Sound", hitmarkers, SETTINGS::settings.hitmarker_sound);
									}
									Checkbox("Clan-tag spammer", SETTINGS::settings.clantag);
									//Checkbox("logs", SETTINGS::settings.eventlogs);
									Checkbox("Watermark", SETTINGS::settings.watermark);
									Checkbox("Ragdoll force", SETTINGS::settings.ragdollforce);
									Checkbox("Reveal competitive ranks", SETTINGS::settings.rankreveal);
									Checkbox("Headshot only killfeed", SETTINGS::settings.headshotonly);
									//Checkbox("Chat spam", SETTINGS::settings.chatspam);
									Combobox("Buy bot", buybot_mode, SETTINGS::settings.buybot);
									//Checkbox("Circle strafe", SETTINGS::settings.namespam);
									Checkbox("Log weapon purchases", SETTINGS::settings.purchaselogs);
									Checkbox("Log damage dealt", SETTINGS::settings.damagelogs);
									//ColorPicker("Menu Color", SETTINGS::settings.menu_col);
								}
							}
							GroupboxEnd();
							GroupboxBegin("config", 125);
							{
								switch (Combobox("config", configs, SETTINGS::settings.config_sel))
								{
								case 0: config = "1"; break;
								case 1: config = "2"; break;
								case 2: config = "3"; break;
								case 3: config = "4"; break;
								case 4: config = "5"; break;
								case 5: config = "6"; break;
								}

								if (Button("Load Config"))
								{
									SETTINGS::settings.Load(config);
									GLOBAL::Msg("Configuration loaded.    \n");
								}

								if (Button("Save Config"))
								{
									SETTINGS::settings.Save(config);
									GLOBAL::Msg("Configuration saved.    \n");
								}
							}
							/*
							if (Button("Load Config"))
							{
								SETTINGS::settings.Load(config);

								INTERFACES::cvar->ConsoleColorPrintf(CColor(200, 255, 0, 255), "[UWUhook] ");
								GLOBAL::Msg("Configuration loaded.    \n");
							}

							if (Button("Save Config"))
							{
								SETTINGS::settings.Save(config);

								INTERFACES::cvar->ConsoleColorPrintf(CColor(200, 255, 0, 255), "[UWUhook] ");
								GLOBAL::Msg("Configuration saved.    \n");
							}*/
							GroupboxEnd();
						}
						break;
						}
					}
				}
				WindowEnd();
			}
			else
			{
			}
		}
	}
}