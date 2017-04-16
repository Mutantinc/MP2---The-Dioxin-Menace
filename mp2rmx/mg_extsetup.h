//-------------------------------------------------------------------
//MG_EXTSETUP.H - POUR LES PARAMETRES DE DEMARRAGE
//-------------------------------------------------------------------
#ifndef _MG_EXTSETUP_H_
#define _MG_EXTSETUP_H_



bool mgExtSetupResult;




static BOOL CALLBACK mgExtSetupDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	switch(uMsg)
	{
	case WM_INITDIALOG:
		SendMessage( hWnd, WM_SETICON, ICON_SMALL, (long)mgIcon );
		return true;
		break;
		
		
		
	case WM_SYSCOMMAND:
		switch(wParam)
		{
		case SC_MINIMIZE:
			ShowWindow( hWnd, SW_MINIMIZE );
			return true;
			break;
			
			
		case SC_CLOSE:
			SendMessage(hWnd, WM_COMMAND, ID_EXT_CANCEL, NULL);
			break;
			
			
		} //fin du switch de WM_SYSCOMMAND
		
		
		
		case WM_COMMAND:
			{
				switch(wParam)
				{
				case MAKEINTRESOURCE(ID_EXT_OK):
					mgExtSetupResult = true;
					DestroyWindow(hWnd);
					hWnd=0;
					return true;
					break;			
					
				case MAKEINTRESOURCE(ID_EXT_CANCEL):
					mgExtSetupResult = false;
					DestroyWindow(hWnd);
					hWnd=0;
					return true;
					break;
					
				}
				break;
			}
			
			
			
			
	}	//fin du switch WM_COMMAND
	
	
	// Let the Default Proc manage the Window
	//return DefWindowProc(hWnd,uMsg,wParam,lParam);
	return false;
}







mgExtSetupDlg( HINSTANCE hInstance )
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_EXTSETUP), NULL, mgExtSetupDlgProc );
	return mgExtSetupResult;
}






#endif