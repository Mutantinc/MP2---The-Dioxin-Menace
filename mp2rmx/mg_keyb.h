//-------------------------------------------------------------------
//MG_KEYB.H - LES GESTIONNAIRES DU CLAVIER
//-------------------------------------------------------------------
#ifndef _MG_KEYB_H_
#define _MG_KEYB_H_


#define		MG_KB_QUIT		VK_ESCAPE
#define		MG_KB_SW_FULL	VK_F1
#define		MG_KB_EXT_SETUP	VK_F2

#define		MG_KB_UP		VK_UP
#define		MG_KB_DOWN		VK_DOWN
#define		MG_KB_LEFT		VK_LEFT
#define		MG_KB_RIGHT		VK_RIGHT
#define		MG_KB_PITCH_UP	VK_NUMPAD8					//=tanguage (forward-back)
#define		MG_KB_PITCH_DN	VK_NUMPAD2
#define		MG_KB_YAW_UP	VK_NUMPAD6					//=lacet	(left-right)
#define		MG_KB_YAW_DN	VK_NUMPAD4
#define		MG_KB_ROLL_UP	VK_NUMPAD9					//=roulis (around the axis of view)
#define		MG_KB_ROLL_DN	VK_NUMPAD7



bool mgKeybTest( int key )
{
	if( mgKeys[key])
	{
		mgKeys[key] = false;
		return true;
	}
	else
		return false;

}


//-------------------------------------------------------------------
//DEFAUT MANAGER (return true to stop the program)
//-------------------------------------------------------------------
bool mgKeyBoardDefault(void)
{
	if( mgKeybTest(MG_KB_QUIT) )
		return true;

#ifdef MG_USE_SWITCH_FULLSCREEN
	if( mgKeybTest(MG_KB_SW_FULL) )					//switches windowed <-> fullscreen
	{
		KillWindow();
		mgFullScreen=!mgFullScreen;
		if (!CreateWin())
		{
			return true;
		}
	}
#endif
				
				
#ifdef MG_USE_EXTSETUP
	if( mgKeybTest(MG_KB_EXT_SETUP) )
	{
		KillWindow();
		mgExtSetupDlg( hInstance );
		if (!CreateWin())
		{
			return true;
		}
	}
#endif


#ifdef MG_USE_FREELOOK
	if( mgKeybTest(MG_KB_UP) )
		mgFreeLookPosZ-=2;

	if( mgKeybTest(MG_KB_DOWN) )
		mgFreeLookPosZ+=2;

	if( mgKeybTest(MG_KB_LEFT) )
		mgFreeLookPosX-=2;

	if( mgKeybTest(MG_KB_RIGHT) )
		mgFreeLookPosX+=2;

	if( mgKeybTest(MG_KB_PITCH_UP) )
		mgFreeLookRotX+=2;

	if( mgKeybTest(MG_KB_PITCH_DN) )
		mgFreeLookRotX-=2;

	if( mgKeybTest(MG_KB_YAW_UP) )
		mgFreeLookRotY-=2;

	if( mgKeybTest(MG_KB_YAW_DN) )
		mgFreeLookRotY+=2;

	if( mgKeybTest(MG_KB_ROLL_UP) )
		mgFreeLookRotZ+=2;

	if( mgKeybTest(MG_KB_ROLL_DN) )
		mgFreeLookRotZ-=2;
#endif

	return false;
}








//-------------------------------------------------------------------
//MAP KEYBOARD DURING RUNTIME
//-------------------------------------------------------------------
bool mgKeyBoardRunTime(void)
{
	//gérer ici des touches supplémentaires

	return mgKeyBoardDefault();
}






//END
#endif
