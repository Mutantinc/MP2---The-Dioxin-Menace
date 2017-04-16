//-------------------------------------------------------------------
//MINIMAL.CPP - SOURCE PRINCIPAL - INIT & UNINIT + CALLBACKS & etc...
//-------------------------------------------------------------------


//---------------------------------------------------------------------------------------------
// macros
//---------------------------------------------------------------------------------------------
#define RELEASE(x)	if( x != 0 ) { x->Release(); x = NULL; }
#define MAXINT 0x7FFFFFFF
#define mod_name "mp2.xm"
#define blob_name "mp2.dat"

//---------------------------------------------------------------------------------------------
// defines + sdk pour fmod "spécial Rez"
//---------------------------------------------------------------------------------------------
#define WIN32_LEAN_AND_MEAN


#undef USE_D3D
//#define USE_D3D


#undef	USE_FPSCT
//#define	USE_FPSCT		//le conteur de FPS


#undef USE_FMOD
//#define USE_FMOD		//pour utiliser fmod (par défaut si on demande les deux à la fois)


#undef	USE_MINIFMOD
#ifndef USE_FMOD
	#define USE_MINIFMOD	//pour utiliser minifmod
#endif


#undef USE_FMUSIC
#if (defined(USE_MINIFMOD) || defined(USE_FMOD))
#define USE_FMUSIC
#endif


#undef USE_SETUP
#define USE_SETUP


#undef USE_TEMPO
//#define USE_TEMPO


//---------------------------------------------------------------------------------------------
// includes
//---------------------------------------------------------------------------------------------
#include <windows.h>
//#include <windowsx.h>
#include <mmsystem.h>		//c'est pour timeGetTime()
#include <stdio.h>
#include <ddraw.h>

#ifdef USE_D3D
	#define D3D_OVERLOADS
	#include <d3d.h>
#endif

#ifdef USE_SETUP
	//include "mp2setup.h"	//pour le setup dialog
#endif

#include "resource.h"		//pour les resources
#include "fmod_add.h"		//pour la gestion de fmod

#include "memmgr.h"			//pour la gestion des blobs & mémoire
#include "demo_add.h"		//les constantes et tout le bazar qui sert à rien
#include "gfxloaders.h"		//les loaders d'images
#include "prim2D.h"			//les routines de Blit et d'effets spéciaux
#include "effets.h"			//les effets, les vrais

#ifdef USE_TEMPO
	#include "tempo.h"		//pour changer les temps des effets
#endif

//---------------------------------------------------------------------------------------------
// links
//---------------------------------------------------------------------------------------------
#ifdef _MSC_VER
#pragma comment(lib,"ddraw.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#endif

//---------------------------------------------------------------------------------------------
// variables
//---------------------------------------------------------------------------------------------
LPDIRECTDRAW7           lpDD;			//Objet direct Draw
LPDIRECTDRAWSURFACE7    lpDDSPrimary;	//Front Buffer
LPDIRECTDRAWSURFACE7    lpDDSBack;		//Back Buffer
LPDIRECTDRAWSURFACE7	lpDDSTemp;		//Back Buffer pour le pixel à pixel en system memory
LPDIRECTDRAWCLIPPER		lpDDClipper;	//un clipper pour quand on n'est pas en fullscreen

#ifdef USE_D3D
LPDIRECT3D7				lpD3D;			//une interface D3D
LPDIRECT3DDEVICE7		lpD3DDevice;	//un device D3D
#endif

HWND                    hwnd;			//Window Handle
BOOL                    bActive;		//L'app est active?
HFONT					Font;			//une fonte pour les petits messages à l'écran


#ifdef USE_FMUSIC
	FMUSIC_MODULE *mod = NULL;
#endif

	
	
	
	
