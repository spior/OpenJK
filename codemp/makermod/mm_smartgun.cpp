
extern "C"
{
#include "../cgame/cg_local.h"
}

int strocr(const char *Src, const char *Ocr)
{
	int num = 0;
	char *p = (char*)Src;
	while(p = strstr(p == Src ? p : ++p, Ocr)) num++;
	return num;
}

void strrpl(char *Dst, const char *Src, size_t size, const char *Rpl, const char *Rplw)
{
	int predicted_size;
	// No occurances so just return
	int ocr = strocr(Src, Rpl);
	if(ocr == 0) return;

	// Predicts the new size and gives an error if it's going to overflow
	predicted_size = (strlen(Src) - ocr * strlen(Rpl) + ocr * strlen(Rplw));
	if(predicted_size > (int)size)
		return;

	// Now this is the actual replacing
	char *temp = (char*)calloc(1, size);
	char *p1 = (char*)Src;
	char *p2 = strstr(p1, Rpl);
	while(p2)
	//for(int i=0;i<numOfReplaces;i++, p2 = strstr(p1, Rpl))
	{
		strncat(temp, p1, p2-p1);
		strcat(temp, Rplw);
		p1 = p2;
		p1+=strlen(Rpl);
		p2 = strstr(p1, Rpl);
		if(!p2) strcat(temp, p1);
	}
	strcpy(Dst, temp);
	free(temp);
}

void MM_SmartGun(qboolean altFire, qboolean isClient, int clientNum, vec3_t origin)
{
	if(!mm_SmartGun.integer)
		return;

	char *value;
	if		(!altFire && !isClient)value = mm_SmartGunCmd.string;
	else if (altFire && !isClient)	value = mm_SmartGunAltCmd.string;
	else if (!altFire && isClient)	value = mm_SmartGunCmdCl.string;
	else if	(altFire && isClient)	value = mm_SmartGunAltCmdCl.string;

	char rplw[64];
	char buf[MAX_CVAR_VALUE_STRING] = { 0 };

	strcpy(buf, value);
	sprintf(rplw, "%i", clientNum);
	strrpl(buf, buf, sizeof(buf), "$cl$", rplw);
	sprintf(rplw, "%i %i %i", (int)origin[0], (int)origin[1], (int)origin[2]);
	strrpl(buf, buf, sizeof(buf), "$xyz$", rplw);
	strcat(buf, "\n");

	trap->SendConsoleCommand(buf);
}