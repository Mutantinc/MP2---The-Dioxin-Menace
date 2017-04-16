//-------------------------------------------------------------------
// MG_SKINDLG.H - SKINNED DIALOGS
//-------------------------------------------------------------------

#ifndef _MG_SKINDLG_H_
#define _MG_SKINDLG_H_

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class mgSkinBitmap
{
public:
	sint width;
	sint height;
	uint32* data;
	uint32 colorkey;	//AABBGGRR; if AA == 0 => no colorkey

	inline mgSkinBitmap(void)
	{
		data=NULL;
	}

};

typedef mgSkinBitmap* mgSkinBitmapP;

//-------------------------------------------------------------------
//-------------------------------------------------------------------
#define MG_SKIN_OFF		0
#define MG_SKIN_ON		1
#define MG_SKIN_OVER	2

typedef class mgSkinButton
{
public:
	mgSkinBitmap* off;	//actually, the same bitmap can be used in several buttons
	mgSkinBitmap* on;
	sint posx;
	sint posy;
	uint state;
	bool active;		//actions enabled/disabled
	bool visible;

	inline mgSkinButton(void)
	{
		off = on = NULL;
	}


	inline uint GetState(void)
	{ return state; }


	inline void SetState( uint newstate )
	{
		if( newstate == MG_SKIN_OVER && state == MG_SKIN_ON )
			return;

		state = newstate;
	}

} *mgSkinButtonP;

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class mgSkinDlg{
public:

	mgSkinBitmap MainSkin;			//main skin 24 bits
	uint8* MainAlpha;				//alpha layer
	mgSkinBitmap Background;		//the screen background
	mgSkinBitmap Temp;				//holds the resulting bitmap before its display
	mgList<mgSkinBitmap> Bitmaps;	//all the bitmaps
	mgList<mgSkinButton> Buttons;	//all the buttons
	bool Redraw;					//true if the window needs to be redrawn
	
//-------------------------------------------------------------------
//-------------------------------------------------------------------

	inline mgSkinDlg( char* mainskin, char* mainalpha, uint mode )
	{
		Redraw = false;

		mgTexture T;
		T.LoadSwitch( mainskin, mode );
		T.ToRGBA();		//32 bits for every skin
		MainSkin.width = Temp.width = T.Width;
		MainSkin.height = Temp.height = T.Height;
		MainSkin.data = (uint32*)T.Data;
		Temp.data = (uint32*)malloc(Temp.width*Temp.height*4);

		//data access mode already set
		uint fp;
		MainAlpha = (uint8*)malloc(MainSkin.width*MainSkin.height);
		if( mainalpha == NULL || (fp = mgOpen(mainalpha)) == NULL )
		{
			Error( "Skin mask not found" );
			memset( MainAlpha, 0xff, MainSkin.width*MainSkin.height );
		}
		else
		{
			mgRead( MainAlpha, MainSkin.width*MainSkin.height, fp );
			mgClose(fp);
		}
	}


	inline ~mgSkinDlg(void)
	{
		uint i;
		for( i=0; i<Bitmaps.size(); i++ )
			RELEASE( Bitmaps.at(i)->data );
			
		RELEASE( MainAlpha );
		RELEASE( MainSkin.data );
		RELEASE( Background.data );
		RELEASE( Temp.data );
	}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

	inline mgSkinBitmapP AddBitmap( char* name, uint mode, uint32 colorkey )
	{
		mgSkinBitmap B;
		mgTexture T;
		T.LoadSwitch( name, mode );
		T.ToRGBA();		//32 bits for every skin
		B.width = T.Width;
		B.height = T.Height;
		B.data = (uint32*)T.Data;
		B.colorkey = colorkey;
		Bitmaps.add(B);
		return Bitmaps.back();
	}


	inline void EraseBitmap( mgSkinBitmapP hbitmap )		//automatically erases all the buttons skinned with this bitmap
	{
		for( int i=Bitmaps.size()-1; i>=0; i-- )
		{
			if( hbitmap == Bitmaps.at(i) )
			{
				for( int j=Buttons.size()-1; j>=0; j-- )
					if( (Buttons.at(j)->off == hbitmap) || (Buttons.at(j)->on == hbitmap) )
						Buttons.erase(j);
				RELEASE( Bitmaps.at(i)->data );
				Bitmaps.erase(i);
				return;
			}
		}
	}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
	
	inline mgSkinButtonP AddButton( mgSkinBitmapP hbmp_off, mgSkinBitmapP hbmp_on, sint posx, sint posy, uint state ) //for simplicity, the mouseover skin is the bmp_on skin
	{
		mgSkinButton B;
		if( hbmp_off )				//if one of the skins doesn't exist but is selected, it just won't be displayed (no error)
			B.off = hbmp_off;
		if( hbmp_on )
			B.on = hbmp_on;
		B.posx = posx;
		B.posy = posy;
		B.SetState(state);
		B.active = true;
		B.visible = true;
		Buttons.add(B);
		return Buttons.back();
	}
	
	inline void EraseButton( mgSkinButtonP hbutton )
	{
		for( int i=Buttons.size()-1; i>=0; i-- )
		{
			if( hbutton == Buttons.at(i) )
			{
				Buttons.erase(i);
				return;
			}
		}
	}
	
//-------------------------------------------------------------------
//-------------------------------------------------------------------

	inline void GetBackground(void)
	{
		BITMAPINFO* pbitmapinfo;
		HDC dcsrc;
		HDC dcdest;
		HBITMAP bitmap;

		RECT rect;
		HWND hwnd = GetDesktopWindow();
		GetWindowRect ( hwnd, &rect );
		Background.width = rect.right-rect.left;
		Background.height = rect.bottom-rect.top;
		dcsrc = GetDC ( hwnd );

		bitmap = CreateCompatibleBitmap ( dcsrc, Background.width, Background.height )	;
		dcdest = CreateCompatibleDC ( dcsrc )					;
		
		if((pbitmapinfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER)))!=NULL)
		{
			pbitmapinfo->bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
			pbitmapinfo->bmiHeader.biWidth         = Background.width;
			pbitmapinfo->bmiHeader.biHeight        = -Background.height;
			pbitmapinfo->bmiHeader.biPlanes        = 1;
			pbitmapinfo->bmiHeader.biBitCount      = 32;
			pbitmapinfo->bmiHeader.biCompression   = BI_RGB;
			pbitmapinfo->bmiHeader.biSizeImage     = 0;
			pbitmapinfo->bmiHeader.biXPelsPerMeter = 0;
			pbitmapinfo->bmiHeader.biYPelsPerMeter = 0;
			pbitmapinfo->bmiHeader.biClrUsed       = 0;
			pbitmapinfo->bmiHeader.biClrImportant  = 0;
			
			SelectObject(dcdest, bitmap);

			
			RELEASE( Background.data );
			if( (Background.data = (uint32*)malloc(Background.width*Background.height*4)) != NULL )
			{
				StretchBlt(	dcdest, 0, 0, Background.width, Background.height, dcsrc, 0, 0, Background.width, Background.height, SRCCOPY ); 
				GetDIBits( dcdest, bitmap, 0, Background.height, Background.data, pbitmapinfo, DIB_RGB_COLORS );
			}
			RELEASE(pbitmapinfo);
		}
		
		DeleteDC ( dcdest );
		DeleteObject ( bitmap );
		ReleaseDC ( hwnd, dcsrc );
	}


