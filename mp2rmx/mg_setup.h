//-------------------------------------------------------------------
//MG_SETUP.H - LAUNCHER INERFACE
//-------------------------------------------------------------------
#ifndef _MG_SETUP_H_
#define _MG_SETUP_H_



BOOL CALLBACK mgSetupDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		if(mgFullScreen)
			SendDlgItemMessage( hWnd,IDC_SETUP_FULLSCREEN, BM_SETCHECK, BST_CHECKED,0 );
		
		SendMessage( hWnd, WM_SETICON, ICON_SMALL, (long)mgIcon );		//only if the titlebar + sysmenu are present
		SendMessage( hWnd, WM_SETTEXT, 0, (long)"Mutant Setup");		//SetWindowText( hWnd, "Mutant Setup" );
		return true;
		break;
		
		
		
	case WM_COMMAND:
		{
			switch(wParam)
			{
			case MAKEINTRESOURCE(ID_SETUP_OK):
				EndDialog( hWnd, true );
				return true;
				break;			
				
			case MAKEINTRESOURCE(ID_SETUP_EXIT):
				EndDialog( hWnd, false );
				return true;
				break;
				
			case MAKEINTRESOURCE(IDC_SETUP_FULLSCREEN):
				mgFullScreen = !mgFullScreen;
				return true;
				break;
				
			}
			break;
		}
		
		
		
		
	case WM_MOUSEMOVE:
		if ( wParam == MK_LBUTTON)
		{
			SendMessage( hWnd, WM_SYSCOMMAND, SC_MOVE | HTCLIENT, 0 );
			return true;
		}
		break;
		
		
		
	}	//end of WM switch
	
	return false;	//false to let DefWindowProc do the rest.
}







int mgSetupDlg(HINSTANCE hInstance)
{
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_SETUP), NULL, mgSetupDlgProc );
}






#endif