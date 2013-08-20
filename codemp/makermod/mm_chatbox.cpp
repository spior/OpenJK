
extern "C"
{
#include "../cgame/cg_local.h"
}

typedef struct mmCBItem_s mmCBItem_t;

typedef struct mmCBItem_s
{
	char		timef[20];
	char		name[40];
	char		string[MAX_SAY_TEXT];
	int			time;
	mmCBItem_t	*next;
	mmCBItem_t	*prev;
};

mmCBItem_t cb_head;

const int	CHATBOX_FONT_HEIGHT = 10;
const float CHATBOX_FONT_SCALE = 0.55f;
const float CHATBOX_TIME_WIDTH = 60.0f;
const float CHATBOX_NAME_WIDTH = 100.0f;
const float CHATBOX_MAX_WIDTH = 580.0f;
const float CHATBOX_TEXT_WIDTH = CHATBOX_MAX_WIDTH - (CHATBOX_TIME_WIDTH+CHATBOX_NAME_WIDTH) - 45;

void MM_ChatBox_AddString(char *time, char *name, char *chatStr)
{
	mmCBItem_t *find = &cb_head;

	while(find->next != NULL) find = find->next;
	find->next = (mmCBItem_t*)calloc(1, sizeof(mmCBItem_t));
	find->next->prev = find;
	find = find->next;
	find->next = NULL;
	
	strncpy(find->timef, time, sizeof(find->timef)-1);
	strncpy(find->name, name, sizeof(find->name)-1);
	strncpy(find->string, chatStr, sizeof(find->string)-1);

	float textWidth = CG_Text_Width(find->string, CHATBOX_FONT_SCALE, FONT_SMALL);
	if(textWidth > CHATBOX_TEXT_WIDTH)
	{
		int i, len;
		textWidth=0;
		for(i=0, len=strlen(find->string); i<len; i++)
		{
			if(Q_IsColorString(find->string+i) && i++)
				continue;
			char c[2] = { 0 }; c[0]=find->string[i];
			float width = CG_Text_Width(c, CHATBOX_FONT_SCALE, FONT_SMALL);
			if(textWidth+width>CHATBOX_TEXT_WIDTH)
				break;
			textWidth+=width;
		}
		int j=i;
		while(j)
			if(find->string[j--] == ' ')
				break;
		if(!j) j=i;
		char *text = find->string+j+1;
		char colorBuf[MAX_SAY_TEXT] = { 0 };
		strncpy(colorBuf, find->string, j+1);
		MM_ChatBox_AddString("", "", va("^%i%s", strrchr(colorBuf, '^') ? *(strrchr(colorBuf, '^')+1)-'0' : 7, text));
		find->string[j+1] = 0;
	}
	find->time = trap->Milliseconds();
}

void MM_ChatBox_DrawStrings(void)
{
	mmCBItem_t *find = &cb_head;
	int linesToDraw = 8;
	int x = 5;
	int y = cg.scoreBoardShowing ? 465 : cg_chatBoxHeight.integer;
	vec4_t	background = {0, 0, 0, 0.5f};

	while(find->next)
		find=find->next;

	y-=CHATBOX_FONT_HEIGHT*(linesToDraw-1);

	CG_FillRect(x-5, y, CHATBOX_MAX_WIDTH, CHATBOX_FONT_HEIGHT*linesToDraw, background);
	CG_FillRect(x+CHATBOX_TIME_WIDTH+CHATBOX_NAME_WIDTH, y, 1, CHATBOX_FONT_HEIGHT*linesToDraw, colorTable[CT_BLACK]);

	y+=CHATBOX_FONT_HEIGHT*(linesToDraw-1);
	y-=2;

	for(int i=0; find && i<linesToDraw; i++, y-=CHATBOX_FONT_HEIGHT, find=find->prev)
	{
		CG_Text_Paint(x, y, CHATBOX_FONT_SCALE, colorWhite, find->timef, 0, CHATBOX_TIME_WIDTH, 4, FONT_SMALL);
		CG_Text_Paint(x+CHATBOX_TIME_WIDTH, y, CHATBOX_FONT_SCALE, colorWhite, find->name, 0, CHATBOX_NAME_WIDTH, 4, FONT_SMALL);
		CG_Text_Paint(x+CHATBOX_TIME_WIDTH+CHATBOX_NAME_WIDTH, y, CHATBOX_FONT_SCALE, colorWhite, find->string, 0, -1, 4, FONT_SMALL);
	}
}