//-------------------------------------------------------------------
//-------------------------------------------------------------------

	inline void Paint( HDC dc, int xpos, int ypos )
	{
		//--------------------------------------------------------------
		//prints the background
		//--------------------------------------------------------------
		memcpy( Temp.data, MainSkin.data, MainSkin.width*MainSkin.height*4 );
		
		
		//--------------------------------------------------------------
		//prints the buttons
		//--------------------------------------------------------------
		mgSkinBitmapP bitmap;
		mgSkinButtonP button;
		uint32 *src, *dst;

		for( uint i=0; i<Buttons.size(); i++ )
		{
			button = Buttons.at(i);
			if( button->visible )
			{
				if( button->GetState() == MG_SKIN_OFF )
					bitmap = button->off;
				else
					bitmap = button->on;

				if( !bitmap ) continue;
				src = bitmap->data;
				dst = Temp.data + button->posy*Temp.width + button->posx;

				for( int j=0; j<bitmap->height; j++ )
				{
					for( int k=0; k<bitmap->width; k++, dst++, src++ )
					{
						if( !(bitmap->colorkey>>24) || *src != bitmap->colorkey )
							*dst = *src;
					}
					dst = dst + Temp.width - bitmap->width;
				}
			}
		}
	
		
		//--------------------------------------------------------------
		//mixes the background with the skinned interface
		//--------------------------------------------------------------
		//clipping
		sint xstart = xpos;					//will be clipped coords of the window on the screen
		sint ystart = ypos;
		sint xend = xpos + MainSkin.width;	//will be clipped boundaries of the window on the screen
		sint yend = ypos + MainSkin.height;
		sint depx = 0, depy = 0;					//will be the coords from where to draw on the skin
		
		if( xstart < 0 ) { depx = -xstart;	xstart = 0; }
		if( xstart >= (sint)Background.width )	return	;
		if( ystart < 0 ) { depy = -ystart; ystart = 0; }
		if( ystart >= (sint)Background.height ) return;
		if( xend <= 0 )	return;
		if( xend > (sint)Background.width ) xend = Background.width;
		if( yend <= 0 )	return ;
		if( yend > (sint)Background.height ) yend = Background.height;
		
		uint32* ptrSrc = &Background.data[ ystart*Background.width + xstart ];
		uint32* ptrDest = &Temp.data[ depy*MainSkin.width + depx ];
		uint8* ptrMask = &MainAlpha[ depy*MainSkin.width + depx ];
		
		//completes Temp with the blended Background
		sint delta = xend - xstart;
		for( int a=0; a<yend-ystart; a++ )
		{
			uint32* ptrdst = ptrDest;
			uint32* ptrsrc = ptrSrc;
			uint8*	ptrmsk = ptrMask;
			for(int i=0; i<delta; i++, ptrdst++, ptrsrc++, ptrmsk++)
			{
				uint8 alpha1 = *ptrmsk;
				if( alpha1 )
				{
					uint8 alpha2 = 255-alpha1;
					uint r = ( alpha2*(((*ptrsrc)&0x00ff0000)>>16) + alpha1*(((*ptrdst)&0x00ff0000)>>16) ) >> 8;
					uint g = ( alpha2*(((*ptrsrc)&0x0000ff00)>>8) + alpha1*(((*ptrdst)&0x0000ff00)>>8)) >> 8;
					uint b = ( alpha2*((*ptrsrc)&0x000000ff) + alpha1*((*ptrdst)&0x000000ff) ) >> 8;
					*ptrdst = (b<<16) | (g<<8) | r;
				}
				else
					*ptrdst = *ptrsrc;
			}
			ptrSrc += Background.width;
			ptrDest += MainSkin.width;
			ptrMask	+= MainSkin.width;
		}
		
		
		//--------------------------------------------------------------
		//prints the resulting bitmap on the screen
		//--------------------------------------------------------------
		BITMAPINFO pbitmap;
		pbitmap.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		pbitmap.bmiHeader.biWidth = MainSkin.width;
		pbitmap.bmiHeader.biHeight = -MainSkin.height;
		pbitmap.bmiHeader.biPlanes = 1;
		pbitmap.bmiHeader.biBitCount = 32;
		pbitmap.bmiHeader.biCompression = BI_RGB;
		pbitmap.bmiHeader.biSizeImage = 0;
		pbitmap.bmiHeader.biXPelsPerMeter = 0;
		pbitmap.bmiHeader.biYPelsPerMeter = 0;
		pbitmap.bmiHeader.biClrUsed = 0;
		pbitmap.bmiHeader.biClrImportant = 0;
		SetDIBitsToDevice ( dc, 0, 0, Temp.width, Temp.height, 0, 0, 0, Temp.height, Temp.data, &pbitmap, DIB_RGB_COLORS );
	}
	