//-------------------------------------------------------------------------------------------------
// LOCK & UNLOCK: pour pouvoir bourrer dans la ram direct à la porcos!!
//-------------------------------------------------------------------------------------------------
BYTE *Lock(LPDIRECTDRAWSURFACE7 lpDDS)
{
	DDSURFACEDESC2 ddsd;
	HRESULT hr;

	if(lpDDS==NULL) return NULL;

	ddsd.dwSize=sizeof(ddsd);

	hr=lpDDS->Lock(NULL,&ddsd,DDLOCK_WAIT|DDLOCK_SURFACEMEMORYPTR|DDLOCK_WRITEONLY|DDLOCK_NOSYSLOCK,NULL);
	if (hr != DD_OK)
		ddsd.lpSurface=NULL;

	return (BYTE *)ddsd.lpSurface;
}


void Unlock(LPDIRECTDRAWSURFACE7 lpDDStoUnlock)
{
	if(lpDDStoUnlock==NULL) return;
	lpDDStoUnlock->Unlock(NULL);
}


//---------------------------------------------------------------------------------------------
// FLIP: échange des buffers
//---------------------------------------------------------------------------------------------
void Flip(void)
{	
	if(fullscreen)
	{
		while( 1 )
			{
				HRESULT ddrval;
				ddrval = lpDDSPrimary->Flip( NULL, 0 );				
				if( ddrval == DD_OK ) break;
				if( ddrval == DDERR_SURFACELOST )
				{
					ddrval = lpDDSPrimary->Restore();
					if( ddrval != DD_OK ) break;					
					ddrval = lpDDSBack->Restore();
					if( ddrval != DD_OK ) break;					
				}
			}
	}
	else
	{
		RECT srcr,dstr;
		POINT pt;

		GetClientRect(hwnd,&srcr);
		pt.x=pt.y=0;
		ClientToScreen(hwnd,&pt);
		dstr=srcr;
		dstr.left+=pt.x;
		dstr.right+=pt.x;
		dstr.top+=pt.y;
		dstr.bottom+=pt.y;
		lpDDSPrimary->Blt(&dstr,lpDDSBack,&srcr,DDBLT_ASYNC,0);
	}
}


//---------------------------------------------------------------------------------------------
// LA FONCTION QUI DECLENCHE LES EFFETS DE LA DEMO EST LA !!!
//---------------------------------------------------------------------------------------------
void HeartBeat(void)
{

#ifdef USE_FPSCT
	//pas précis: à refaire
	int fps=1000/(timeGetTime()-basetime-efftm+1);
#endif

	efftm=timeGetTime()-basetime;
	

#ifdef USE_FMUSIC
	if (mod != NULL)
	{
		row_av=row_ap;
		row_ap=FMUSIC_GetRow(mod);
		if(row_av > row_ap)
			row_av=-1;

		ord_ap=FMUSIC_GetOrder(mod);
	}
#endif



#ifdef USE_FMUSIC
	BOOL boolts;
	if (ord_ts == -1 || row_ts == -1)										//alors on doit se fier au temps uniquement
		boolts=(efftm < thistm);											//ne pas remplacer par <= au cas où thistm=0; pour changer d'effet
	else
		boolts=( ord_ap != ord_ts || row_ap < row_ts || row_av >= row_ts );	//sinon on se fie aux effets de Rez

	if (boolts)
#else
	if ( efftm < thistm )
#endif
	{
		(*(effets[noeffet].fonc))();
		frame++;
		
		//framecounter + renseignements divers
		#ifdef USE_FPSCT
			char fps_buffer[60];
			//sprintf(fps_buffer,"fps:%d    frames:%d    order:%d    row:%d",fps,frame,ord_ap,row_ap);
			sprintf(fps_buffer,"ord:%d  row:%d",ord_ap,row_ap);
			Print(lpDDSBack,0,0,fps_buffer,blanc);
		#endif
		
		Flip();
	}
	else
	{

		basetime=timeGetTime();	//on met efftm parce qu'un effet peut ne pas se terminer sur thistm
		noeffet+=1;
		if(effets[noeffet].fonc==NULL)
		{

			#ifdef USE_FMUSIC
				FMUSIC_FreeSong(mod);
				FSOUND_Close();
			#endif

			PostMessage(hwnd, WM_CLOSE, 0, 0);	//on quitte proprement en le demandant à Windows
		}

		thistm=effets[noeffet].tm;
		#ifdef USE_FMUSIC
			row_ts=effets[noeffet].row;
			ord_ts=effets[noeffet].ord;
		#endif
	}
}


