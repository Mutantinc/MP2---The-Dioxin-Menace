//-------------------------------------------------------------------
//MG_GLOBAL.H - LES VARS GLOBALES DE L'APPLI
//-------------------------------------------------------------------
#ifndef _MG_GLOBAL_H_
#define _MG_GLOBAL_H_


//-------------------------------------------------------------------
// VARIABLES
//-------------------------------------------------------------------
//***** gérer aspectratio et fixedsize

uint		mgWidth = 640;
uint		mgHeight = 480;
uint		mgDepth = 32;

#ifdef MG_USE_FULLSCREEN
bool		mgFullScreen = true;
#else
bool		mgFullScreen = false;
#endif

bool		mgAspectRatio = true;
bool		mgFixedSize = false;

void		(*mgViewPortMode)();					//how to initialize the screen when the viewport is changed

//-------------------------------------------------------------------

const char*	mgClassName = "MENGI";					//internal name (useful for window RegisterClass and others)

#ifdef MG_USE_SMALLEST_CODE
#define		mgDesc		""
#define		mgTitle		""
#else
#define		mgDesc		"Mutant inc. application"	//description of the application (<64 chars)
#define		mgTitle		"Mutant.Pouletz.2.Remix - The Dioxin Menace"		//window title
#endif

HICON		mgIcon;									//handle of the EXE icon and the top-left corner of the window

//-------------------------------------------------------------------

#define		mgZlibFile	"mp2rmx.dat"

//-------------------------------------------------------------------

#define		mgFontName		"Arial"					//font grabbed in windows
#define		mgFontHeight	120						//font height
#define		mgFontWeight	FW_SEMIBOLD	//FW_NORMAL	//font width

//-------------------------------------------------------------------

bool		mgKeys[256];							//keyboard switches
bool		(*mgKeyBoard)();						//function pointer of the keyboard manager

//-------------------------------------------------------------------

#ifdef MG_USE_MOUSE
uint		mgMouseX, mgMouseY;
bool		mgLButton, mgRButton;
#endif

//-------------------------------------------------------------------

#ifdef MG_USE_FREELOOK
float		mgFreeLookPosX = 0.0f;
float		mgFreeLookPosY = 0.0f;
float		mgFreeLookPosZ = 100.0f;
float		mgFreeLookRotX = 0.0f;
float		mgFreeLookRotY = 0.0f;
float		mgFreeLookRotZ = 0.0f;
#endif

//-------------------------------------------------------------------

//END
#endif