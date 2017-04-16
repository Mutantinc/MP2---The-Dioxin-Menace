//-------------------------------------------------------------------
//MINIMAL.CPP - SOURCE DE BASE: INCLUDES + WINMAIN
//-------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------------------------
#include <windows.h>				//for Windows
#include "mg_defines.h"				//all defines

//#include <exception>
//#include <string>
//#include <vector>					//objects' lists
//using namespace std;				//to avoid writing std:: everywhere

#ifndef MG_USE_SMALLEST_CODE
#include <stdio.h>					//I/O
#include <math.h>
#else
#include "mg_crt.h"					//CRT functions replacement
#include "rtlib2.h"					//maths functions replacement
									//in this mode, you'll probably need to recompile minifmod
#endif


#ifndef MG_USE_SMALLEST_CODE
#include "resource.h"				//resources declarations
#endif
//-----------------------------------------------------------

#include "mg_types.h"				//basic types
#include "mg_global.h"				//global vars
#include "mg_mem.h"					//files and memory management
#include "mg_list.h"				//list management template
#include "mg_add.h"					//bazaar & misc
#include "mg_impexp.h"

//-----------------------------------------------------------

#ifdef MG_USE_FMUSIC

#ifdef MG_USE_FMOD					//sound & melody
#include "../fmod/lib/fmod.h"
#elif defined(MG_USE_MINIFMOD)
#include "../minifmod/lib/minifmod.h"
#endif

#include "mg_fmod.h"
#endif

//-----------------------------------------------------------
#include "mg_vector.h"
#include "mg_quaternion.h"
#include "mg_matrix.h"
//-----------------------------------------------------------

#ifdef MG_USE_OPENGL
#include <gl/gl.h>					//OpenGL 32
#include <gl/glu.h>					//GLu32
#include <gl/glaux.h>				//GLaux
#ifdef MG_USE_OPENGL_EXT
#include "glext.h"					//extensions openGL 1.2 (multitexturing...)
#endif
#include "mg_ogl.h"
#endif

//#pragma comment(lib, "opengl32.lib")
//#pragma comment(lib, "glu32.lib")
//#pragma comment(lib, "glaux.lib")

//-----------------------------------------------------------

#include "mg_time.h"				//functions to manage time
#include "mg_timeline.h"			//effects' structure

#ifdef MG_USE_TEMPO
#include "mg_tempo.h"				//effets' scheduler
#endif

//-----------------------------------------------------------

#include "mg_bone.h"
#include "mg_vertex.h"
#include "mg_mapping.h"

#include "mg_texture.h"				//graphic files & textures management
#include "mg_material.h"
#include "mg_ogl2D.h"				//additional 2D-only OpenGL functions

#include "mg_face.h"
#include "mg_keyframe.h"
#include "mg_object.h"
#include "mg_mesh.h"
#include "mg_mesh2.h"
#include "mg_light.h"
#include "mg_camera.h"
#include "mg_scene.h"

//-----------------------------------------------------------

#include "data.h"					//included data big setup
#include "resource.h"
#include "mg_effects.h"				//the effects temselves

//-----------------------------------------------------------

#ifndef MG_USE_SMALLEST_CODE

#ifdef MG_USE_SETUP
#include "mg_setup.h"				//classic setup dialog
#elif defined( MG_USE_SKINSETUP )
#include "mg_skindlg.h"				//skinned setup dialog
#endif


#ifdef MG_USE_EXTSETUP
#include "mg_extsetup.h"			//runtime setup
#endif

#endif

//-----------------------------------------------------------

#include "mg_keyb.h"				//keyboard management

//-----------------------------------------------------------


//WINMAIN
#ifndef MG_USE_SMALLEST_CODE
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
#else
int WINAPI WinMainCRTStartup()
#endif
{


	//ZLIB ARCHIVE INITIALIZATION
#ifdef MG_DATA_DEFAULT_ZLIB
#ifdef MG_DATA_ZLIB_PRELOAD
	mgZlibFileHandle = mgMemOpen( mgZlibFile );
#elif defined( MG_DATA_ZLIB_RESOURCE )
	mgZlibFileHandle = mgRcOpen( (char*)MG_ZLIB_FILE );
#endif
	if( !mgZlibFileHandle )
		FatalError(strcat("Archive not found: ", mgZlibFile));
#endif



#ifdef MG_USE_SMALLEST_CODE
	//*****
	//mgIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MPICON));	//loads the icon
	mgIcon = LoadIcon(NULL, IDI_APPLICATION);
#else
	mgIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MPICON));	//loads the icon
#endif

	MSG msg;
	active=true;

	
	//KEYBOARD INITIALIZATION
	memset(mgKeys, 0, sizeof(mgKeys));		//clears keyboard buffer
	mgKeyBoard = &mgKeyBoardDefault;				//default keyboard manager

	
	//DISPLAY INITIALIZATION
#ifndef MG_USE_SMALLEST_CODE
#ifdef MG_USE_SETUP
	if( !mgSetupDlg(hInstance) )
		return 0;
#elif defined( MG_USE_SKINSETUP )
	if( !mgSkinSetupDlg(hInstance) )
		return 0;
#endif
#endif

	if ( !CreateWin() )
	{
		KillWindow();
		return 0;
	}

	
	//FONT INITIALIZATION (hDC must exist, so placed after CreateWin)
	mgFontInit(hDC, mgFontName, mgFontHeight, mgFontWeight, &mgFontList, mgFontTex);
	
	
	//SOUND INITIALIZATION
#ifdef MG_USE_FMUSIC
	mgSoundInit(MG_DATA_FILE);
#endif
	
	
	//TIMELINE INITIALIZATION
	CurTimeLine.Effects = CurEffects;
	CurTimeLine.Start();

	//END OF INITIALIZATION
	//-----------------------------------------------------------

	//BEGINING OF THE MESSAGES' PUMP
	while( !done )
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				done = true;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (active && !CurTimeLine.Run())		//draws the scene
			{
				done = true;						//if failed
			}
			else
			{
				if(mgKeyBoard()) {done = true;}
			}
		}
	}
	

	//SHUTDOWN EVERYTHING
#ifdef MG_USE_FMUSIC
	mgSoundKill();
#endif

#ifdef MG_DATA_DEFAULT_ZLIB
#ifdef MG_DATA_ZLIB_PRELOAD
	mgMemClose( mgZlibFileHandle );
#elif defined( MG_DATA_ZLIB_RESOURCE )
	mgRcClose( mgZlibFileHandle );
#endif
#endif

	KillWindow();
	mgFontKill(mgFontList);

	return(msg.wParam);
}





//END