//---------------------------------------------------------------------------------------------
// UNINIT: On termine tout
//---------------------------------------------------------------------------------------------
void Uninit(void)
{
	HDC dc=CreateDC("DISPLAY",NULL,NULL,NULL);
	SetSystemPaletteUse( dc, SYSPAL_STATIC );
	DeleteDC(dc);


#ifdef USE_D3D
	if( lpD3D != NULL)
	{
		lpD3D->Release();
		lpD3D = NULL;
	}
	
 
	if( lpD3DDevice )
	{
		lpD3DDevice->Release();	//le result doit être à 0 mais de toute façon on uninit
		lpD3DDevice = NULL;
	}
#endif


	if( lpDD != NULL )
    {
        if(fullscreen) lpDD->RestoreDisplayMode();
		if(lpDDSPrimary != NULL )
        {
            lpDDSPrimary->Release();
            lpDDSPrimary = NULL;			
        }

		if(!fullscreen) 
			if(lpDDSBack!=NULL) lpDDSBack->Release();		
		lpDDSBack=NULL;
		
		lpDD->Release();
        lpDD = NULL;
    }

	if (hwnd!=0) 
	{DestroyWindow(hwnd); hwnd=0;}
	ShowCursor(TRUE);
	DeleteObject(Font);
}


//---------------------------------------------------------------------------------------------
// Boucle de messages principale
//---------------------------------------------------------------------------------------------
LRESULT CALLBACK WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
    {
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
	     return FALSE;
		 break;

		case WM_ACTIVATEAPP:
			bActive = wParam;
			break;

		case WM_SETCURSOR:
			if(fullscreen)
				SetCursor(NULL);
			break;

		case WM_KEYDOWN:
			switch( wParam )
			{
				case VK_ESCAPE:
					PostMessage(hWnd, WM_CLOSE, 0, 0);
					break;
			}
			break;

			//case WM_MOVE:	//je le rajoute ou pas?

			case WM_CLOSE:
			case WM_DESTROY:   
			if( lpDD != NULL ) 
				Uninit();
				
			PostQuitMessage(0); 
			break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}


//---------------------------------------------------------------------------------------------
// ERROR: Gestion des messages d'erreur
//---------------------------------------------------------------------------------------------
void Error(char *str)
{
	if(lpDD!=NULL) lpDD->SetCooperativeLevel(hwnd, DDSCL_NORMAL);
		MessageBox(hwnd,str,"Boulard !",MB_ICONERROR|MB_OK|MB_TASKMODAL);

	Uninit();			
}


