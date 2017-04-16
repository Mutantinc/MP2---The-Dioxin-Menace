//--------------------------------------------------------------------
// DEMO_ADD.H - LES SETTINGS ET CONSTANTES LES PLUS GENERAUX DE LA DEMO
//--------------------------------------------------------------------
#ifndef _DEMOPLUS_H_
#define _DEMOPLUS_H_

//--------------------------------------------------------------------
// settings super g�n�raux
//--------------------------------------------------------------------
DWORD width=640;		//largeur de l'�cran
DWORD pitch;			//largeur veritable de l'ecran en memoire video
DWORD height=480;		//hauteur de l'�cran
DWORD depth=32;			//profondeur de l'�cran
BOOL fullscreen=true;	//plein �cran?
LPGUID video=NULL;



const double pi=3.1415926535897932384626433832795;	//faut pas abuser quand m�me!

const DWORD noir=0;
const DWORD rouge=0x00FF0000;
const DWORD vert=0x0000FF00;
const DWORD bleu=0x000000FF;
const DWORD jaune=0x00FFFF00;
const DWORD violet=0x00FF00FF;
const DWORD cyan=0x0000FFFF;
const DWORD blanc=0x00FFFFFF;

//-------------------------------------------------------


typedef struct effst
{
	DWORD tm;
	int ord;
	int row;
	void (*fonc)();
} eff;



char* int2str(int i, char* buf)
{
		sprintf(buf,"%d",i);
		return buf;
}



int str2int(char* buf)
{
		int i;
		sscanf( buf, "%d", &i );
		return i;
}

#endif