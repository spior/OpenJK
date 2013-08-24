#pragma once

#define PLUGIN_VER					"1.0.0"
#define VERSION						"^7P^0roxy^5M^0od ^3Client Plugin ^7v^5"PLUGIN_VER
#define VERSION_CLEAN				"ProxyMod Client Plugin v"PLUGIN_VER

//
// mm_main.cpp
//
void MM_Init(void);
void MM_Shutdown(void);
void MM_DrawActiveFrame(void);

//
// mm_smartgun.cpp
//
void MM_SmartGun(qboolean altFire, qboolean isClient, int clientNum, vec3_t origin);

//
// mm_chatbox.cpp
//
void MM_ChatBox_AddString(char *time, char *name, char *chatStr);
void MM_ChatBox_DrawStrings(void);

//
// mm_cmds.cpp
//
void CG_Telet_f(void);
void CG_Markt_f(void);
void CG_Destt_f(void);
void CG_MarkOffset_f(void);
void CG_MarktOffset_f(void);
void CG_MarkSky_f(void);
void CG_MarkSkyOffset_f(void);
void CG_MarkOB_f(void);
void CG_Rotatet_f(void);
void CG_Movet_f(void);