//---------------------------------------------------------------------------------------------
// INIT: Toute l'initialisation
//---------------------------------------------------------------------------------------------
void Init(LPGUID lpguid)	//lpguid est le driver de carte vidéo
{
	DDSURFACEDESC2 ddsd;
	DDSCAPS2 ddscaps;
	HRESULT hr;
	GUID NullGUID={0,0,0,0,0,0,0,0,0,0,0};
	
	// Object already initialized
	if (lpDD!=NULL) return;

	// If handle is null, create a new window
	if(hwnd==NULL)
	{
		WNDCLASSEX    wcex;	
		wcex.cbSize             = sizeof(wcex);
		wcex.style              = CS_DBLCLKS;
		wcex.lpfnWndProc        = WindowProc;
		wcex.cbClsExtra         = 0;
		wcex.cbWndExtra         = 0;
		wcex.hInstance          = GetModuleHandle(NULL);
		wcex.hIcon              = LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_MPICON));
		wcex.hCursor            = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground      = HBRUSH(GetStockObject(BLACK_BRUSH));
		wcex.lpszMenuName       = NULL;
		wcex.lpszClassName      = "MUTANT";
		wcex.hIconSm            = NULL;

		if(!RegisterClassEx(&wcex))
		{
			Error("Cannot register window class");
			return;
		}
    
		//
		// Create Main Window
		//
		RECT r={0,0,width,height};
		DWORD style= WS_SYSMENU | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

		AdjustWindowRectEx(&r,style,FALSE,NULL);	//pour calculer la taille de la fenêtre à partir de celle de l'aire client


		hwnd = CreateWindowEx( NULL, "MUTANT", "Mutant.Pouletz.2 - The Dioxin Menace !", style, r.left,r.top,r.right,r.bottom, NULL, NULL, GetModuleHandle(NULL), NULL );
		if (hwnd==NULL) { Error("Cannot create window"); return; }


		SetWindowPos( hwnd, NULL, 0, 0, r.right-r.left, r.bottom-r.top, SWP_NOZORDER/*| SWP_NOMOVE*/);
		ShowWindow( hwnd, SW_NORMAL );
		UpdateWindow(hwnd);
		//SetFocus(hwnd);
	}

	//portnawak !	//utiliser avec un driver GUID de carte vidéo....
	if(lpguid!=NULL)
		if(memcmp(lpguid,&NullGUID,sizeof(GUID))==0) 
		{
			lpguid=NULL;
		}
	
	if(!fullscreen) 
	{
		lpguid=NULL;
		HDC dc=CreateDC("DISPLAY",NULL,NULL,NULL);
		depth=GetDeviceCaps(dc,BITSPIXEL);
		DeleteDC(dc);
	}


	//création de l'interface DD
	if(DirectDrawCreateEx(NULL, (VOID**)&lpDD, IID_IDirectDraw7, NULL) == DD_OK )
	{

		if(fullscreen)
		{
			hr=lpDD->SetCooperativeLevel(hwnd, DDSCL_NOWINDOWCHANGES | DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN /*| DDSCL_ALLOWREBOOT*/ );

			if(FAILED(hr))
				lpDD->SetCooperativeLevel(hwnd, DDSCL_NORMAL);
		}

		else
			lpDD->SetCooperativeLevel(hwnd, DDSCL_NORMAL);
	}
	else 
	{
		Error("Unable to create DirectDraw Object");
		return;
	}	
		
	if(fullscreen)
		if(lpDD->SetDisplayMode(width, height, depth, 0, 0) != DD_OK)
		{
			Error("Unable to switch to requested video mode");
			return;
		}