//-------------------------------------------------------------------
//-------------------------------------------------------------------
#define MG_SKIN_INSIDE	((mgSkinButtonP)1)
#define MG_SKIN_OUTSIDE	((mgSkinButtonP)0)
	
	inline mgSkinButtonP Locate( sint posx, sint posy )	//those are coords relative to the skinned window
	{
		mgSkinButtonP T;
		mgSkinBitmapP B;
		for( uint i=0; i<Buttons.size(); i++ )
		{
			T = Buttons.at(i);
			if( (T->GetState() != MG_SKIN_OFF) || !T->off )
				B = T->on;
			else
				B = T->off;
			if( !(B && T->active && T->visible )) continue;
			if( T->posx <= posx &&  posx < (T->posx + B->width) && T->posy <= posy &&  posy < (T->posy + B->height) )	//we're into this button
				if( B->data[(posy-T->posy)*B->width+(posx-T->posx)] != B->colorkey )	//we're into the efficient part of the button
					return T;
		}
		
		if( MainAlpha[posy*MainSkin.width+posx] )
			return MG_SKIN_INSIDE;
		
		return MG_SKIN_OUTSIDE;	//definitely outside
	}
	
//-------------------------------------------------------------------
//-------------------------------------------------------------------

	inline void ResetStates(void)
	{
		for( uint i=0; i<Buttons.size(); i++ )
			if( Buttons.at(i)->GetState() == MG_SKIN_OVER )
				Buttons.at(i)->SetState(MG_SKIN_OFF);
		Redraw = true;
	}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

};
















//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//THE MESSAGE LOOP
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------

//bool mgSetupResult = false;
mgSkinDlg* mgSDlg = NULL;
mgSkinButtonP butquit, butwin, butfull, butstart;


