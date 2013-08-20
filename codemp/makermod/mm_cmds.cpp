
extern "C"
{
#include "../cgame/cg_local.h"
}

void GetTargetOrigin(vec3_t *vecOut)
{
	trace_t tr;
	vec3_t myOrigin, fPos;

	VectorCopy(cg.predictedPlayerState.origin, myOrigin);
	myOrigin[2] += cg.predictedPlayerState.viewheight;

	AngleVectors(cg.predictedPlayerState.viewangles, fPos, NULL, NULL);

	for (int i=0; i<3; i++)
		fPos[i] = myOrigin[i] + fPos[i]*16777216;

	CG_Trace(&tr, myOrigin, 0, 0, fPos, cg.clientNum, MASK_PLAYERSOLID );

	VectorMA(tr.endpos, 48, tr.plane.normal, *vecOut);
}

void CG_Telet_f(void)
{
	vec3_t fPos, telepos;
	int i;
	char arg1[MAX_TOKEN_CHARS];
	vec3_t adjustedOrigin;

	if ( trap->Cmd_Argc() > 1 )
	{ // telet <client number> (Teleport to player *THIS DOESN'T WORK FULLY*)
		trap->Cmd_Argv(1, arg1, sizeof(arg1));
		i = atoi(arg1);

		if ((i == 0) && (arg1[0] != '0'))
		{
			trap->Print( "Selection by player name not supported.\n" );
			return;
		}

		if (i>MAX_CLIENTS || i<0)
			return;

		VectorCopy(cg_entities[i].currentState.pos.trBase, telepos);

		// This line so you don't telefrag the other player...
		telepos[2] += 80;
	}
	else // telet (Teleport to target)
		GetTargetOrigin(&telepos);

	trap->SendClientCommand(va("mtele %i %i %i %i", (int)telepos[0], (int)telepos[1], (int)telepos[2], (int)cg.refdef.viewangles[YAW]));
}

void CG_Markt_f(void)
{
	vec3_t	telepos;

	GetTargetOrigin(&telepos);

	trap->SendClientCommand(va("mmark %i %i %i", (int)telepos[0], (int)telepos[1], (int)telepos[2]));
}

void CG_Destt_f(void)
{
	vec3_t	telepos;

	GetTargetOrigin(&telepos);

	trap->SendClientCommand(va("mdest %i %i %i", (int)telepos[0], (int)telepos[1], (int)telepos[2]));
}

void CG_MarkOffset_f(void)
{
	if (trap->Cmd_Argc() < 4)
		trap->Print( "Command usage: ^1markoffset <x> <y> <z>\n" );
	else
	{
		char arg1[MAX_TOKEN_CHARS];
		char arg2[MAX_TOKEN_CHARS];
		char arg3[MAX_TOKEN_CHARS];

		trap->Cmd_Argv(1, arg1, sizeof(arg1));
		trap->Cmd_Argv(2, arg2, sizeof(arg2));
		trap->Cmd_Argv(3, arg3, sizeof(arg3));
		trap->SendClientCommand(va("mmark %i %i %i", (int)cg.predictedPlayerState.origin[0]+atoi(arg1)*10, (int)cg.predictedPlayerState.origin[1]+atoi(arg2)*10, (int)cg.predictedPlayerState.origin[2]+atoi(arg3)*10));
	}
}

void CG_MarktOffset_f(void)
{
	vec3_t	telepos;

	if (trap->Cmd_Argc() < 4)
		trap->Print( "Command usage: ^1marktoffset <x> <y> <z>\n" );
	else
	{
		char arg1[MAX_TOKEN_CHARS];
		char arg2[MAX_TOKEN_CHARS];
		char arg3[MAX_TOKEN_CHARS];

		trap->Cmd_Argv(1, arg1, sizeof(arg1));
		trap->Cmd_Argv(2, arg2, sizeof(arg2));
		trap->Cmd_Argv(3, arg3, sizeof(arg3));
		
		GetTargetOrigin(&telepos);

		trap->SendClientCommand(va("mmark %i %i %i", (int)telepos[0]+atoi(arg1)*10, (int)telepos[1]+atoi(arg2)*10, (int)telepos[2]+atoi(arg3)*10));
	}
}