////////////////////////////////////////////////////////////////
//création des buffers	
	ZeroMemory(&ddsd,sizeof(ddsd));			//il n'y a pas de constructeur de dd***
	ZeroMemory(&ddscaps,sizeof(ddscaps));	//idem

	ddsd.dwSize = sizeof(ddsd);
	
	if(fullscreen)
	{
		//Primary & Back buffers
		ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		ddsd.dwBackBufferCount = 1;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
							  DDSCAPS_FLIP |
							  DDSCAPS_COMPLEX |
							  DDSCAPS_VIDEOMEMORY|DDSCAPS_3DDEVICE;

		if(lpDD->CreateSurface(&ddsd, &lpDDSPrimary, NULL) != DD_OK)
		{
			ddsd.ddsCaps.dwCaps &= ~DDSCAPS_VIDEOMEMORY;
			if(lpDD->CreateSurface(&ddsd, &lpDDSPrimary, NULL) != DD_OK)
			{
				Error("Unable to create Primary Surface");
				return;		
			}
		}

		//ontrouve le pitch
		lpDDSPrimary->GetSurfaceDesc(&ddsd);
		pitch=ddsd.lPitch/4;

		// Gets BackBuffer for fullscreen
		ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
		if( lpDDSPrimary->GetAttachedSurface( &ddscaps, &lpDDSBack ) != DD_OK )
		{
			Error("Unable to get the Back Buffer");
			return;
		}
	}
	else
	{
		// Primary
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE|DDSCAPS_3DDEVICE;

		if(lpDD->CreateSurface(&ddsd, &lpDDSPrimary, NULL) != DD_OK)
		{
			Error("Unable to create Primary Surface");
			return;
		}
		//on trouve le pitch
		pitch=width;

		
		// Backbuffer
		ddsd.dwFlags = DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN|DDSCAPS_3DDEVICE;
		ddsd.dwWidth=width;
		ddsd.dwHeight=height;


		if(lpDD->CreateSurface(&ddsd, &lpDDSBack, NULL) != DD_OK)
		{
			Error("Unable to get the Back Buffer");		
			return;
		}


		//**** voir l'utilité de ce clipper...
		lpDD->CreateClipper(0,&lpDDClipper,NULL);
		lpDDClipper->SetHWnd(0, hwnd);			
		lpDDSPrimary->SetClipper(lpDDClipper);
		lpDDClipper->Release();

	}



		//SystemMemory Temp buffer pour mes effets 2D pix-à-pix
		ZeroMemory(&ddsd,sizeof(ddsd));
		ZeroMemory(&ddscaps,sizeof(ddscaps));
		ddsd.dwSize=sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY|DDSCAPS_3DDEVICE;
		ddsd.dwWidth=width;
		ddsd.dwHeight=height;

		if(lpDD->CreateSurface(&ddsd, &lpDDSTemp, NULL) != DD_OK)
		{
			Error("Unable to create SysMem Temp Buffer");
			return;
		}










#ifdef USE_D3D
/////////////////////////////////////////////////////////////////
//INIT LA PARTIE 3D

	//on crée l'interface entre DD et D3D puis entre D3D et la carte vidéo
    if(FAILED(lpDD->QueryInterface(IID_IDirect3D7, (VOID**)&lpD3D)))
        Error("D3D object couldn't be initialized");


    // The GUID here is hard coded. In a real-world application this should be retrieved by enumerating devices.
    if(FAILED(lpD3D->CreateDevice(IID_IDirect3DHALDevice, lpDDSBack, &lpD3DDevice)))
    {
        // pas de hard-> on essaie le soft
        if(FAILED(lpD3D->CreateDevice(IID_IDirect3DRGBDevice, lpDDSBack, &lpD3DDevice)))
            Error ("Neither a HAL not a HEL could be initialized");
    }


	//init le viewport
	D3DVIEWPORT7 vp = { 0, 0, width, height, 0.0f, 1.0f };	//**** mettre les deux à 0.0f pour rendre sur un plan
    if(FAILED(lpD3DDevice->SetViewport(&vp)))
        Error("Could not initialize Viewport");

	//material
	D3DMATERIAL7 mtrl;
    ZeroMemory( &mtrl, sizeof(mtrl));
    mtrl.ambient.r = 1.0f;
    mtrl.ambient.g = 1.0f;
    mtrl.ambient.b = 1.0f;
    lpD3DDevice->SetMaterial(&mtrl);

	//lumière
    lpD3DDevice->SetRenderState(D3DRENDERSTATE_AMBIENT,0xffffffff);


	//matrices de transformation
    // The world matrix controls the position and orientation of the polygons in world space
    // The view matrix defines the position and orientation of the camera
    // The projection matrix defines how the 3-D scene is "projected" onto the 2-D render target surface

	D3DMATRIX imat;			//matrice identité
	imat._11 = imat._22 = imat._33 = imat._44 = 1.0f;
	imat._12 = imat._13 = imat._14 = imat._41 = 0.0f;
	imat._21 = imat._23 = imat._24 = imat._42 = 0.0f;
	imat._31 = imat._32 = imat._34 = imat._43 = 0.0f;
	
	D3DMATRIX matWorld = imat;

    lpD3DDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld);	//pour l'instant cette matrice reste id
 
    D3DMATRIX matView = imat;
    matView._43 = 10.0f;
    lpD3DDevice->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView);
 
    D3DMATRIX matProj = imat;
    matProj._11 =  1.0f;
    matProj._22 =  1.0f;
    matProj._34 =  1.0f;
    matProj._43 = -1.0f;
    matProj._44 =  0.0f;
    lpD3DDevice->SetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProj);