BOOL CALLBACK mgSkinSetupDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC dc = GetDC( hWnd );
	RECT winrect; GetWindowRect( hWnd, &winrect );
	static bool firstpaint = true;;
	mgSkinButtonP Test;


	//paint if forced
	
	if( mgSDlg && mgSDlg->Redraw )
		mgSDlg->Paint( dc, winrect.left, winrect.top );


	switch(uMsg)
	{
	case WM_INITDIALOG:
		SetWindowText( hWnd, "Mutant Setup" );
		mgSDlg = new mgSkinDlg( launcher_back_, launcher_mask_, MG_DATA_DEFAULT );
		butquit =	mgSDlg->AddButton( NULL, mgSDlg->AddBitmap( launcher_quit_, MG_DATA_DEFAULT, PURPLE ), 197, 35, MG_SKIN_OFF );
		butwin =	mgSDlg->AddButton( NULL, mgSDlg->AddBitmap( launcher_win_, MG_DATA_DEFAULT, PURPLE ), 70, 188, mgFullScreen?MG_SKIN_OFF:MG_SKIN_ON );
		butfull =	mgSDlg->AddButton( NULL, mgSDlg->AddBitmap( launcher_full_, MG_DATA_DEFAULT, PURPLE ), 149, 176, mgFullScreen?MG_SKIN_ON:MG_SKIN_OFF );
		butstart =	mgSDlg->AddButton( NULL, mgSDlg->AddBitmap( launcher_start_, MG_DATA_DEFAULT, PURPLE ), 86, 258, MG_SKIN_OFF );
		GetClientRect( GetDesktopWindow(), &winrect );
		SetWindowPos( hWnd, HWND_TOPMOST, (winrect.right-mgSDlg->MainSkin.width)/2, (winrect.bottom-mgSDlg->MainSkin.height)/2, mgSDlg->MainSkin.width, mgSDlg->MainSkin.height, SWP_SHOWWINDOW );
		return TRUE;
		break;
		
		
	case WM_ACTIVATE:
		if( wParam==WA_INACTIVE )
		{
			firstpaint = true;
			ShowWindow( hWnd, SW_MINIMIZE );
		}
		else
		{
			if( firstpaint )
			{
				firstpaint = false;
				mgSDlg->GetBackground();
			}
			ShowWindow( hWnd, SW_SHOWNORMAL );
		}
		return 0;
		break;


	case WM_ERASEBKGND:
		GetWindowRect( hWnd, &winrect );
		mgSDlg->Paint( (HDC)wParam, winrect.left, winrect.top );
		return true;
		break;


	case WM_PAINT:		//very important to avoid flickering !!!
		return true;	//prevents the window from being displayed during a very short time on a new position with its previous background
		break;
	
	
	case WM_MOUSEMOVE:
		if ( wParam == MK_LBUTTON)
			if( mgSDlg->Locate( LOWORD(lParam), HIWORD(lParam) ) != MG_SKIN_OUTSIDE )
				SendMessage( hWnd, WM_SYSCOMMAND, SC_MOVE | HTCLIENT, 0 );

		Test = mgSDlg->Locate( LOWORD(lParam), HIWORD(lParam));
		if( (Test == MG_SKIN_INSIDE) || (Test == MG_SKIN_OUTSIDE) )
		{
			mgSDlg->ResetStates();
			return 0;
		}

		if( Test  == butwin)
			butwin->SetState( MG_SKIN_OVER );
		else if( Test == butfull )
			butfull->SetState( MG_SKIN_OVER );
		else if( Test == butquit )
			butquit->SetState( MG_SKIN_OVER );
		else if( Test == butstart )
			butstart->SetState( MG_SKIN_OVER );

		mgSDlg->Redraw = true;
		return 0;
		break;

	
	case WM_LBUTTONDOWN:
		Test = mgSDlg->Locate( LOWORD(lParam), HIWORD(lParam));

		if( Test  == butwin)
		{
			butwin->SetState( MG_SKIN_ON );
			butfull->SetState( MG_SKIN_OFF );
			mgFullScreen = false;
		}
		else if( Test == butfull )
		{
			butwin->SetState( MG_SKIN_OFF );
			butfull->SetState( MG_SKIN_ON );
			mgFullScreen = true;
		}
		else if( Test == butquit )
		{
			//mgSetupResult = false;
			//DestroyWindow( hWnd );
			EndDialog( hWnd, false );
		}
		else if( Test == butstart )
		{
			//mgSetupResult = true;
			//DestroyWindow( hWnd );
			EndDialog( hWnd, true );
		}

		mgSDlg->Redraw = true;
		return 0;
		break;
		

	}	//end of WM switch

	return false;	//false to let DefWindowProc do the rest.
}






int mgSkinSetupDlg(HINSTANCE hInstance)
{
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_SKINSETUP), NULL, mgSkinSetupDlgProc );
	//return mgSetupResult;
}



















//END
#endif