void CG_MarkSky_f(void)
{
	trace_t	tr;
	vec3_t	fPos;

	VectorCopy(cg.predictedPlayerState.origin, fPos);
	fPos[2] += 16777216;

	CG_Trace(&tr, cg.predictedPlayerState.origin, 0, 0, fPos, cg.clientNum, (CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_BODY|CONTENTS_TERRAIN) );

	trap->SendClientCommand(va("mmark %i %i %i", (int)tr.endpos[0], (int)tr.endpos[1], (int)tr.endpos[2]));
}

void CG_MarkSkyOffset_f(void)
{
	trace_t	tr;
	vec3_t	fPos;

	if (trap->Cmd_Argc() < 4)
		trap->Print( "Command usage: ^1markskyoffset <x> <y> <z>\n" );
	else
	{
		char arg1[MAX_TOKEN_CHARS];
		char arg2[MAX_TOKEN_CHARS];
		char arg3[MAX_TOKEN_CHARS];

		trap->Cmd_Argv(1, arg1, sizeof(arg1));
		trap->Cmd_Argv(2, arg2, sizeof(arg2));
		trap->Cmd_Argv(3, arg3, sizeof(arg3));

		VectorCopy(cg.predictedPlayerState.origin, fPos);
		fPos[2] += 16777216;

		CG_Trace(&tr, cg.predictedPlayerState.origin, 0, 0, fPos, cg.clientNum, (CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_BODY|CONTENTS_TERRAIN) );

		trap->SendClientCommand(va("mmark %i %i %i", (int)tr.endpos[0]+atoi(arg1)*10, (int)tr.endpos[1]+atoi(arg1)*10, (int)tr.endpos[2]+atoi(arg1)*10));
	}
}

void CG_MarkOB_f(void)
{
	vec3_t  telepos;
	int i;
	char arg1[MAX_TOKEN_CHARS];

	if (trap->Cmd_Argc() < 2)
		trap->Print( "Command usage: ^1markob <ob>\n" );
	else
	{
		trap->Cmd_Argv( 1, arg1, sizeof(arg1) );
		i = atoi( arg1 );

		if (i<0)
			return;

		VectorCopy(cg_entities[i].currentState.pos.trBase, telepos);
		trap->SendClientCommand(va("mmark %i %i %i", (int)telepos[0], (int)telepos[1], (int)telepos[2]));
	}
}

void CG_Rotatet_f(void)
{
	if(trap->Cmd_Argc() < 2)
		trap->Print( "Command usage: ^1rotatet <ob>\n" );
	else
	{
		char arg[MAX_STRING_CHARS] = { 0 }, arg1[MAX_STRING_CHARS] = { 0 };

		trap->Cmd_Argv(1, arg, sizeof(arg));
		trap->Cmd_Argv(2, arg1, sizeof(arg1));

		centity_t *cent = &cg_entities[atoi(arg)];

		trap->SendClientCommand(va("%s %i %i %i", 
			(trap->Cmd_Argc() >= 3 && atoi(arg1) ? "msetangles" : "mrotate"), 
			(int)cg_entities[cg.clientNum].lerpAngles[0] - (int)cent->lerpAngles[0], 
			(int)cg_entities[cg.clientNum].lerpAngles[1] - (int)cent->lerpAngles[1], 
			(int)cg_entities[cg.clientNum].lerpAngles[2] - (int)cent->lerpAngles[2]));
	}
}

void CG_Movet_f(void)
{
	if(trap->Cmd_Argc() < 2)
		trap->Print( "Command usage: ^1movet <ob>\n" );
	else
	{
		vec3_t telepos;
		char arg[MAX_STRING_CHARS] = { 0 }, arg1[MAX_STRING_CHARS] = { 0 };

		trap->Cmd_Argv(1, arg, sizeof(arg));
		trap->Cmd_Argv(2, arg1, sizeof(arg1));

		centity_t *cent = &cg_entities[atoi(arg)];
		GetTargetOrigin(&telepos);
		trap->SendClientCommand(va("%s %i %i %i", 
			(trap->Cmd_Argc() >= 3 && atoi(arg1) ? "msetorigin" : "mmove"), 
			(int)telepos[0] - (int)cent->lerpOrigin[0],
			(int)telepos[1] - (int)cent->lerpOrigin[1],
			(int)telepos[2] - (int)cent->lerpOrigin[2]));
	}
}