#endif











////////////////////////////////////////////////////////////////
//virer la souris en plein-écran
	if(fullscreen) while(ShowCursor(FALSE) >= 0);	

	//on crée une fonte pour divers messages
	Font = CreateFont(width < 640 ? 16 : 16,
    0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
    ANSI_CHARSET,
    OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS,
    NONANTIALIASED_QUALITY,
    VARIABLE_PITCH,
    "Arial");

////////////////////////////////////////////////////////////////


#ifdef USE_MINIFMOD
//	FSOUND_SetBufferSize(1000);
	if (!FSOUND_Init(44100, 0))
	{
		Error("module audio channel not free");
		return;
	}

	mod = FMUSIC_LoadSong(mod_name, NULL);
	if (!mod)
	{
		Error ("module missing");
		return;
	}
#elif defined(USE_FMOD)
	if (!FSOUND_Init(44100,32,0))
	{
		Error("module audio channel not free");
		return;
	}

	mod = FMUSIC_LoadSong(mod_name);
	if (!mod)
	{
		Error ("module missing");
		return;
	}
#endif




	basetime=timeGetTime();	//pour initialiser le temps avant le premier effet
}



//---------------------------------------------------------------------------------------------
// Dlg Proc pour le Setup
//---------------------------------------------------------------------------------------------
BOOL CALLBACK SetupDlgProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch(message)
	{
	case WM_ACTIVATEAPP:
		if(fullscreen)
			SendDlgItemMessage(hwnd,IDC_FULLSCREEN, BM_SETCHECK, BST_CHECKED,0);
		break;
		
		
		
	case WM_COMMAND:
		switch(wparam)
		{
		case MAKEINTRESOURCE(ID_SETUP_OK):
			DestroyWindow(hwnd);
			hwnd=0;
			Init(video);
			break;			
			
		case MAKEINTRESOURCE(ID_SETUP_EXIT):
			DestroyWindow(hwnd);
			hwnd=0;
			PostQuitMessage(0); 
			Uninit();
			break;
			
		case MAKEINTRESOURCE(IDC_FULLSCREEN):
			fullscreen = !fullscreen;
			break;
		}
		break;
		
	}
	//
	// Let the Default Proc manage the Window
	return FALSE;
}



//---------------------------------------------------------------------------------------------
// WinMain:
//---------------------------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG         msg;
	unsigned t=0,p;

    
#ifdef USE_FMUSIC
#ifndef USE_MEMLOAD
	FSOUND_File_SetCallbacks(fileopen, fileclose, fileread, fileseek, filetell);
#else
	FSOUND_File_SetCallbacks(memopen, memclose, memread, memseek, memtell);
#endif
#endif

    lpCmdLine = lpCmdLine; //commandline
    hPrevInstance = hPrevInstance; //un seul démarrage permis

//---------------------------------------------------------------------------------------------
//SetupDlg
//---------------------------------------------------------------------------------------------
#ifdef USE_SETUP
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_SETUP), NULL, SetupDlgProc, NULL);
#else
	Init(video);	//mettre un driver de carte vidéo en arg. NULL est le default driver
#endif



    while(TRUE)
    {
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE ) )
		{
			if( !GetMessage( &msg, NULL, 0, 0 ) )	//vérifie si on n'a pas reçc WM_QUIT
				break;
			else
			{
				TranslateMessage(&msg); 
				DispatchMessage(&msg);
			}
		}

		
		//routine pour limiter le framerate

		else
		{
			p=timeGetTime();
			if(bActive && p-t>1000/70) {HeartBeat(); t=p;}
		}

//		else if (bActive) HeartBeat();

//		else WaitMessage();	//nécessaire?
	}

	return 0;
}

// FIN