//-------------------------------------------------------------------
//MG_ADD.H - MORE BAZAAR
//-------------------------------------------------------------------
#ifndef _MG_ADD_H_
#define _MG_ADD_H_


//---------------------------------------------------------------------------------------------
// VARIOUS
//---------------------------------------------------------------------------------------------
#define mgPi	3.1415926535897932384626433832795f
#define mg2Pi	6.283185307179586476925286766559f

#define RAD_TO_DEG(angle)	(angle)*180.f/mgPi
#define DEG_TO_RAD(angle)	(angle)*mgPi/180.f

#define RGBA_TO_INT32( r, g, b, a )	(a<<24+b<<16+g<<8+r)


uint pow2over( uint x )	//to increase x to the next power of 2
{
	uint t = x;
	for( sint i=-1; t; t=t>>1, i++ );
	uint y=1<<i;
	if( x!=y )
		y=y<<1;
	return y;
}



//---------------------------------------------------------------------------------------------
// COLORS
//---------------------------------------------------------------------------------------------
//				  0xAABBGGRR
uint	BLACK	= 0xFF000000;
uint	RED		= 0xFF0000FF;
uint	GREEN	= 0xFF00FF00;
uint	BLUE	= 0xFFFF0000;
uint	YELLOW	= 0xFF00FFFF;
uint	PURPLE	= 0xFFFF00FF;
uint	CYAN	= 0xFFFFFF00;
uint	GREY	= 0xFF808080;
uint	WHITE	= 0xFFFFFFFF;

//---------------------------------------------------------------------------------------------
// MESSAGES & ERRORS
//---------------------------------------------------------------------------------------------
#ifndef MG_USE_SMALLEST_CODE

void Msg(char *str)
{
	MessageBox( NULL, str, "Message...", MB_OK|MB_TASKMODAL );
}


bool MsgYesNo(char *str)
{
	if (MessageBox( NULL, str, "Yes/No", MB_YESNO|MB_ICONEXCLAMATION ) == IDYES)
		return true;
	else
		return false;
}


void Error(char *str)
{
	MessageBox( NULL, str, "Boulette!", MB_ICONERROR|MB_OK|MB_TASKMODAL );
}


void FatalError(char *str)
{
	MessageBox( NULL, str, "Boulette!", MB_ICONERROR|MB_OK|MB_TASKMODAL );
	exit(0);
}

#endif

//---------------------------------------------------------------------------------------------
// WINDOWS SIZES
// NB: only modifies the two strictly necessary values
//---------------------------------------------------------------------------------------------
void GetWinP(HWND hWnd, RECT* rect)	//Window area position coords. this fx won't modify the size values
{
	RECT tmprect;
	GetWindowRect(hWnd, &tmprect);
	(*rect).left = tmprect.left;
	(*rect).top = tmprect.top;
}


void GetWinS(HWND hWnd, RECT* rect)	//Window's client area size. this fx won't modify the position values
{
	RECT tmprect;
	GetClientRect(hWnd, &tmprect);
	(*rect).right = tmprect.right;
	(*rect).bottom = tmprect.bottom;
}





//END
#endif