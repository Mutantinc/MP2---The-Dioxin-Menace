//-------------------------------------------------------------------
//EFFETS.H - TOUS LES EFFETS DE LA DEMO SONT LA
//-------------------------------------------------------------------
#ifndef _EFFETS_H_
#define _EFFETS_H_

#include "gfxloaders.h"
#include "tempo.h"


//--------------------------------------------------------------------
// PREPARATION GENERALE DES EFFETS
//--------------------------------------------------------------------

	//les variables portant sur le graphe et la musique
int frame=0, noeffet=0;
DWORD basetime, efftm=0, thistm=MAXINT;	//pour une gestion par le temps; MAXINT est là por donner le temps du premier effet
int row_av=0, row_ap=0, row_ts=-1, ord_ap=0, ord_ts=-1;				//pour une gestion par la synchro musique
eff effets[];




//--------------------------------------------------------------------
// LIAISONS EXTERNES AVEC LES INTERFACES DX & FMOD
//--------------------------------------------------------------------

//extern LPDIRECTDRAW7 lpDD;
extern LPDIRECTDRAWSURFACE7 lpDDSBack;
extern LPDIRECTDRAWSURFACE7 lpDDSTemp;

#ifdef USE_D3D
	extern LPDIRECT3DDEVICE7	lpD3DDevice;
#endif


#ifdef USE_FMUSIC
	#ifdef USE_MINIFMOD
		signed char		FMUSIC_PlaySong(FMUSIC_MODULE *mod);
	#else
		DLL_API signed char		F_API FMUSIC_PlaySong(FMUSIC_MODULE *mod);
	#endif

	extern FMUSIC_MODULE *mod;
#endif




	
//--------------------------------------------------------------------
// GOTOEFF - pour changer d'effet en cours de route
//--------------------------------------------------------------------

	//la fonction pour démarrer directmeent à un certain effet avec la musique synchronisée sur cet effet (nécessite Fmod Api pour y arriver)
	//nb doit etre le numero d'un effet d'init
#ifdef USE_FMOD
	void gotoeff(int nb)
	{
		thistm=0;	//par précaution
		noeffet=nb-1;
		ord_ts=effets[noeffet].ord;
		row_ts=effets[noeffet].row;
		if (row_ts != -1 && ord_ts != -1)
		{
			row_ap=-1;
			FMUSIC_SetMasterVolume(mod,0);
			FMUSIC_SetOrder(mod,ord_ts);

			while( FMUSIC_GetOrder(mod) != ord_ts ) {}	//FMod trop lent à raffraichir ses variables de daube

			while( FMUSIC_GetRow(mod) < row_ts ) {}
			FMUSIC_SetMasterVolume(mod,255);
		}
	}
#endif
	
	
	
	
	
	
//---------------------------------------------------------------------------------------------
// POINTEURS SUR TOUS LES GRAPHS
//---------------------------------------------------------------------------------------------

unsigned int blob;

//D3DVERTEX vrect[4];

LPDIRECTDRAWSURFACE7 intro01_01;
LPDIRECTDRAWSURFACE7 intro01_02;
LPDIRECTDRAWSURFACE7 intro02_01;
LPDIRECTDRAWSURFACE7 intro02_02;
LPDIRECTDRAWSURFACE7 intro03_01;
LPDIRECTDRAWSURFACE7 intro03_02;
LPDIRECTDRAWSURFACE7 t0;
LPDIRECTDRAWSURFACE7 t1;
LPDIRECTDRAWSURFACE7 t2;
LPDIRECTDRAWSURFACE7 t2_1;
LPDIRECTDRAWSURFACE7 t2_2;
LPDIRECTDRAWSURFACE7 no_3d;

LPDIRECTDRAWSURFACE7 ti00;
LPDIRECTDRAWSURFACE7 m;
LPDIRECTDRAWSURFACE7 p;
LPDIRECTDRAWSURFACE7 deux;
LPDIRECTDRAWSURFACE7 mechant;
LPDIRECTDRAWSURFACE7 mutant;
LPDIRECTDRAWSURFACE7 pouletz;
LPDIRECTDRAWSURFACE7 project;

LPDIRECTDRAWSURFACE7 bip01;
LPDIRECTDRAWSURFACE7 bip02;
LPDIRECTDRAWSURFACE7 bip03;
LPDIRECTDRAWSURFACE7 eclat1;
LPDIRECTDRAWSURFACE7 eclat2;
LPDIRECTDRAWSURFACE7 eclat3;
LPDIRECTDRAWSURFACE7 eclat4;
LPDIRECTDRAWSURFACE7 eclat5;
LPDIRECTDRAWSURFACE7 grenade;
LPDIRECTDRAWSURFACE7 mg01;
LPDIRECTDRAWSURFACE7 pling00;
LPDIRECTDRAWSURFACE7 pling01;
LPDIRECTDRAWSURFACE7 pling02;
LPDIRECTDRAWSURFACE7 tele1;
LPDIRECTDRAWSURFACE7 tele2;
//LPDIRECTDRAWSURFACE7 star;

LPDIRECTDRAWSURFACE7 villefond;
LPDIRECTDRAWSURFACE7 champ;
LPDIRECTDRAWSURFACE7 flotte;
LPDIRECTDRAWSURFACE7 fumee1;
LPDIRECTDRAWSURFACE7 fumee2;
LPDIRECTDRAWSURFACE7 grosyeux;
LPDIRECTDRAWSURFACE7 tracteur;
LPDIRECTDRAWSURFACE7 comm;
LPDIRECTDRAWSURFACE7 debsoucoupe;
LPDIRECTDRAWSURFACE7 debville;
LPDIRECTDRAWSURFACE7 soldier;
//LPDIRECTDRAWSURFACE7 lumiere;
LPDIRECTDRAWSURFACE7 ombre;

LPDIRECTDRAWSURFACE7 mutation;
LPDIRECTDRAWSURFACE7 splif21;
LPDIRECTDRAWSURFACE7 splif22;

LPDIRECTDRAWSURFACE7 splif11;
LPDIRECTDRAWSURFACE7 splif12;
LPDIRECTDRAWSURFACE7 fear;

LPDIRECTDRAWSURFACE7 creddan;
LPDIRECTDRAWSURFACE7 credrez;
LPDIRECTDRAWSURFACE7 credmars;
LPDIRECTDRAWSURFACE7 credfond;
LPDIRECTDRAWSURFACE7 credits;

LPDIRECTDRAWSURFACE7 fight01;
LPDIRECTDRAWSURFACE7 fight02;
LPDIRECTDRAWSURFACE7 fight03;
LPDIRECTDRAWSURFACE7 netoile;
LPDIRECTDRAWSURFACE7 spirale;
LPDIRECTDRAWSURFACE7 trbleus;

LPDIRECTDRAWSURFACE7 reveal;
LPDIRECTDRAWSURFACE7 revealfond;
LPDIRECTDRAWSURFACE7 toostrong;
LPDIRECTDRAWSURFACE7 upw;
LPDIRECTDRAWSURFACE7 upwfond;
LPDIRECTDRAWSURFACE7 dontresist;

LPDIRECTDRAWSURFACE7 guests;
LPDIRECTDRAWSURFACE7 gacetone;
LPDIRECTDRAWSURFACE7 gak;
LPDIRECTDRAWSURFACE7 gfond;
LPDIRECTDRAWSURFACE7 gsplif;

LPDIRECTDRAWSURFACE7 brume01;
LPDIRECTDRAWSURFACE7 brume02;
LPDIRECTDRAWSURFACE7 brume03;
LPDIRECTDRAWSURFACE7 brume04;
LPDIRECTDRAWSURFACE7 brume05;
LPDIRECTDRAWSURFACE7 mutachat;
LPDIRECTDRAWSURFACE7 mutapig;
LPDIRECTDRAWSURFACE7 ruelle03;
LPDIRECTDRAWSURFACE7 strciel;
LPDIRECTDRAWSURFACE7 strcocha;
LPDIRECTDRAWSURFACE7 streyes;
LPDIRECTDRAWSURFACE7 strmurs;
LPDIRECTDRAWSURFACE7 strstreum;
LPDIRECTDRAWSURFACE7 strtoits;
LPDIRECTDRAWSURFACE7 started;

LPDIRECTDRAWSURFACE7 city;
LPDIRECTDRAWSURFACE7 master;

LPDIRECTDRAWSURFACE7 ge1;
LPDIRECTDRAWSURFACE7 ge2;
LPDIRECTDRAWSURFACE7 ge3;
LPDIRECTDRAWSURFACE7 ge4;
LPDIRECTDRAWSURFACE7 ge5;
LPDIRECTDRAWSURFACE7 ge6;
LPDIRECTDRAWSURFACE7 ge7;
LPDIRECTDRAWSURFACE7 ge8;
LPDIRECTDRAWSURFACE7 gefra;
LPDIRECTDRAWSURFACE7 genefond;

LPDIRECTDRAWSURFACE7 main1;
LPDIRECTDRAWSURFACE7 main2;
LPDIRECTDRAWSURFACE7 gniii_tete;

LPDIRECTDRAWSURFACE7 birthfond;
LPDIRECTDRAWSURFACE7 booh;
LPDIRECTDRAWSURFACE7 frafond;
LPDIRECTDRAWSURFACE7 fralayer;
LPDIRECTDRAWSURFACE7 gruh1;
LPDIRECTDRAWSURFACE7 gruh2;
LPDIRECTDRAWSURFACE7 gfra;
LPDIRECTDRAWSURFACE7 hufond1;
LPDIRECTDRAWSURFACE7 hufond2;
LPDIRECTDRAWSURFACE7 hufond3;
LPDIRECTDRAWSURFACE7 handsup;
LPDIRECTDRAWSURFACE7 handsup2;
LPDIRECTDRAWSURFACE7 pouletacet1;
LPDIRECTDRAWSURFACE7 tetonne;
LPDIRECTDRAWSURFACE7 tetonnefond;
LPDIRECTDRAWSURFACE7 tetonnetexte;
LPDIRECTDRAWSURFACE7 timast;
LPDIRECTDRAWSURFACE7 sheepdef;
LPDIRECTDRAWSURFACE7 vs;
LPDIRECTDRAWSURFACE7 evilpoulet;
LPDIRECTDRAWSURFACE7 combo;
LPDIRECTDRAWSURFACE7 ko;
LPDIRECTDRAWSURFACE7 mutation2;
LPDIRECTDRAWSURFACE7 hasnow;
LPDIRECTDRAWSURFACE7 dot;

LPDIRECTDRAWSURFACE7 atomica;
LPDIRECTDRAWSURFACE7 atomicb;
LPDIRECTDRAWSURFACE7 trash1;
LPDIRECTDRAWSURFACE7 trash1b;
LPDIRECTDRAWSURFACE7 trash2;
LPDIRECTDRAWSURFACE7 trash2b;
LPDIRECTDRAWSURFACE7 trash3;
LPDIRECTDRAWSURFACE7 trash4;
LPDIRECTDRAWSURFACE7 trash4b;
LPDIRECTDRAWSURFACE7 trash5;
LPDIRECTDRAWSURFACE7 trash5b;
LPDIRECTDRAWSURFACE7 trash6a;
LPDIRECTDRAWSURFACE7 trash6b;
LPDIRECTDRAWSURFACE7 trash6c;
LPDIRECTDRAWSURFACE7 trash7a;
LPDIRECTDRAWSURFACE7 trash7b;
LPDIRECTDRAWSURFACE7 trash7c;
LPDIRECTDRAWSURFACE7 trash8a;
LPDIRECTDRAWSURFACE7 trash8b;
LPDIRECTDRAWSURFACE7 trash8c;
LPDIRECTDRAWSURFACE7 trash9;
LPDIRECTDRAWSURFACE7 trash9b;
LPDIRECTDRAWSURFACE7 trash10a;
LPDIRECTDRAWSURFACE7 trash10b;
LPDIRECTDRAWSURFACE7 trash10c;
LPDIRECTDRAWSURFACE7 trash11a;
LPDIRECTDRAWSURFACE7 trash11b;
LPDIRECTDRAWSURFACE7 trash12a;
LPDIRECTDRAWSURFACE7 trash12b;
LPDIRECTDRAWSURFACE7 trash13a;
LPDIRECTDRAWSURFACE7 trash13b;

LPDIRECTDRAWSURFACE7 mandarine;
LPDIRECTDRAWSURFACE7 orion;
LPDIRECTDRAWSURFACE7 move;
LPDIRECTDRAWSURFACE7 dim4;
LPDIRECTDRAWSURFACE7 ojuice;
LPDIRECTDRAWSURFACE7 eclipse;
LPDIRECTDRAWSURFACE7 tpolm;
LPDIRECTDRAWSURFACE7 digitalmurder;
LPDIRECTDRAWSURFACE7 popsyteam;
LPDIRECTDRAWSURFACE7 bomb;
LPDIRECTDRAWSURFACE7 purple;
LPDIRECTDRAWSURFACE7 equinox;
LPDIRECTDRAWSURFACE7 etron;
LPDIRECTDRAWSURFACE7 threestate;
LPDIRECTDRAWSURFACE7 pixelfr;
LPDIRECTDRAWSURFACE7 condense;
LPDIRECTDRAWSURFACE7 calodox;

LPDIRECTDRAWSURFACE7 gni_fond;
LPDIRECTDRAWSURFACE7 explo_tete;
LPDIRECTDRAWSURFACE7 explo_fond;


//-------------------------------------------------------------------
//EFFETS D'INIT ET UNINIT
//-------------------------------------------------------------------
void eff_onetimestart(void)
{
	cls(lpDDSBack,noir);	//laisser toujours ici

	
	
	blob=blobopen(blob_name);	//le blob des graphes

/*
	vrect[0] = D3DVERTEX(D3DVECTOR(0,1,0),D3DVECTOR(0,0,-1),0,1);
	vrect[1] = D3DVERTEX(D3DVECTOR(1,1,0),D3DVECTOR(0,0,-1),1,1);
	vrect[2] = D3DVERTEX(D3DVECTOR(0,0,0),D3DVECTOR(0,0,-1),0,0);
	vrect[3] = D3DVERTEX(D3DVECTOR(1,0,0),D3DVECTOR(0,0,-1),1,0);
*/

	thistm=0;	//pour changer d'effet
}


void eff_incipit(void)	//fonction d'attente
{
	cls(lpDDSBack,noir);
}

void eff_onetimestop(void)
{
	blobclose(blob);
	thistm=0;
}









//---------------------------------------------------------------------------------------------
//LE SCHBOING ELASTIQUE DU DEBUT
//LES SCROLLINGS
//LE GYROPHARE
//---------------------------------------------------------------------------------------------

void e01_OPEN(void)
{
	intro01_01=LdMemPCX(lpDD, blob, 0);
	intro01_02=LdMemPCX(lpDD, blob, 1);
	intro02_01=LdMemPCX(lpDD, blob, 2);
	intro02_02=LdMemPCX(lpDD, blob, 3);
	intro03_01=LdMemPCX(lpDD, blob, 4);
	intro03_02=LdMemPCX(lpDD, blob, 5);
	t0=LdMemPCX(lpDD, blob, 6);
	t1=LdMemPCX(lpDD, blob, 7);
	t2=LdMemPCX(lpDD, blob, 8);
	t2_1=LdMemPCX(lpDD, blob, 9);
	t2_2=LdMemPCX(lpDD, blob, 10);
	no_3d=LdMemPCX(lpDD, blob, 11);

	
	#ifdef USE_FMUSIC
		FMUSIC_PlaySong(mod);
	#endif


	#ifdef USE_TEMPO
		#ifdef USE_FMOD
			gotoeff(SetTempo(effets));
		#endif
	#endif

	
	thistm=0;
}




void e0101(void)
{
	cls(lpDDSTemp,noir);
	int w=(int)(640*((float)efftm/(float)thistm)*(0.7+0.3*(1-sin((3*pi*efftm)/thistm))));
	int h=(int)(360*((float)efftm/(float)thistm)*(0.7+0.3*(cos((2*pi*efftm)/thistm))));
	dIo2(lpDDSTemp,(width-w)/2,(height-h)/2,w,h,intro01_02,0,0,640,360,(255*efftm)/thistm);
	if(rand()>16000)
		dIo2(lpDDSTemp,(width-w)/2,(height-h)/2,w,h,intro01_01,0,0,640,360,(80*efftm)/thistm);

	dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}



//le premier scrolling
void e0102(void)
{
	cls(lpDDSTemp,noir);

	dI(lpDDSTemp,0-(546*efftm)/thistm,60,NULL,NULL,intro01_02,NULL,NULL,NULL,NULL);
	if(rand()>16000)
		dIo1(lpDDSTemp,0-(546*efftm)/thistm,60,intro01_01,80);


	unsigned int inttm1=(thistm/2)-1500, inttm2=(thistm/2)+1500;	//effet SW dure 3000
	const wf=100;	//largeur finale

	if(efftm>inttm1 && efftm<inttm2)
	{
		int w=640-((640-wf)*(efftm-inttm1))/3000;
		int h=(118*w)/640;
		dIo2t(lpDDSTemp,(width-w)/2,(height-h)/2,w,h,t0,NULL,NULL,NULL,NULL,255-(255*(efftm-inttm1))/3000,noir);

		int flash=255-(255*(efftm-inttm1))/((inttm2-inttm1)/3);
		if (flash<0) flash=0;
		clsorect(lpDDSTemp,0,60,640,360,flash,blanc);
	}
	

	if(efftm>thistm-500)
		dIf(lpDDSBack,0,0,lpDDSTemp,255-(255*(efftm-thistm+500))/500);
	else
		dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}







//le second scrolling
void e0122(void)
{
	cls(lpDDSTemp,noir);

	dI(lpDDSTemp,0-(181*efftm)/thistm,60,NULL,NULL,intro02_02,NULL,NULL,NULL,NULL);
	if(rand()>16000)
		dIo1(lpDDSTemp,0-(181*efftm)/thistm,60,intro02_01,80);


	unsigned int inttm1=(thistm/2)-1500, inttm2=(thistm/2)+1500;	//effet SW dure 3000
	const wf=80;	//largeur finale

	if(efftm>inttm1 && efftm<inttm2)
	{
		int w=640-((640-wf)*(efftm-inttm1))/3000;
		int h=(166*w)/640;
		dIo2t(lpDDSTemp,(width-w)/2,(height-h)/2,w,h,t1,NULL,NULL,NULL,NULL,255-(255*(efftm-inttm1))/3000,noir);

		int flash=255-(255*(efftm-inttm1))/((inttm2-inttm1)/3);
		if (flash<0) flash=0;
		clsorect(lpDDSTemp,0,60,640,360,flash,blanc);
	}
	
	if(efftm<500)
		dIf(lpDDSBack,0,0,lpDDSTemp,(255*efftm)/500);
	else
		dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}



//le shift rapide vers la gauche
void e0125(void)
{
	cls(lpDDSBack,noir);
	int y=efftm%4-2;
	dI(lpDDSBack,0,60+y,NULL,NULL,intro03_01,NULL,NULL,NULL,NULL);


	dIt(lpDDSBack,200,400,NULL,NULL,t2,NULL,NULL,NULL,NULL,noir);
	if((rand()%100 +efftm)%200>100)
		dIt(lpDDSBack,488,400,NULL,NULL,t2_2,NULL,NULL,NULL,NULL,noir);
	else
		dIt(lpDDSBack,488,400,NULL,NULL,t2_1,NULL,NULL,NULL,NULL,noir);


	dI(lpDDSBack,-181-(640*efftm)/thistm,60,NULL,NULL,intro02_02,NULL,NULL,NULL,NULL);
//	if(rand()>16000)
//		dIo1(lpDDSTemp,0-(181*efftm)/thistm,60,intro02_01,80);

	clsrect(lpDDSBack,0,420,640-(640*efftm)/thistm,20,noir);
}







//cave + effet gyrophare
void e0140(void)
{
	int x=-79+(int)(79*cos((6*(0.8*pi)*efftm)/thistm));
	int y=efftm%4-2;

	//zoom sur le bazar que seul Danube sait ce que c'est
	int sw=610-(int)(30*cos((pi*(x-158))/158));
	int sh=(360*sw)/640;
	int sx=-x+640-sw;
	int sy=(360-sh)/2;


	//cls(lpDDSBack,noir);
	cls(lpDDSTemp,noir);

	
	int opa=128+(int)(127*cos((13*pi*efftm)/thistm));
	dI(lpDDSTemp,0,60+y,width,360,intro03_01,sx,sy,sw,sh);
	dIo2(lpDDSTemp,0,60+y,width,360,intro03_02,sx,sy,sw,sh,opa);


		dIt(lpDDSTemp,200,400,NULL,NULL,t2,NULL,NULL,NULL,NULL,noir);
	if((rand()%100 +efftm)%200>100)
		dIt(lpDDSTemp,488,400,NULL,NULL,t2_2,NULL,NULL,NULL,NULL,noir);
	else
		dIt(lpDDSTemp,488,400,NULL,NULL,t2_1,NULL,NULL,NULL,NULL,noir);

	
	dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}



//still no 3d
void e0141(void)
{
	cls(lpDDSTemp,noir);
	const int wfin= 1710;

	int wcur= 171+((wfin-171)*(thistm-efftm))/thistm;
	int hcur= (wcur*276)/171;
	int xcur=(640-wcur)/2, ycur=(480-hcur)/2;

	dI(lpDDSTemp,xcur,ycur,wcur,hcur,no_3d,0,0,171,276);

	int flash=255-(255*efftm)/thistm;
	clsorect(lpDDSTemp,0,0,640,480,flash,blanc);

	dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}




void e0142(void)
{
	cls(lpDDSBack,noir);
	dI(lpDDSBack,(640-171)/2,(480-276)/2,171,276,no_3d,0,0,171,276);
}




void e01_CLOSE(void)
{
	RELEASE(intro01_01);
	RELEASE(intro01_02);
	RELEASE(intro02_01);
	RELEASE(intro02_02);
	RELEASE(intro03_01);
	RELEASE(intro03_02);
	RELEASE(t0);
	RELEASE(t1);
	RELEASE(t2);
	RELEASE(t2_1);
	RELEASE(t2_2);
	RELEASE(no_3d);
	thistm=0;
}



//---------------------------------------------------------------------------------------------
//TITRE
//---------------------------------------------------------------------------------------------



void e02_OPEN(void)
{
	deux=LdMemPCX(lpDD, blob, 12);
	m=LdMemPCX(lpDD, blob, 13);
	mechant=LdMemTGA(lpDD, blob, 14);
	mutant=LdMemTGA32(lpDD, blob, 15);
	p=LdMemPCX(lpDD, blob, 16);
	pouletz=LdMemTGA32(lpDD, blob, 17);
	project=LdMemTGA32(lpDD, blob, 18);
	dot=LdMemPCX(lpDD, blob, 129);
	ti00=LdMemTGA(lpDD, blob, 19);
	thistm=0;
}



void e0200(void)
{
	dI(lpDDSTemp,0,0,NULL,NULL,ti00,NULL,NULL,NULL,NULL);

	if(efftm<500)
	{
		int flash=255-(255*efftm)/500;
		clsorect(lpDDSTemp,0,0,640,480,flash,blanc);
	}

	dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}




void e0201(void)
{

	dI(lpDDSTemp,0,0,NULL,NULL,ti00,NULL,NULL,NULL,NULL);

	dIa(lpDDSTemp,0+rand()%3,0+rand()%3,mutant);
	dI(lpDDSTemp,209,140,NULL,NULL,m,NULL,NULL,NULL,NULL);

	if(efftm<500)
	{
		int flash=255-(255*efftm)/500;
		clsorect(lpDDSTemp,0,0,640,480,flash,blanc);
	}

	dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}




void e0202(void)
{

	dI(lpDDSTemp,0,0,NULL,NULL,ti00,NULL,NULL,NULL,NULL);

	dIa(lpDDSTemp,0+rand()%2,0+rand()%2,mutant);
	dI(lpDDSTemp,209,140,NULL,NULL,m,NULL,NULL,NULL,NULL);

	dIa(lpDDSTemp,188+rand()%3,324+rand()%3,pouletz);
	dI(lpDDSTemp,347,138,NULL,NULL,p,NULL,NULL,NULL,NULL);

	if(efftm<500)
	{
		int flash=255-(255*efftm)/500;
		clsorect(lpDDSTemp,0,0,640,480,flash,blanc);
	}

	dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}



void e0203(void)
{

	dI(lpDDSTemp,0,0,NULL,NULL,ti00,NULL,NULL,NULL,NULL);

	dIa(lpDDSTemp,0+rand()%2,0+rand()%2,mutant);
	dI(lpDDSTemp,209,140,NULL,NULL,m,NULL,NULL,NULL,NULL);

	dIa(lpDDSTemp,188+rand()%2,324+rand()%2,pouletz);
	dI(lpDDSTemp,347,138,NULL,NULL,p,NULL,NULL,NULL,NULL);

	dI(lpDDSTemp,410,273,NULL,NULL,dot,NULL,NULL,NULL,NULL);
	dIa(lpDDSTemp,411+rand()%2,322+rand()%2,project);

	if(efftm<500)
	{
		int flash=255-(255*efftm)/500;
		clsorect(lpDDSTemp,0,0,640,480,flash,blanc);
	}

	dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}



void e0204(void)
{

	dI(lpDDSTemp,0,0,NULL,NULL,ti00,NULL,NULL,NULL,NULL);

	dIa(lpDDSTemp,rand()%2,rand()%2,mutant);
	dI(lpDDSTemp,209,140,NULL,NULL,m,NULL,NULL,NULL,NULL);

	dIa(lpDDSTemp,188+rand()%2,324+rand()%2,pouletz);
	dI(lpDDSTemp,347,138,NULL,NULL,p,NULL,NULL,NULL,NULL);

	dI(lpDDSTemp,410,273,NULL,NULL,dot,NULL,NULL,NULL,NULL);
	dIa(lpDDSTemp,411+rand()%2,322+rand()%2,project);

	dI(lpDDSTemp,30,120,NULL,NULL,mechant,NULL,NULL,NULL,NULL);
	dI(lpDDSTemp,460,139,NULL,NULL,deux,NULL,NULL,NULL,NULL);


	if(efftm<500)
	{
		int flash=255-(255*efftm)/500;
		clsorect(lpDDSTemp,0,0,640,480,flash,blanc);
	}

	dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}



void e0205(void)
{

	dI(lpDDSTemp,0,0,NULL,NULL,ti00,NULL,NULL,NULL,NULL);

	dIa(lpDDSTemp,rand()%2,rand()%2,mutant);
	dI(lpDDSTemp,209,140,NULL,NULL,m,NULL,NULL,NULL,NULL);

	dIa(lpDDSTemp,188+rand()%2,324+rand()%2,pouletz);
	dI(lpDDSTemp,347,138,NULL,NULL,p,NULL,NULL,NULL,NULL);

	dI(lpDDSTemp,410,273,NULL,NULL,dot,NULL,NULL,NULL,NULL);
	dIa(lpDDSTemp,411+rand()%2,322+rand()%2,project);

	dI(lpDDSTemp,30,120,NULL,NULL,mechant,NULL,NULL,NULL,NULL);
	dI(lpDDSTemp,460,139,NULL,NULL,deux,NULL,NULL,NULL,NULL);


	if(efftm>thistm-2000)
		dIf(lpDDSBack,0,0,lpDDSTemp,255-(255*(efftm-thistm+2000))/2000);
	else
		dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}



void e02_CLOSE(void)
{
	RELEASE(m);
	RELEASE(deux);
	RELEASE(mechant);
	RELEASE(mutant);
	RELEASE(pouletz);
	RELEASE(project);
	RELEASE(ti00);
	RELEASE(p);
	RELEASE(dot);
	thistm=0;
}


//---------------------------------------------------------------------------------------------
//TELEVISION + BOMBE
//---------------------------------------------------------------------------------------------


void e03_OPEN(void)
{
	bip01=LdMemPCX(lpDD, blob, 20);
	bip02=LdMemPCX(lpDD, blob, 21);
	bip03=LdMemPCX(lpDD, blob, 22);
	eclat1=LdMemPCX(lpDD, blob, 23);
	eclat2=LdMemPCX(lpDD, blob, 24);
	eclat3=LdMemPCX(lpDD, blob, 25);
	eclat4=LdMemPCX(lpDD, blob, 26);
	eclat5=LdMemPCX(lpDD, blob, 27);
	pling00=LdMemPCX(lpDD, blob, 28);
	pling01=LdMemPCX(lpDD, blob, 29);
	pling02=LdMemPCX(lpDD, blob, 30);
	tele1=LdMemPCX(lpDD, blob, 31);
	tele2=LdMemPCX(lpDD, blob, 32);
	grenade=LdMemPCX(lpDD, blob, 33);
	mg01=LdMemPCX(lpDD, blob, 34);
	trash10a=LdMemPCX(lpDD, blob, 152);
	trash10b=LdMemPCX(lpDD, blob, 153);
	trash10c=LdMemPCX(lpDD, blob, 154);
	atomica=LdMemPCX(lpDD, blob, 130);
	atomicb=LdMemPCX(lpDD, blob, 131);
	thistm=0;
}


void e0300(void)
{
	cls(lpDDSTemp,noir);
	dI(lpDDSTemp,0,60,NULL,NULL,tele1,NULL,NULL,NULL,NULL);

	if(rand()>16000)
		dIo1(lpDDSTemp,0,60,tele2,50);


	int tra=efftm%30;
	if(tra>20)
		dIo1t(lpDDSTemp,0,60,trash10a,5+rand()%5,noir);
	else if(tra>10)
		dIo1t(lpDDSTemp,0,60,trash10b,5+rand()%5,noir);
	else
		dIo1t(lpDDSTemp,0,60,trash10c,5+rand()%5,noir);


	if(efftm<2500)
		dIf(lpDDSBack,0,0,lpDDSTemp,(255*efftm)/2500);
	else
		dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}



void e0301(void)
{
	cls(lpDDSTemp,noir);
	dI(lpDDSTemp,118,60,NULL,NULL,pling01,NULL,NULL,NULL,NULL);
	dIt(lpDDSTemp,0,60,NULL,NULL,pling00,NULL,NULL,NULL,NULL,rouge);

	int tra=efftm%60;
	if(tra>40)
		dIo1t(lpDDSTemp,0,60,trash10c,5+rand()%5,noir);
	else if(tra>20)
		dIo1t(lpDDSTemp,0,60,trash10a,5+rand()%5,noir);
	else
		dIo1t(lpDDSTemp,0,60,trash10b,5+rand()%5,noir);


	dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}



void e0302(void)
{
	//**** rajouter les étoiles (comment?)
	const demit=1400;
	int xb=(width*efftm)/demit;
	int yb=((xb-320)*(xb-320))/2000+280;

	if (efftm<demit/2)
	{
		dI(lpDDSTemp,118,60,NULL,NULL,pling01,NULL,NULL,NULL,NULL);
		dIo1t(lpDDSTemp,xb,yb,mg01,100,0x00FF00F0);
		dIt(lpDDSTemp,0,60,NULL,NULL,pling00,NULL,NULL,NULL,NULL,rouge);
	}
	else
	{
		dI(lpDDSTemp,0,60,NULL,NULL,pling02,NULL,NULL,NULL,NULL);
		dIt(lpDDSTemp,xb,yb,NULL,NULL,mg01,NULL,NULL,NULL,NULL,0x00FF00F0);

		int cx=(efftm-demit/2);
		int cy=((efftm-demit/2)*(efftm-demit/2))/6250;
		int xv1=292-cx/6, yv1=cy+263;
		int xv2=296-cx/5, yv2=cy+301;
		int xv3=338+cx/6, yv3=cy+291;
		int xv4=279-cx/8, yv4=cy+293;
		int xv5=329+cx/4, yv5=cy+247;

		dIt(lpDDSTemp,xv1,yv1,NULL,NULL,eclat1,NULL,NULL,NULL,NULL,0x00FF01F7);
		dIt(lpDDSTemp,xv2,yv2,NULL,NULL,eclat2,NULL,NULL,NULL,NULL,0x00FF01F7);
		dIt(lpDDSTemp,xv3,yv3,NULL,NULL,eclat3,NULL,NULL,NULL,NULL,0x00FF01F7);
		dIt(lpDDSTemp,xv4,yv4,NULL,NULL,eclat4,NULL,NULL,NULL,NULL,0x00FF01F7);
		dIt(lpDDSTemp,xv5,yv5,NULL,NULL,eclat5,NULL,NULL,NULL,NULL,0x00FF01F7);
	}

	clsrect(lpDDSTemp,0,0,640,60,noir);
	clsrect(lpDDSTemp,0,420,640,60,noir);



	int tra=efftm%60;
	if(tra>40)
		dIo1t(lpDDSTemp,0,60,trash10a,5+rand()%5,noir);
	else if(tra>20)
		dIo1t(lpDDSTemp,0,60,trash10b,5+rand()%5,noir);
	else
		dIo1t(lpDDSTemp,0,60,trash10c,5+rand()%5,noir);


	dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);


}



void e0303(void)
{
	//int opa=210+rand()%40;
	cls(lpDDSTemp,noir);

	//dIf(lpDDSTemp,0,60,grenade,opa);
	dI(lpDDSTemp,0,60,640,360,grenade,0,0,640,360);


	if (efftm+750>thistm)
		dI(lpDDSTemp,252,203,71,37,bip03,0,0,71,37);
		//dIf(lpDDSTemp,252,203,bip03,opa);
	else if(efftm%770>385)
		dI(lpDDSTemp,252,203,71,37,bip02,0,0,71,37);
		//dIf(lpDDSTemp,252,203,bip02,opa);
	else
		dI(lpDDSTemp,252,203,71,37,bip01,0,0,71,37);
		//dIf(lpDDSTemp,252,203,bip01,opa);


	int zw=640-((640-418+159)*efftm)/thistm;
	int zh=480-((480-348+154)*efftm)/thistm;
	int zx=(640-zw)/2;
	int zy=(480-zh)/2;



	int tra=efftm%60;
	if(tra>40)
		dIo1t(lpDDSTemp,0,60,trash10a,5+rand()%5,noir);
	else if(tra>20)
		dIo1t(lpDDSTemp,0,60,trash10b,5+rand()%5,noir);
	else
		dIo1t(lpDDSTemp,0,60,trash10c,5+rand()%5,noir);


	int xat=190;
	if (efftm<thistm-750)
		xat+=((650-190)*(thistm-750-efftm))/(thistm-750);

	if(tra>30)
		dIo1t(lpDDSTemp,xat,95,atomica,5+rand()%10,noir);
	else
		dIo1t(lpDDSTemp,xat,95,atomicb,5+rand()%10,noir);


	dI(lpDDSBack,0,0,640,480,lpDDSTemp,zx,zy,zw,zh);
	clsrect(lpDDSBack,0,0,640,60,noir);
	clsrect(lpDDSBack,0,420,640,60,noir);
}


void e03_CLOSE(void)
{
	RELEASE(bip01);
	RELEASE(bip02);
	RELEASE(bip03);
	RELEASE(eclat1);
	RELEASE(eclat2);
	RELEASE(eclat3);
	RELEASE(eclat4);
	RELEASE(eclat5);
	RELEASE(pling00);
	RELEASE(pling01);
	RELEASE(pling02);
	RELEASE(tele1);
	RELEASE(tele2);
	RELEASE(grenade);
	RELEASE(mg01);
	RELEASE(atomica);
	RELEASE(atomicb);
	RELEASE(trash10a);
	RELEASE(trash10b);
	RELEASE(trash10c);
	thistm=0;
}





//---------------------------------------------------------------------------------------------
// EXPLOSION
//---------------------------------------------------------------------------------------------
void explo_OPEN(void)
{
	trash1=LdMemPCX(lpDD, blob, 132);
	trash1b=LdMemPCX(lpDD, blob, 133);
	trash3=LdMemPCX(lpDD, blob, 136);
	trash4=LdMemPCX(lpDD, blob, 137);
	trash4b=LdMemPCX(lpDD, blob, 138);
	trash10a=LdMemPCX(lpDD, blob, 152);
	trash10b=LdMemPCX(lpDD, blob, 153);
	trash10c=LdMemPCX(lpDD, blob, 154);
	atomica=LdMemPCX(lpDD, blob, 130);
	atomicb=LdMemPCX(lpDD, blob, 131);
	explo_tete=LdMemPCX(lpDD, blob, 179);
	explo_fond=LdMemPCX(lpDD, blob, 180);

	thistm=0;
}




void explo(void)
{
	
	const unsigned bl=(2*thistm)/3;
	int f;
	if(efftm<bl)
		f=255-(32*efftm)/bl;
	else
		f=223-(223*(efftm-bl))/(thistm-bl);

	cls(lpDDSTemp,(f<<16)+(f<<8)+f);




int we=600+(int)(40*cos(6*pi*efftm/4));
int he=(360*we)/640;

dI(lpDDSTemp,0,60,640,360,explo_fond,(640-we)/2,(360-he)/2,we,he);


	

	int tra=rand()%60;

	
	if(tra>30)
		dIo1t(lpDDSTemp,0,85,trash1,80,noir);
	else
		dIo1t(lpDDSTemp,0,85,trash1b,80,noir);



	
	int xt3=rand()%(640-311);
	int yt3=60+rand()%(360-347);

	dIo1t(lpDDSTemp,xt3,yt3,trash3,40,noir);




	int xat=rand()%(640-297);
	int yat=55+rand()%10;

	if(tra>30)
		dIo2t(lpDDSTemp,xat,yat,297,360,atomica,0,0,203,246,40,noir);
	else
		dIo2t(lpDDSTemp,xat,yat,297,360,atomicb,0,0,203,246,40,noir);



	
	int xt4=rand()%(640-216);
	int yt4=60+rand()%(360-270);

	if(tra>30)
		dIo1t(lpDDSTemp,xt4,yt4,trash4,60,noir);
	else
		dIo1t(lpDDSTemp,xt4,yt4,trash4b,60,noir);





	int xt4b=rand()%(640-288);
	int yt4b=55+rand()%10;

	if (tra>30)
		dIo2t(lpDDSTemp,xt4b,yt4b,288,360,trash4,0,0,216,270,30,noir);
	else
		dIo2t(lpDDSTemp,xt4b,yt4b,288,360,trash4b,0,0,216,270,30,noir);




	
	if(tra>40)
		dIo1t(lpDDSTemp,0,60,trash10a,80,noir);
	else if(tra>20)
		dIo1t(lpDDSTemp,0,60,trash10b,80,noir);
	else
		dIo1t(lpDDSTemp,0,60,trash10c,80,noir);


	
	dIf(lpDDSBack,0,0,lpDDSTemp,f);





//cls(lpDDSTemp,noir);



//int opa=rand()%256;
//dIo2(lpDDSTemp,150,66,explo_tete,opa);


/*
	const unsigned bl=(2*thistm)/3;
	int f;
	if(efftm<bl)
		f=255-(32*efftm)/bl;
	else
		f=223-(223*(efftm-bl))/(thistm-bl);

clsorect(lpDDSTemp,0,60,640,360,(f<<16)+(f<<8)+f,blanc);
*/


//dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);


}




void explo_CLOSE(void)
{
	RELEASE(trash1);
	RELEASE(trash1b);
	RELEASE(trash3);
	RELEASE(trash4);
	RELEASE(trash4b);
	RELEASE(atomica);
	RELEASE(atomicb);
	RELEASE(trash10a);
	RELEASE(trash10b);
	RELEASE(trash10c);

	RELEASE(explo_tete);
	RELEASE(explo_fond);

	thistm=0;
}


//---------------------------------------------------------------------------------------------
//LE CHAMP
//---------------------------------------------------------------------------------------------

void e04_OPEN(void)
{
	champ=LdMemPCX(lpDD, blob, 36);
	flotte=LdMemPCX(lpDD, blob, 37);
	fumee1=LdMemPCX(lpDD, blob, 38);
	fumee2=LdMemPCX(lpDD, blob, 39);
	grosyeux=LdMemPCX(lpDD, blob, 40);
	tracteur=LdMemPCX(lpDD, blob, 41);
	ombre=LdMemPCX(lpDD, blob, 47);
	trash3=LdMemPCX(lpDD, blob, 136);
	trash5=LdMemPCX(lpDD, blob, 139);
	trash5b=LdMemPCX(lpDD, blob, 140);
	trash13a=LdMemPCX(lpDD, blob, 159);
	trash13b=LdMemPCX(lpDD, blob, 160);
	thistm=0;
}


void e0401(void)
{
	cls(lpDDSTemp,noir);
	dI(lpDDSTemp,0,60,640,360,champ,0,0,640,360);
	
	int oy=480-(385*efftm)/thistm;
	int tx=50;

	if (efftm>700)
	{
		tx=50+(200*(efftm-400))/thistm;

		if((efftm/200)%2)
			dIt(lpDDSTemp,tx-28,109,NULL,NULL,fumee1,NULL,NULL,NULL,NULL,0x6B733C);
		else
			dIt(lpDDSTemp,tx-28,109,NULL,NULL,fumee2,NULL,NULL,NULL,NULL,0x6B733C);
	}


	dIt(lpDDSTemp,tx,120+efftm%2,NULL,NULL,tracteur,NULL,NULL,NULL,NULL,0x6B733C);
	
	if((efftm/70)%2)
		dI(lpDDSTemp,480,140,NULL,NULL,flotte,NULL,NULL,NULL,NULL);

	if ((oy<380 && oy>360) || (oy<350 && oy>310))
		dIt(lpDDSTemp,448,311,NULL,NULL,grosyeux,NULL,NULL,NULL,NULL,rouge);

	int tra=efftm%60;

	if(tra>30)
		dIo1t(lpDDSTemp,375-rand()%2,112-rand()%2,trash13a,5+rand()%10,noir);
	else
		dIo1t(lpDDSTemp,375-rand()%2,112-rand()%2,trash13b,5+rand()%10,noir);
	

	if(tra>30)
		dIo1t(lpDDSTemp,378-rand()%4,60+rand()%4,trash5,3+rand()%7,noir);
	else
		dIo1t(lpDDSTemp,378-rand()%4,60+rand()%4,trash5b,3+rand()%7,noir);
	
	
	dIo1t(lpDDSTemp,rand()%2,73+rand()%2,trash3,3+rand()%7,noir);
		
	
	dIo1t(lpDDSTemp,0,oy,ombre,170,blanc);


	int opa=255;
	if (efftm<500)
		opa=(255*efftm)/500;
	dIf(lpDDSBack,0,0,lpDDSTemp,opa);
}


void e04_CLOSE(void)
{
	RELEASE(champ);
	RELEASE(flotte);
	RELEASE(fumee1);
	RELEASE(fumee2);
	RELEASE(grosyeux);
	RELEASE(tracteur);
	RELEASE(ombre);
	RELEASE(trash3);
	RELEASE(trash5);
	RELEASE(trash5b);
	RELEASE(trash13a);
	RELEASE(trash13b);
	thistm=0;
}

//---------------------------------------------------------------------------------------------
//LE DEBARQUEMENT
//---------------------------------------------------------------------------------------------

void e05_OPEN(void)
{
	villefond=LdMemTGA(lpDD, blob, 35);
	comm=LdMemPCX(lpDD, blob, 42);
	debsoucoupe=LdMemPCX(lpDD, blob, 43);
	debville=LdMemPCX(lpDD, blob, 44);
	soldier=LdMemPCX(lpDD, blob, 45);
	//lumiere=LdMemPCX(lpDD, blob, 46);
	trash9=LdMemPCX(lpDD, blob, 150);
	trash9b=LdMemPCX(lpDD, blob, 151);
	thistm=0;
}


void e0500(void)
{
	int pp[6][3];
	const int trien=500, tsouc=4000, tray1=500, tray2=2500;

	int vfx=(efftm/40)%800;
	cls(lpDDSTemp,noir);
	dI(lpDDSTemp,vfx,60,NULL,NULL,villefond,0,0,800,360);
	dI(lpDDSTemp,vfx-800,60,NULL,NULL,villefond,0,0,800,360);
	dIt(lpDDSTemp,0,328,NULL,NULL,debville,NULL,NULL,NULL,NULL,violet);

	if (efftm>=trien+tsouc+tray1+tray2)
	{
		dIt(lpDDSTemp,0,60,NULL,NULL,debsoucoupe,NULL,NULL,NULL,NULL,violet);
		clsorect(lpDDSTemp,198,185,253,295,107+(efftm%200)/5,blanc);

		int dec[6]={-35, 15, -15, 35, -5, 10};

		for(int i=0;i<6;i++)
		{
			pp[i][0]=1+((442+dec[i])*(efftm-trien-tsouc-tray1-tray2-i*1000))/(thistm-trien-tsouc-tray1-tray2);
			pp[i][1]=1+(379*(efftm-trien-tsouc-tray1-tray2-i*1000))/(thistm-trien-tsouc-tray1-tray2);
			pp[i][2]=130+(720*(efftm-trien-tsouc-tray1-tray2-i*1000))/(thistm-trien-tsouc-tray1-tray2);

			dIt(lpDDSTemp,(640-pp[i][0])/2+dec[i],pp[i][2],pp[i][0],pp[i][1],soldier,0,0,443,380,violet);
		}
	}

	else if (efftm>=trien+tsouc+tray1)
	{
		int lw=1+(252*(efftm-trien-tsouc-tray1))/tray2;
		dIt(lpDDSTemp,0,60,NULL,NULL,debsoucoupe,NULL,NULL,NULL,NULL,violet);
		clsorect(lpDDSTemp,(640-lw)/2+5,185,lw,295,107+(efftm%200)/5,blanc);

		int a=176-(176*(efftm-trien-tsouc-tray1))/tray2;
		int xc=14-abs((int)(a*cos((4.5*pi*(efftm-trien-tsouc-tray1))/tray2)));
		dIt(lpDDSTemp,xc,60,162,420,comm,0,0,162,420,violet);
	}

	else if (efftm>=trien+tsouc)
	{
		int lh=(290*(efftm-trien-tsouc))/tray1;
		dIt(lpDDSTemp,0,60,NULL,NULL,debsoucoupe,NULL,NULL,NULL,NULL,violet);
		clsrect(lpDDSTemp,324,185,1,lh,blanc);
	}
	
	else if (efftm>=trien)
	{
		int sy=1+(354*(efftm-trien))/tsouc;
		dIt(lpDDSTemp,0,60,640,sy,debsoucoupe,0,355-sy,640,sy,violet);
	}
	else
	{/* que dalle */}



	if (efftm>=thistm-1500)
	{
		int xc2=14-(176*(efftm-thistm+1500))/1000;
		dIt(lpDDSTemp,xc2,60,162,420,comm,0,0,162,420,violet);
	}
	else if (efftm>=trien+tsouc+tray1+tray2)
		dIt(lpDDSTemp,14,60,162,420,comm,0,0,162,420,violet);


		
	int opa=255;
	if (efftm>=thistm-1000)
		opa=(255*(thistm-efftm))/1000;

	int tra=efftm%60;
	if (tra>30)
		dIo1t(lpDDSTemp,rand()%4,60+rand()%4,trash9,10+rand()%5,noir);
	else
		dIo1t(lpDDSTemp,rand()%4,60+rand()%4,trash9b,10+rand()%5,noir);

	dIf(lpDDSBack,0,0,lpDDSTemp,opa);
}	


void e05_CLOSE(void)
{
	RELEASE(villefond);
	RELEASE(comm);
	RELEASE(debsoucoupe);
	RELEASE(debville);
	RELEASE(soldier);
//	RELEASE(lumiere);
	RELEASE(trash9);
	RELEASE(trash9b);
	thistm=0;
}

//---------------------------------------------------------------------------------------------
//POULET SUISSE + MUTATION
//---------------------------------------------------------------------------------------------

void e06_OPEN(void)
{
	mutation=LdMemPCX(lpDD, blob, 48);
	splif21=LdMemTGA(lpDD, blob, 49);
	splif22=LdMemPCX(lpDD, blob, 50);
	thistm=0;
}


void e0600(void)
{
	cls(lpDDSTemp,noir);

	int bz=efftm%2-1;
	dI(lpDDSTemp,bz,60+bz,640,360,splif21,0,0,640,360);


	if(efftm<800)
		dIf(lpDDSBack,0,0,lpDDSTemp,(255*efftm)/800);
	else
		dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}


void e0601(void)
{
	cls(lpDDSBack,noir);

	int bz=efftm%2-1;
	dI(lpDDSBack,bz,60+bz,640,360,splif21,0,0,640,360);

	int xps=80+(int)(560*(1-sqrtf((float)efftm/(float)thistm)));
	dIt(lpDDSBack,xps,113,559,307,splif22,0,0,559,307,violet);
}



void e0602(void)
{
	cls(lpDDSBack,noir);

	int bz=efftm%2-1;
	dI(lpDDSBack,bz,60+bz,640,360,splif21,0,0,640,360);
	dIt(lpDDSBack,80,113,559,307,splif22,0,0,559,307,violet);

	dI(lpDDSTemp,bz,65+bz,640,23,splif21,0,5,640,23);	//pour préparer le blur de l'effet suivant
}



void e0603(void)
{
	cls(lpDDSBack,noir);

	int bz=efftm%2-1;
	dI(lpDDSBack,bz,60+bz,640,360,splif21,0,0,640,360);
	dIt(lpDDSBack,80,113,559,307,splif22,0,0,559,307,violet);

	int xm=640-(627*efftm)/thistm;
	dIo2(lpDDSTemp,bz,65+bz,640,23,splif21,0,5,640,23,50);
	dIt(lpDDSTemp,xm,65,1254-xm,23,mutation,0,0,614,23,violet);
	dI(lpDDSBack,0,65,640,23,lpDDSTemp,0,65,640,23);
}




void e0604(void)
{
	cls(lpDDSBack,noir);

	int bz=efftm%2-1;
	dI(lpDDSBack,bz,60+bz,640,360,splif21,0,0,640,360);
	dIt(lpDDSBack,80,113,559,307,splif22,0,0,559,307,violet);

	int wm=1241-(627*efftm)/thistm;
	dIo2(lpDDSTemp,bz,65+bz,640,23,splif21,0,5,640,23,50);
	dIt(lpDDSTemp,13,65,wm,23,mutation,0,0,614,23,violet);
	dI(lpDDSBack,0,65,640,23,lpDDSTemp,0,65,640,23);
}



void e0605(void)
{
	cls(lpDDSBack,noir);

	int bz=efftm%2-1;
	dI(lpDDSBack,bz,60+bz,640,360,splif21,0,0,640,360);
	dIt(lpDDSBack,80,113,559,307,splif22,0,0,559,307,violet);
	dIt(lpDDSBack,13,65,614,23,mutation,0,0,614,23,violet);
}




void e0606(void)
{
	cls(lpDDSBack,noir);

	int bz=efftm%2-1;
	dI(lpDDSBack,bz,60+bz,640,360,splif21,0,0,640,360);
	dIt(lpDDSBack,80,113,559,307,splif22,0,0,559,307,violet);

	int xm=13-(627*efftm)/thistm;
	dIo2(lpDDSTemp,bz,65+bz,640,23,splif21,0,5,640,23,50);
	dIt(lpDDSTemp,xm,65,627-xm,23,mutation,0,0,614,23,violet);
	dI(lpDDSBack,0,65,640,23,lpDDSTemp,0,65,640,23);
}



void e0607(void)
{
	cls(lpDDSBack,noir);

	int bz=efftm%2-1;
	dI(lpDDSBack,bz,60+bz,640,360,splif21,0,0,640,360);
	dIt(lpDDSBack,80,113,559,307,splif22,0,0,559,307,violet);

	int wm=1241-(627*efftm)/thistm;
	dIo2(lpDDSTemp,bz,65+bz,640,23,splif21,0,5,640,23,50);
	dIt(lpDDSTemp,-614,65,wm,23,mutation,0,0,614,23,violet);
	dI(lpDDSBack,0,65,640,23,lpDDSTemp,0,65,640,23);
}



void e0608(void)
{
	cls(lpDDSBack,noir);

	int bz=efftm%2-1;
	dI(lpDDSBack,bz,60+bz,640,360,splif21,0,0,640,360);
	dIt(lpDDSBack,80,113,559,307,splif22,0,0,559,307,violet);
}



void e06_CLOSE(void)
{
	RELEASE(mutation);
	RELEASE(splif21);
	RELEASE(splif22);
	thistm=0;
}


//---------------------------------------------------------------------------------------------
//FANTOME + FEAR
//---------------------------------------------------------------------------------------------

void e08_OPEN(void)
{
	fear=LdMemPCX(lpDD, blob, 51);
	splif11=LdMemPCX(lpDD, blob, 52);
	splif12=LdMemPCX(lpDD, blob, 53);
	thistm=0;
}


void e0800(void)
{
	cls(lpDDSBack,noir);


	dI(lpDDSBack,0,60,485,360,splif11,0,0,485,360);

	int fy=500-((500-155)*efftm)/thistm;
	dIt(lpDDSBack,200,fy,405,265,splif12,0,0,405,265,violet);

	if (thistm-efftm<1000)
		dIo1t(lpDDSBack,60+rand()%3,198+rand()%3,fear,255-(255*(thistm-efftm))/1000,violet);

	int flh=255-(255*efftm)/500;
	if (flh<0)	flh=0;

	int cflh=(flh<<16)+(flh<<8)+flh;
	clsrect(lpDDSBack,0,0,640,60,cflh);
	clsrect(lpDDSBack,0,420,640,60,cflh);
}



void e0801(void)
{
	cls(lpDDSTemp,noir);
	dI(lpDDSTemp,0,60,485,360,splif11,0,0,485,360);

	int px=60-(60*efftm)/thistm;
	dIt(lpDDSTemp,px+rand()%3,198+rand()%3,252,81,fear,0,0,252,81,violet);

	
	int fx=200+(100*efftm)/thistm, fy=165-(int)(10*cos((6*pi*efftm)/thistm));
	dIt(lpDDSTemp,fx,fy,405,265,splif12,0,0,405,265,violet);


	clsrect(lpDDSTemp,0,420,640,60,noir);

/*	
	int flh=255-(255*efftm)/500;
	if (flh<0)	flh=0;

	int cflh=(flh<<16)+(flh<<8)+flh;
	clsrect(lpDDSTemp,0,0,640,60,cflh);
	clsrect(lpDDSTemp,0,420,640,60,cflh);
*/

	if (thistm-efftm<2790)
		dIf(lpDDSBack,0,0,lpDDSTemp,(255*(thistm-efftm))/2790);
	else
		dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}



void e08_CLOSE(void)
{
	RELEASE(fear);
	RELEASE(splif11);
	RELEASE(splif12);
	thistm=0;
}


//---------------------------------------------------------------------------------------------
//BOOH
//---------------------------------------------------------------------------------------------
void boohf_OPEN(void)
{
	gruh1=LdMemPCX(lpDD, blob, 109);
	gruh2=LdMemPCX(lpDD, blob, 121);
	booh=LdMemPCX(lpDD, blob, 105);
	trash6a=LdMemPCX(lpDD, blob, 141);
	trash6b=LdMemPCX(lpDD, blob, 142);
	trash6c=LdMemPCX(lpDD, blob, 143);
	thistm=0;
}



void boohf(void)
{
	cls(lpDDSTemp,noir);
	dI(lpDDSTemp,0,60,640,360,booh,0,0,640,360);


	int tra=efftm%240;
	if (tra>160)
		dIo1t(lpDDSTemp,0,265+rand()%5,trash6a,10+rand()%10,noir);
	else if (tra>80)
		dIo1t(lpDDSTemp,0,265+rand()%5,trash6b,10+rand()%10,noir);
	else
		dIo1t(lpDDSTemp,0,265+rand()%5,trash6c,10+rand()%10,noir);


	int xr=27+rand()%6, yr=67+rand()%6;
	dIt(lpDDSTemp,xr,yr,110,322,gruh2,0,0,110,322,blanc);
	dIt(lpDDSTemp,xr+4,yr-2,110,322,gruh1,0,0,110,322,blanc);
	
	
	dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}


void boohf_CLOSE(void)
{
	RELEASE(gruh1);
	RELEASE(gruh2);
	RELEASE(booh);
	RELEASE(trash6a);
	RELEASE(trash6b);
	RELEASE(trash6c);
	thistm=0;
}


//---------------------------------------------------------------------------------------------
//LES CREDITS
//---------------------------------------------------------------------------------------------


void e09_OPEN(void)
{
	creddan=LdMemPCX(lpDD, blob, 54);
	credmars=LdMemPCX(lpDD, blob, 55);
	credrez=LdMemPCX(lpDD, blob, 56);
	credfond=LdMemPCX(lpDD, blob, 57);
	credits=LdMemPCX(lpDD, blob, 58);
	trash8a=LdMemPCX(lpDD, blob, 147);
	trash8b=LdMemPCX(lpDD, blob, 148);
	trash8c=LdMemPCX(lpDD, blob, 149);
	thistm=0;
}


void e0900(void)
{
	cls(lpDDSTemp,noir);
	dI(lpDDSTemp,0,95,640,325,credfond,20+(efftm%400)/20,(efftm%300)/15,640,325);
	dIt(lpDDSTemp,0,60,640,360,credits,0,0,640,360,violet);


	dIt(lpDDSTemp,390+rand()%2,129+rand()%2,137,53,credmars,0,0,137,53,blanc);
	dIt(lpDDSTemp,364+rand()%2,248+rand()%2,95,42,credrez,0,0,95,42,blanc);
	dIt(lpDDSTemp,224+rand()%2,318+rand()%2,197,58,creddan,0,0,197,58,blanc);

	int tra=efftm%240;

	if (tra>160)
	{
		dIo1t(lpDDSTemp,281,60,trash8a,5+rand()%5,noir);
		dIo1t(lpDDSTemp,281,180,trash8b,5+rand()%5,noir);
		dIo1t(lpDDSTemp,281,300,trash8c,5+rand()%5,noir);
	}
	else if (tra>80)
	{
		dIo1t(lpDDSTemp,281,60,trash8b,5+rand()%5,noir);
		dIo1t(lpDDSTemp,281,180,trash8c,5+rand()%5,noir);
		dIo1t(lpDDSTemp,281,300,trash8a,5+rand()%5,noir);
	}
	else
	{
		dIo1t(lpDDSTemp,281,60,trash8c,5+rand()%10,noir);
		dIo1t(lpDDSTemp,281,180,trash8a,5+rand()%10,noir);
		dIo1t(lpDDSTemp,281,300,trash8b,5+rand()%10,noir);
	}


		if(efftm<1000)
	{
		int flash=255-(255*efftm)/1000;
		clsorect(lpDDSTemp,0,0,640,480,flash,blanc);
	}

	dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}



void e09_CLOSE(void)
{
	RELEASE(creddan);
	RELEASE(credmars);
	RELEASE(credrez);
	RELEASE(credfond);
	RELEASE(credits);
	RELEASE(trash8a);
	RELEASE(trash8b);
	RELEASE(trash8c);
	thistm=0;
}


//---------------------------------------------------------------------------------------------
//FRA PART
//---------------------------------------------------------------------------------------------

void frapart_OPEN(void)
{
	frafond=LdMemPCX(lpDD, blob, 106);
	fralayer=LdMemPCX(lpDD, blob, 107);
	thistm=0;
}



void frapart(void)
{
	dI(lpDDSTemp,0,0,640,480,frafond,0,0,640,480);

	int yl1=60-(240*efftm)/thistm;
	int yl2=yl1+300;

	dIo1(lpDDSTemp,0,yl1,fralayer,200);
	dIo1t(lpDDSTemp,10,yl2,fralayer,150,0xCEA958);

	clsrect(lpDDSTemp,0,0,377,60,noir);
	clsrect(lpDDSTemp,0,420,377,60,noir);


	if(efftm<500)
	{
		int flash=255-(255*efftm)/500;
		clsorect(lpDDSTemp,0,0,640,480,flash,blanc);
	}

	dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}



void frapart_CLOSE(void)
{
	RELEASE(frafond);
	RELEASE(fralayer);
	thistm=0;
}


//---------------------------------------------------------------------------------------------
//GUESTS GFX
//---------------------------------------------------------------------------------------------

void e12_OPEN(void)
{
	guests=LdMemPCX(lpDD, blob, 70);
	gacetone=LdMemPCX(lpDD, blob, 71);
	gak=LdMemPCX(lpDD, blob, 72);
	gfond=LdMemPCX(lpDD, blob, 73);
	gsplif=LdMemPCX(lpDD, blob, 74);
	gfra=LdMemPCX(lpDD, blob, 110);
	thistm=0;
}


void e1200(void)
{
	cls(lpDDSTemp,noir);
	dI(lpDDSTemp,0,95,640,325,gfond,(efftm%400)/20,(efftm%300)/15,640,325);
	dIt(lpDDSTemp,0,60,640,360,guests,0,0,640,360,violet);


	dIt(lpDDSTemp,177+rand()%2,101+rand()%2,225,51,gacetone,0,0,225,51,blanc);
	dIt(lpDDSTemp,80+rand()%2,158+rand()%2,160,46,gsplif,0,0,160,46,blanc);
	dIt(lpDDSTemp,213+rand()%2,215+rand()%2,214,53,gak,0,0,241,53,blanc);
	dIt(lpDDSTemp,432+rand()%2,282+rand()%2,134,64,gfra,0,0,134,64,blanc);


		if(efftm<500)
	{
		int flash=255-(255*efftm)/500;
		clsorect(lpDDSTemp,0,0,640,480,flash,blanc);
	}


	if(efftm>thistm-1500)
		dIf(lpDDSBack,0,0,lpDDSTemp,255-(255*(efftm-thistm+1500))/1500);
	else
		dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}



void e12_CLOSE(void)
{
	RELEASE(guests);
	RELEASE(gacetone);
	RELEASE(gak);
	RELEASE(gfond);
	RELEASE(gsplif);
	RELEASE(gfra);
	thistm=0;
}


//---------------------------------------------------------------------------------------------
//TETONNE
//---------------------------------------------------------------------------------------------

void tetonnef_OPEN(void)
{
	tetonne=LdMemPCX(lpDD, blob, 117);
	tetonnefond=LdMemTGA(lpDD, blob, 118);
	tetonnetexte=LdMemPCX(lpDD, blob, 119);
	trash7a=LdMemPCX(lpDD, blob, 144);
	trash7b=LdMemPCX(lpDD, blob, 145);
	trash7c=LdMemPCX(lpDD, blob, 146);
	trash8a=LdMemPCX(lpDD, blob, 147);
	trash8b=LdMemPCX(lpDD, blob, 148);
	trash8c=LdMemPCX(lpDD, blob, 149);
	thistm=0;
}



void tetonnef(void)
{
	cls(lpDDSTemp,noir);

	int xc=0-(168*efftm)/thistm;

	dI(lpDDSTemp,xc,60,808,360,tetonnefond,0,0,80,90);
	dItv(lpDDSTemp,0,60,640,360);


	dIt(lpDDSTemp,0,82,640,338,tetonne,0,0,640,338,violet);
	dIt(lpDDSTemp,224+rand()%4,122+rand()%4,414,199,tetonnetexte,0,0,414,199,violet);


	int tra=efftm%240;

	if (tra>160)
	{
		dIo1t(lpDDSTemp,0,60,trash7a,5+rand()%10,noir);
		dIo1t(lpDDSTemp,0,180,trash7b,5+rand()%10,noir);
		dIo1t(lpDDSTemp,0,300,trash7c,5+rand()%10,noir);

		dIo1t(lpDDSTemp,281,60,trash8a,5+rand()%10,noir);
		dIo1t(lpDDSTemp,281,180,trash8b,5+rand()%10,noir);
		dIo1t(lpDDSTemp,281,300,trash8c,5+rand()%10,noir);
	}
	else if (tra>80)
	{
		dIo1t(lpDDSTemp,0,60,trash7b,5+rand()%10,noir);
		dIo1t(lpDDSTemp,0,180,trash7c,5+rand()%10,noir);
		dIo1t(lpDDSTemp,0,300,trash7a,5+rand()%10,noir);

		dIo1t(lpDDSTemp,281,60,trash8b,5+rand()%10,noir);
		dIo1t(lpDDSTemp,281,180,trash8c,5+rand()%10,noir);
		dIo1t(lpDDSTemp,281,300,trash8a,5+rand()%10,noir);
	}
	else
	{
		dIo1t(lpDDSTemp,0,60,trash7c,5+rand()%10,noir);
		dIo1t(lpDDSTemp,0,180,trash7a,5+rand()%10,noir);
		dIo1t(lpDDSTemp,0,300,trash7b,5+rand()%10,noir);

		dIo1t(lpDDSTemp,281,60,trash8c,5+rand()%10,noir);
		dIo1t(lpDDSTemp,281,180,trash8a,5+rand()%10,noir);
		dIo1t(lpDDSTemp,281,300,trash8b,5+rand()%10,noir);
	}



	if(efftm<500)
		dIf(lpDDSBack,0,0,lpDDSTemp,(255*efftm)/500);
	else if(efftm>thistm-500)
		dIf(lpDDSBack,0,0,lpDDSTemp,255-(255*(efftm-thistm+500))/500);
	else
		dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}



void tetonnef_CLOSE(void)
{
	RELEASE(tetonne);
	RELEASE(tetonnefond);
	RELEASE(tetonnetexte);
	RELEASE(trash7a);
	RELEASE(trash7b);
	RELEASE(trash7c);
	RELEASE(trash8a);
	RELEASE(trash8b);
	RELEASE(trash8c);
	thistm=0;
}


//---------------------------------------------------------------------------------------------
//HANDS-UP 1
//---------------------------------------------------------------------------------------------


void handsupf1_OPEN(void)
{
	handsup=LdMemPCX(lpDD, blob, 114);
	hufond1=LdMemPCX(lpDD, blob, 111);
	hufond2=LdMemPCX(lpDD, blob, 112);
	hufond3=LdMemPCX(lpDD, blob, 113);
	thistm=0;
}



void handsupf1(void)
{
	cls(lpDDSTemp,noir);

//	static int x1,x2,x3;
	int xhu=(243*efftm)/thistm;
//	if (xhu<0)
//	{xhu=0;}
//	else
//	{
		int x3=(66*efftm)/thistm;
		int x2=(125*efftm)/thistm;
		int x1=(184*efftm)/thistm;
//	}


	dIt(lpDDSTemp,0,60,640,360,hufond3,x3,0,640,360,violet);
	dIt(lpDDSTemp,0,60,640,360,hufond2,x2,0,640,360,violet);
	dIt(lpDDSTemp,0,60,640,360,hufond1,x1,0,640,360,violet);

	int opa2=(255*(thistm-efftm))/750;
	if(opa2<255)
		dIo2t(lpDDSTemp,0,114,640,306,handsup,xhu,0,640,306,opa2,violet);
	else
		dIt(lpDDSTemp,0,114,640,306,handsup,xhu,0,640,306,violet);

	int opa=(255*efftm)/750;
	if(opa<255)
		dIf(lpDDSBack,0,0,lpDDSTemp,opa);
	else
		dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}



void handsupf1_CLOSE(void)
{
	RELEASE(handsup);
	thistm=0;
}



//---------------------------------------------------------------------------------------------
//HANDS-UP 2
//---------------------------------------------------------------------------------------------


void handsupf2_OPEN(void)
{
	handsup2=LdMemTGA(lpDD, blob, 115);
	thistm=0;
}



void handsupf2(void)
{
	cls(lpDDSTemp,noir);

	int xhu=232-(232*efftm)/thistm;
	int x3=66-(66*efftm)/thistm;
	int x2=125-(125*efftm)/thistm;
	int x1=184-(184*efftm)/thistm;

	dIt(lpDDSTemp,0,60,640,360,hufond3,x3,0,640,360,violet);
	dIt(lpDDSTemp,0,60,640,360,hufond2,x2,0,640,360,violet);
	dIt(lpDDSTemp,0,60,640,360,hufond1,x1,0,640,360,violet);

	int opa2=(255*efftm)/750;
	if(opa2<255)
		dIo2t(lpDDSTemp,0,60,640,360,handsup2,xhu,0,640,360,opa2,violet);
	else
		dIt(lpDDSTemp,0,60,640,360,handsup2,xhu,0,640,360,violet);

	int opa=(255*(thistm-efftm))/750;
	if(opa<255)
		dIf(lpDDSBack,0,0,lpDDSTemp,opa);
	else
		dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}



void handsupf2_CLOSE(void)
{
	RELEASE(handsup2);
	RELEASE(hufond1);
	RELEASE(hufond2);
	RELEASE(hufond3);
	thistm=0;
}


//---------------------------------------------------------------------------------------------
//LA BASTON: SHEEP DEFENDER vs POULETZ
//---------------------------------------------------------------------------------------------

void e10_OPEN(void)
{
	fight01=LdMemPCX(lpDD, blob, 59);
	fight02=LdMemPCX(lpDD, blob, 60);
	fight03=LdMemPCX(lpDD, blob, 61);
	netoile=LdMemPCX(lpDD, blob, 62);
	spirale=LdMemPCX(lpDD, blob, 63);
	trbleus=LdMemPCX(lpDD, blob, 64);
	sheepdef=LdMemPCX(lpDD, blob, 122);
	vs=LdMemPCX(lpDD, blob, 123);
	evilpoulet=LdMemPCX(lpDD, blob, 124);
	combo=LdMemPCX(lpDD, blob, 125);
	ko=LdMemPCX(lpDD, blob, 126);
	thistm=0;
}


void e1010(void)
{
	int xf=(1*efftm)/2;
	mosaic(lpDDSTemp,xf,60,trbleus);
	dIt(lpDDSTemp,0,105,640,315,fight01,0,0,640,315,violet);
	clsrect(lpDDSTemp,0,0,640,60,noir);
	clsrect(lpDDSTemp,0,420,640,60,noir);

	if(efftm>thistm/5)
		dIt(lpDDSTemp,10,120,365,46,sheepdef,0,0,365,46,noir);
	if(efftm>2*thistm/5)
		dIt(lpDDSTemp,250,280,79,42,vs,0,0,79,42,noir);
	if(efftm>3*thistm/5)
		dIt(lpDDSTemp,345,330,263,41,evilpoulet,0,0,263,41,noir);


		if(efftm<500)
	{
		int flash=255-(255*efftm)/500;
		clsorect(lpDDSTemp,0,0,640,480,flash,blanc);
	}

	dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}


void e1020(void)
{
	int yf=(2*efftm)/5;
	mosaic(lpDDSTemp,0,-yf,spirale);
	dIt(lpDDSTemp,0,60,525,360,fight02,0,0,525,360,violet);
	clsrect(lpDDSTemp,0,0,640,60,noir);
	clsrect(lpDDSTemp,0,420,640,60,noir);

	if(efftm>thistm/4 && efftm%600 > 200)
		dIt(lpDDSTemp,465,10,170,142,combo,0,0,170,142,noir);


	if(efftm<500)
	{
		int flash=255-(255*efftm)/500;
		clsorect(lpDDSTemp,0,0,640,480,flash,blanc);
	}

	dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}


void e1030(void)
{
	int pf=(1*efftm)/2;
	mosaic(lpDDSTemp,-pf,-pf,netoile);
	dIt(lpDDSTemp,0,60,555,360,fight03,0,0,555,360,violet);
	clsrect(lpDDSTemp,0,0,640,60,noir);
	clsrect(lpDDSTemp,0,420,640,60,noir);

	if(efftm>thistm/4 && efftm%600 > 200)
		dIt(lpDDSTemp,7,270,184,131,ko,0,0,184,131,0xFF8A00);


	if(efftm<500)
	{
		int flash=255-(255*efftm)/500;
		clsorect(lpDDSTemp,0,0,640,480,flash,blanc);
	}

	dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}


void e10_CLOSE(void)
{
	RELEASE(fight01);
	RELEASE(fight02);
	RELEASE(fight03);
	RELEASE(netoile);
	RELEASE(spirale);
	RELEASE(trbleus);
	RELEASE(sheepdef);
	RELEASE(vs);
	RELEASE(evilpoulet);
	RELEASE(combo);
	RELEASE(ko);
	thistm=0;
}



//---------------------------------------------------------------------------------------------
//BIRTH
//---------------------------------------------------------------------------------------------

void birth_OPEN(void)
{
	timast=LdMemPCX(lpDD, blob, 120);
	birthfond=LdMemTGA(lpDD, blob, 104);
	pouletacet1=LdMemPCX(lpDD, blob, 116);
	trash11a=LdMemPCX(lpDD, blob, 155);
	trash11b=LdMemPCX(lpDD, blob, 156);
	trash12a=LdMemPCX(lpDD, blob, 157);
	trash12b=LdMemPCX(lpDD, blob, 158);
	thistm=0;
}



void birth(void)
{

	//clsrect(lpDDSTemp,0,60,640,360,noir);
	const int wf=800;
	int wc=(640+wf)/2+(int)(((wf-640)/2)*cos((11*pi*efftm)/thistm));
	int hc=(wc*360)/640;
	int xc=(640-wc)/2, yc=(480-hc)/2;

	dI(lpDDSTemp,xc,yc,wc,hc,birthfond,0,0,80,45);

	clsrect(lpDDSTemp,0,0,640,60,noir);
	clsrect(lpDDSTemp,0,420,640,60,noir);


	const unsigned int cp1=thistm/8;
	const unsigned int cp3=(3*thistm)/9;
	const unsigned int cp5=(5*thistm)/9;
	const unsigned int cp7=(7*thistm)/9;
	const unsigned int cp8=(8*thistm)/9;

	int tra=efftm%240;

	if(efftm<cp1)
	{
		dIo1t(lpDDSTemp,0,60,timast,(255*efftm)/cp1,violet);
		if(tra>120)
			dIo1t(lpDDSTemp,398,60,trash11a,(35*efftm)/cp1,noir);
		else
			dIo1t(lpDDSTemp,398,60,trash11b,(35*efftm)/cp1,noir);
	}

	
	else if (efftm<cp3)
	{
		dIt(lpDDSTemp,0,60,391,360,timast,0,0,391,360,violet);

		if(tra>120)
			dIo1t(lpDDSTemp,398,60,trash11a,35+rand()%15,noir);
		else
			dIo1t(lpDDSTemp,398,60,trash11b,35+rand()%15,noir);
	}



	else if (efftm<cp5)
	{
		dIo1t(lpDDSTemp,0,60,timast,255-(255*(efftm-cp3))/(cp5-cp3),violet);
		dIo1t(lpDDSTemp,395,60,pouletacet1,(255*(efftm-cp3))/(cp5-cp3),violet);

		if(tra>120)
		{
			dIo1t(lpDDSTemp,398,60,trash11a,35-(35*(efftm-cp3))/(cp5-cp3),noir);
			dIo1t(lpDDSTemp,0,60,trash12a,(25*(efftm-cp3))/(cp5-cp3),noir);
		}
		else
		{
			dIo1t(lpDDSTemp,398,60,trash11b,35-(35*(efftm-cp3))/(cp5-cp3),noir);
			dIo1t(lpDDSTemp,0,60,trash12a,(25*(efftm-cp3))/(cp5-cp3),noir);
		}
	}



	else if (efftm<cp7)
	{
		dIt(lpDDSTemp,395,60,235,360,pouletacet1,0,0,235,360,violet);

		if(tra>120)
			dIo1t(lpDDSTemp,0,60,trash12a,20+rand()%15,noir);
		else
			dIo1t(lpDDSTemp,0,60,trash12b,20+rand()%15,noir);
	}




	else if (efftm<cp8)
	{
		dIo1t(lpDDSTemp,395,60,pouletacet1,255-(255*(efftm-cp7))/(cp8-cp7),violet);
		if(tra>120)
			dIo1t(lpDDSTemp,0,60,trash12a,25-(25*(efftm-cp7))/(cp8-cp7),noir);
		else
			dIo1t(lpDDSTemp,0,60,trash12b,25-(25*(efftm-cp7))/(cp8-cp7),noir);
	}


	if(efftm>thistm-1500)
		dIf(lpDDSBack,0,0,lpDDSTemp,255-(255*(efftm-thistm+1500))/1500);
	else
		dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}



void birth_CLOSE(void)
{
	RELEASE(timast);
	RELEASE(birthfond);
	RELEASE(pouletacet1);
	RELEASE(trash11a);
	RELEASE(trash11b);
	RELEASE(trash12a);
	RELEASE(trash12b);
	thistm=0;
}


//---------------------------------------------------------------------------------------------
//VILLE MASTERIZED
//---------------------------------------------------------------------------------------------

void e14_OPEN(void)
{
	city=LdMemPCX(lpDD, blob, 90);
	master=LdMemPCX(lpDD, blob, 91);
	villefond=LdMemTGA(lpDD, blob, 35);
	bomb=LdMemPCX(lpDD,blob,162);
	calodox=LdMemPCX(lpDD,blob,163);
	condense=LdMemPCX(lpDD,blob,164);
	dim4=LdMemPCX(lpDD,blob,165);
	digitalmurder=LdMemPCX(lpDD,blob,166);
	eclipse=LdMemPCX(lpDD,blob,167);
	etron=LdMemPCX(lpDD,blob,168);
	equinox=LdMemPCX(lpDD,blob,169);
	mandarine=LdMemPCX(lpDD,blob,170);
	move=LdMemPCX(lpDD,blob,171);
	ojuice=LdMemPCX(lpDD,blob,172);
	orion=LdMemPCX(lpDD,blob,173);
	pixelfr=LdMemPCX(lpDD,blob,174);
	popsyteam=LdMemPCX(lpDD,blob,175);
	purple=LdMemPCX(lpDD,blob,176);
	threestate=LdMemPCX(lpDD,blob,177);
	tpolm=LdMemPCX(lpDD,blob,178);

	thistm=0;
}



void e1410(void)
{
	const int arrivee=4000;
	int vfx=(efftm/30)%800;
	cls(lpDDSTemp,noir);
	dI(lpDDSTemp,0-vfx,60,NULL,NULL,villefond,0,0,800,360);
	dI(lpDDSTemp,800-vfx,60,NULL,NULL,villefond,0,0,800,360);
	
	int ctx=(641*efftm)/thistm;
	dIt(lpDDSTemp,0,148,640,272,city,ctx,0,640,272,violet);
	
	int msx;
	if (efftm>arrivee)
	{
		//msx=290+(int)(350*(1-sqrtf((float)(efftm-thistm+6000)/(float)2000)));
		msx=640-((640+322)*(efftm-arrivee))/(thistm-arrivee-1000);
		//if (msx<290) msx=290;
		dIt(lpDDSTemp,msx,81,322,339,master,0,0,322,339,violet);
	}
	else
		msx=9999;
	
	
	LPDIRECTDRAWSURFACE7 greetsg1[10]={dim4, mandarine, eclipse, orion, digitalmurder, calodox, move, popsyteam, bomb, ojuice};
	LPDIRECTDRAWSURFACE7 greetsg2[7]={equinox, threestate, pixelfr, condense, tpolm, purple, etron};
	static int blink1[10][4]={{450,150,500,0},{440,330,700,0},{370,250,900,0},{340,370,1100,0},{300,100,1300,0},{200,200,1500,0},{220,310,1700,0},{100,100,1900,0},{100,230,2100,0},{50,350,2300,0}};
	static int blink2[7][4]={{450,150,thistm-500,0},{433,290,thistm-700,0},{350,210,thistm-900,0},{300,370,thistm-1100,0},{250,100,thistm-1300,0},{120,200,thistm-1500,0},{100,310,thistm-1700,0}};
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd,sizeof(ddsd));
	
	if(efftm >= thistm-1000 )
	{
		for (int gind2=0; gind2<7; gind2++)
			if( frame%( /*1+(efftm-thistm+1000)/170*/ 2 ) == 0)
				dIt(lpDDSTemp,blink2[gind2][0],blink2[gind2][1],NULL,NULL,greetsg2[gind2],NULL,NULL,NULL,NULL,0x833B27);
			
			dIf(lpDDSBack,0,0,lpDDSTemp,255-(255*(efftm-thistm+1000))/1000);
	}
	else if ( efftm < thistm-1000)
	{
		
		for (int gind1=0; gind1<10; gind1++)
		{
			(greetsg1[gind1])->GetSurfaceDesc(&ddsd);
			
			if( blink1[gind1][3] == 1 )
				dIt(lpDDSTemp,blink1[gind1][0] - 600*(efftm-blink1[gind1][2])/500,blink1[gind1][1],NULL,NULL,greetsg1[gind1],NULL,NULL,NULL,NULL,0x833B27);
			
			else if (msx <= (signed)(blink1[gind1][0] + ddsd.dwWidth + 80))
			{
				blink1[gind1][2]=efftm;
				blink1[gind1][3]=1;
			}
			
			else if( efftm > (unsigned)blink1[gind1][2] && efftm < (unsigned)blink1[gind1][2]+500 )
			{
				if( frame%( 1+(500-efftm+blink1[gind1][2])/170 ) == 0)
				{
					dIt(lpDDSTemp,blink1[gind1][0],blink1[gind1][1],NULL,NULL,greetsg1[gind1],NULL,NULL,NULL,NULL,0x833B27);
				}
			}
			
			else if (efftm >= (unsigned)blink1[gind1][2]+500)
				dIt(lpDDSTemp,blink1[gind1][0],blink1[gind1][1],NULL,NULL,greetsg1[gind1],NULL,NULL,NULL,NULL,0x833B27);
		}
		
		
		
		
		for (int gind2=0; gind2<7; gind2++)
		{
			
			if (blink2[gind2][3] != 0)
			{
				int gtx=640-((640-blink2[gind2][0])*(efftm-blink2[gind2][3]))/500;
				if( gtx < blink2[gind2][0] )
					gtx=blink2[gind2][0];
				
				dIt(lpDDSTemp,gtx,blink2[gind2][1],NULL,NULL,greetsg2[gind2],NULL,NULL,NULL,NULL,0x833B27);
			}
			
			
			else if( msx+322+5 <= blink2[gind2][0] )
				blink2[gind2][3]=efftm;
		}
		
		
		
		dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
	}
}







void e14_CLOSE(void)
{
	RELEASE(city);
	RELEASE(master);
	RELEASE(villefond);
	RELEASE(bomb);
	RELEASE(calodox);
	RELEASE(condense);
	RELEASE(dim4);
	RELEASE(digitalmurder);
	RELEASE(eclipse);
	RELEASE(etron);
	RELEASE(equinox);
	RELEASE(mandarine);
	RELEASE(move);
	RELEASE(ojuice);
	RELEASE(orion);
	RELEASE(pixelfr);
	RELEASE(popsyteam);
	RELEASE(purple);
	RELEASE(threestate);
	RELEASE(tpolm);

	thistm=0;
}


//---------------------------------------------------------------------------------------------
//POULETZ REVELATION
//---------------------------------------------------------------------------------------------

void e11_OPEN(void)
{
	reveal=LdMemPCX(lpDD, blob, 65);
	revealfond=LdMemPCX(lpDD, blob, 66);
	dontresist=LdMemPCX(lpDD, blob, 161);
	toostrong=LdMemPCX(lpDD, blob, 67);
	upw=LdMemTGA(lpDD, blob, 68);
	upwfond=LdMemPCX(lpDD, blob, 69);
	trash10a=LdMemPCX(lpDD, blob, 152);
	trash10b=LdMemPCX(lpDD, blob, 153);
	trash10c=LdMemPCX(lpDD, blob, 154);
	thistm=0;
}

void e1110(void)
{
	int fy=efftm/3;
	mosaic(lpDDSTemp,0,-fy,revealfond);

	int ry=-275+(695*efftm)/thistm;
//	if(ry>70) ry=70;
	dIt(lpDDSTemp,50,ry,540,695,reveal,0,0,540,695,violet);

	dIt(lpDDSTemp,0,60,211,360,dontresist,0,0,211,360,violet);

	clsrect(lpDDSTemp,0,0,640,60,noir);
	clsrect(lpDDSTemp,0,420,640,60,noir);

	int tra=efftm%240;

	if (tra>160)
		dIo1t(lpDDSTemp,0,60,trash10a,15+rand()%8,noir);
	else if (tra>80)
		dIo1t(lpDDSTemp,0,60,trash10b,15+rand()%8,noir);
	else
		dIo1t(lpDDSTemp,0,60,trash10c,15+rand()%8,noir);

/*
	int opa=(255*efftm)/750;
	if(opa>255) opa=255;
	dIf(lpDDSBack,0,0,lpDDSTemp,opa);
*/
	dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}



void e1120(void)
{
	//cls(lpDDSBack,noir);
	dI(lpDDSTemp,0,60,640,360,upwfond,0,140,640,360);


/*
    lpD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, noir, 0L, 0L);


   D3DMATRIX matSpin;
    matSpin._11 = matSpin._22 = matSpin._33 = matSpin._44 = 1.0f;
    matSpin._12 = matSpin._13 = matSpin._14 = matSpin._41 = 0.0f;
    matSpin._21 = matSpin._23 = matSpin._24 = matSpin._42 = 0.0f;
    matSpin._31 = matSpin._32 = matSpin._34 = matSpin._43 = 0.0f;
    
    matSpin._11 =  (FLOAT)cos( efftm/64 );
    matSpin._22 =  (FLOAT)cos( efftm/64 );
    matSpin._21 = -(FLOAT)sin( efftm/64 );
    matSpin._12 =  (FLOAT)sin( efftm/64 );
 
    lpD3DDevice->SetTransform( D3DTRANSFORMSTATE_WORLD, &matSpin );
    
	
	if(FAILED(lpD3DDevice->BeginScene()))
        Error("could not begin a rendering");
	lpD3DDevice->SetTexture( 0, upwfond );
	lpD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,D3DFVF_VERTEX,vrect,4,D3DDP_WAIT);
    lpD3DDevice->EndScene();

*/
	clsrect(lpDDSTemp,0,0,640,60,noir);
	clsrect(lpDDSTemp,0,420,640,60,noir);
	//clsrect(lpDDSBack,0,60,640,360,0xE09209);
	//dIr(lpDDSBack,60,60,120,120,upwfond,50,50,200,200,0);
	dIt(lpDDSTemp,171,76,297,344,upw,0,0,297,344,violet);
	dIt(lpDDSTemp,310,275,330,145,toostrong,0,0,330,145,violet);


	if(efftm<500)
	{
		int flash=255-(255*efftm)/500;
		clsorect(lpDDSTemp,0,0,640,480,flash,blanc);
	}


	if(efftm>thistm-1000)
		dIf(lpDDSBack,0,0,lpDDSTemp,255-(255*(efftm-thistm+1000))/1000);
	else
		dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}


void e11_CLOSE(void)
{
	RELEASE(reveal);
	RELEASE(revealfond);
	RELEASE(dontresist);
	RELEASE(toostrong);
	RELEASE(upw);
	RELEASE(upwfond);
	RELEASE(trash10a);
	RELEASE(trash10b);
	RELEASE(trash10c);
	thistm=0;
}


//---------------------------------------------------------------------------------------------
//STREETO
//---------------------------------------------------------------------------------------------

void e13_OPEN(void)
{
	strciel=LdMemTGA(lpDD, blob, 83);
	strcocha=LdMemPCX(lpDD, blob, 84);
	streyes=LdMemPCX(lpDD, blob, 85);
	strmurs=LdMemPCX(lpDD, blob, 86);
	strstreum=LdMemPCX(lpDD, blob, 87);
	strtoits=LdMemPCX(lpDD, blob, 88);
	trash2=LdMemPCX(lpDD, blob, 134);
	trash2b=LdMemPCX(lpDD, blob, 135);
	thistm=0;
}

void e1310(void)
{
	cls(lpDDSTemp,noir);
	int xciel=50-(50*efftm)/thistm;
	int xcocha=70+efftm%2-1;
	const int ytoits=90;

	dIt(lpDDSTemp,xciel,60,500,360,strciel,0,0,500,360,violet);
	dIt(lpDDSTemp,xciel+500,60,500,360,strciel,0,0,500,360,violet);

	dIt(lpDDSTemp,120,ytoits,400,120,strtoits,0,0,400,120,violet);
	clsrect(lpDDSTemp,0,ytoits+120,640,420-120-ytoits,noir);

	dIt(lpDDSTemp,-30,60,NULL,NULL,strmurs,0,0,700,360,violet);

	dIt(lpDDSTemp,xcocha,165,156,259,strcocha,0,0,156,259,violet);

	int tra=efftm%120;
	if(tra>60)
		dIo1t(lpDDSTemp,391,60,trash2,5+rand()%10,noir);
	else	
		dIo1t(lpDDSTemp,391,60,trash2b,5+rand()%10,noir);
	
	
	dIf(lpDDSBack,0,0,lpDDSTemp,(255*efftm)/thistm);
}



void e1311(void)
{
	int defil=(50*thistm)/effets[noeffet-1].tm;
	int xciel=0-(defil*efftm)/thistm;
	//int xtoits=;
	int xcocha=70-(45*efftm)/thistm+efftm%2-1;
	int ystreum=440-((int)sqrt((300*300*efftm)/thistm));
	const int ytoits=90;

	dIt(lpDDSTemp,xciel,60,500,360,strciel,0,0,500,360,violet);
	dIt(lpDDSTemp,xciel+500,60,500,360,strciel,0,0,500,360,violet);

	dIt(lpDDSTemp,120,ytoits,400,120,strtoits,0,0,400,120,violet);
	clsrect(lpDDSTemp,0,ytoits+120,640,420-120-ytoits,noir);

	dIt(lpDDSTemp,-30,60,NULL,NULL,strmurs,0,0,700,360,violet);

	dIt(lpDDSTemp,0,ystreum,640,119,strstreum,0,0,640,119,violet);
	clsrect(lpDDSTemp,0,ystreum+119,640,420-119-ystreum,noir);


	clsrect(lpDDSTemp,0,0,640,60,noir);
	clsrect(lpDDSTemp,0,420,640,60,noir);
	dIt(lpDDSTemp,xcocha,165,156,259,strcocha,0,0,156,259,violet);

	int tra=efftm%240;
	if(tra>120)
		dIo1t(lpDDSTemp,391,60,trash2,5+rand()%10,noir);
	else	
		dIo1t(lpDDSTemp,391,60,trash2b,5+rand()%10,noir);

	dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}

void e1312(void)
{
	int defil=(50*thistm)/effets[noeffet-2].tm;
	int xciel=0-(50*effets[noeffet-1].tm)/effets[noeffet-2].tm-(defil*efftm)/thistm;
	int xcocha=25-(10*efftm)/thistm+efftm%2-1;
	const int ytoits=90;

	dIt(lpDDSTemp,xciel,60,500,360,strciel,0,0,500,360,violet);
	dIt(lpDDSTemp,xciel+500,60,500,360,strciel,0,0,500,360,violet);

	dIt(lpDDSTemp,120,ytoits,400,120,strtoits,0,0,400,120,violet);
	clsrect(lpDDSTemp,0,ytoits+120,640,420-120-ytoits,noir);

	dIt(lpDDSTemp,-30,60,NULL,NULL,strmurs,0,0,700,360,violet);

	dIt(lpDDSTemp,0,140,640,119,strstreum,0,0,640,119,violet);
	clsrect(lpDDSTemp,0,259,640,161,noir);


	clsrect(lpDDSTemp,0,0,640,60,noir);
	clsrect(lpDDSTemp,0,420,640,60,noir);
	dIt(lpDDSTemp,xcocha,165,156,259,strcocha,0,0,156,259,violet);

	
	int tra=efftm%240;
	if(tra>120)
		dIo1t(lpDDSTemp,391,60,trash2,10+rand()%10,noir);
	else	
		dIo1t(lpDDSTemp,391,60,trash2b,10+rand()%10,noir);
	
	
	dIf(lpDDSBack,0,0,lpDDSTemp,255-(255*efftm)/thistm);
}

void e1320(void)
{
	cls(lpDDSBack,noir);
	dI(lpDDSBack,rand()%4,60+rand()%4,640,360,streyes,0,0,640,360);
}


void e13_CLOSE(void)
{
	RELEASE(strciel);
	RELEASE(streyes);
	RELEASE(strstreum);
	RELEASE(strcocha);
	RELEASE(strmurs);
	RELEASE(strtoits);
	RELEASE(trash2);
	RELEASE(trash2b);
	thistm=0;
}



//---------------------------------------------------------------------------------------------
//MUTATION HAS NOW STARTED
//---------------------------------------------------------------------------------------------

void started_OPEN(void)
{
	brume01=LdMemPCX(lpDD, blob, 75);
	brume02=LdMemPCX(lpDD, blob, 76);
	brume03=LdMemPCX(lpDD, blob, 77);
	brume04=LdMemPCX(lpDD, blob, 78);
	brume05=LdMemPCX(lpDD, blob, 79);
	mutachat=LdMemPCX(lpDD, blob, 80);
	mutapig=LdMemPCX(lpDD, blob, 81);
	ruelle03=LdMemPCX(lpDD, blob, 82);
	mutation2=LdMemPCX(lpDD, blob, 127);
	hasnow=LdMemPCX(lpDD, blob, 128);
	started=LdMemPCX(lpDD, blob, 89);
	thistm=0;
}


void e1330(void)
{
	int opa=efftm%4-2+150-(150*efftm)/thistm;
	int xb1=efftm%4-2+0-(565*efftm)/thistm;
	int xb2=efftm%4-2+188+(452*efftm)/thistm;
	int xb3=efftm%4-2+0-(363*efftm)/thistm;
	int xb4=efftm%4-2+154+(486*efftm)/thistm;
	int yb5=efftm%2-1+311+(109*efftm)/thistm;


	dI(lpDDSTemp,0,60,640,360,ruelle03,0,0,640,360);
	dIt(lpDDSTemp,345+(80*efftm)/thistm,89,312,331,mutapig,0,0,312,331,violet);
	dIt(lpDDSTemp,-5-(80*efftm)/thistm,104,384,316,mutachat,0,0,384,316,violet);

	dIo1t(lpDDSTemp,xb1,137,brume01,opa,violet);
	dIo1t(lpDDSTemp,xb2,175,brume02,opa,violet);
	dIo1t(lpDDSTemp,xb3,60,brume03,opa,violet);
	dIo1t(lpDDSTemp,xb4,60,brume04,opa,violet);
	dIo1t(lpDDSTemp,95,yb5,brume05,opa,violet);

	int xs=240+(int)(5*cos((9*pi*efftm)/thistm))+efftm%4-2;
	int ys=51+(int)(5*sin((7.5*pi*efftm)/thistm))+efftm%4-2;
	dIt(lpDDSTemp,xs,ys,354,17,started,0,0,354,17,noir);	

	clsrect(lpDDSTemp,0,0,640,60,noir);
	clsrect(lpDDSTemp,0,420,640,60,noir);



	int fade;

	if (efftm<thistm/5)
	{
		fade=(255*efftm)/(thistm/5);
		dIo1t(lpDDSTemp,0,60,mutation2,fade,noir);
	}

	if (efftm>=thistm/5 && efftm<2*thistm/5)
	{
		fade=(255*(efftm-thistm/5))/(thistm/5);
		dIo1t(lpDDSTemp,0,60,mutation2,255-fade,noir);
		dIo1t(lpDDSTemp,220,60,hasnow,fade,noir);
	}

	if (efftm>=2*thistm/5 && efftm<3*thistm/5)
	{
		fade=(255*(efftm-2*thistm/5))/(thistm/5);
		dIo1t(lpDDSTemp,220,60,hasnow,255-fade,noir);
		dIo1t(lpDDSTemp,464,60,started,fade,noir);
	}

	if (efftm>=3*thistm/5 && efftm<4*thistm/5)
	{
		fade=(255*(efftm-3*thistm/5))/(thistm/5);
		dIo1t(lpDDSTemp,464,60,started,255-fade,noir);
	}


	dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}


void started_CLOSE(void)
{
	RELEASE(brume01);
	RELEASE(brume02);
	RELEASE(brume03);
	RELEASE(brume04);
	RELEASE(brume05);
	RELEASE(mutapig);
	RELEASE(mutachat);
	RELEASE(ruelle03);
	RELEASE(mutation2);
	RELEASE(hasnow);
	RELEASE(started);
	thistm=0;
}


//---------------------------------------------------------------------------------------------
//GENERIQUE
//---------------------------------------------------------------------------------------------

void e15_OPEN(void)
{
	ge1=LdMemPCX(lpDD, blob, 92);
	ge2=LdMemPCX(lpDD, blob, 93);
	ge3=LdMemPCX(lpDD, blob, 94);
	ge4=LdMemPCX(lpDD, blob, 95);
	ge5=LdMemPCX(lpDD, blob, 96);
	ge6=LdMemPCX(lpDD, blob, 97);
	ge7=LdMemPCX(lpDD, blob, 98);
	ge8=LdMemPCX(lpDD, blob, 99);
	gefra=LdMemPCX(lpDD, blob, 108);
	genefond=LdMemPCX(lpDD, blob, 100);
	thistm=0;
}

/*
void e1500(void)
{
	cls(lpDDSTemp,noir);
	dI(lpDDSTemp,97,62,446,354,genefond,0,0,446,354);

	if(efftm<1500)
	{
		int flash=255-(255*efftm)/1500;
		clsorect(lpDDSTemp,0,0,640,480,flash,blanc);
	}

	dI(lpDDSBack,0,0,640,480,lpDDSTemp,0,0,640,480);
}
*/

void e1501(void)
{
	cls(lpDDSBack,noir);
	dI(lpDDSBack,97,62,446,354,genefond,0,0,446,354);
	dI(lpDDSBack,126,223,410,61,ge1,0,0,410,61);
}

void e1502(void)
{
	cls(lpDDSBack,noir);
	dI(lpDDSBack,97,62,446,354,genefond,0,0,446,354);
	dI(lpDDSBack,126,223,410,61,ge2,0,0,410,61);
}

void e1503(void)
{
	cls(lpDDSBack,noir);
	dI(lpDDSBack,97,62,446,354,genefond,0,0,446,354);
	dI(lpDDSBack,126,223,410,61,ge3,0,0,410,61);
}

void e1504(void)
{
	cls(lpDDSBack,noir);
	dI(lpDDSBack,97,62,446,354,genefond,0,0,446,354);
	dI(lpDDSBack,126,223,410,61,ge4,0,0,410,61);
}

void e1505(void)
{
	cls(lpDDSBack,noir);
	dI(lpDDSBack,97,62,446,354,genefond,0,0,446,354);
	dI(lpDDSBack,126,223,410,61,ge5,0,0,410,61);
}

void e1506(void)
{
	cls(lpDDSBack,noir);
	dI(lpDDSBack,97,62,446,354,genefond,0,0,446,354);
	dI(lpDDSBack,126,223,410,61,ge6,0,0,410,61);
}

void e1507(void)
{
	cls(lpDDSBack,noir);
	dI(lpDDSBack,97,62,446,354,genefond,0,0,446,354);
	dI(lpDDSBack,126,223,410,61,ge7,0,0,410,61);
}

void e1508(void)
{
	cls(lpDDSBack,noir);
	dI(lpDDSBack,97,62,446,354,genefond,0,0,446,354);
	dI(lpDDSBack,126,223,410,61,gefra,0,0,410,61);
}

void e1509(void)
{
	cls(lpDDSBack,noir);
	dI(lpDDSBack,97,62,446,354,genefond,0,0,446,354);
	dI(lpDDSBack,126,223,410,61,ge8,0,0,410,61);
}


void e15_CLOSE(void)
{
	RELEASE(ge1);
	RELEASE(ge2);
	RELEASE(ge3);
	RELEASE(ge4);
	RELEASE(ge5);
	RELEASE(ge6);
	RELEASE(ge7);
	RELEASE(ge8);
	RELEASE(gefra);
	RELEASE(genefond);
	thistm=0;
}
//---------------------------------------------------------------------------------------------
//GNIII !!!!
//---------------------------------------------------------------------------------------------

void e16_OPEN(void)
{
	main1=LdMemPCX(lpDD, blob, 101);
	main2=LdMemPCX(lpDD, blob, 102);
	gniii_tete=LdMemTGA(lpDD, blob, 103);
	gni_fond=LdMemPCX(lpDD, blob, 181);
	thistm=0;
}

void e1600(void)
{
	cls(lpDDSBack,noir);
	dI(lpDDSBack,0,60,640,360,gni_fond,0,0,640,360);
	dIt(lpDDSBack,150,60,399,360,gniii_tete,0,0,399,360,violet);
	
	int sk=rand()%10-5;
	dIt(lpDDSBack,130+sk,301,133,119,main1,0,0,133,119,violet);
	dIt(lpDDSBack,295+sk,304,145,116,main2,0,0,145,116,violet);
}



void e16_CLOSE(void)
{
	RELEASE(main1);
	RELEASE(main2);
	RELEASE(gniii_tete);
	thistm=0;
}



//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------


//LISTE D'EFFETS
//UNE SEQUENCE PAR LIGNE
eff effets[]={{MAXINT,-1,-1,eff_onetimestart},{1000,-1,-1,eff_incipit},
/*........*/


{MAXINT, -1, -1, e01_OPEN},
{2400, -1, -1,	e0101},{9300,-1,-1,e0102},	//scroll 1
{8000,-1,-1,e0122},{300,-1,-1,e0125},		//scroll 2
{6000,-1,-1,e0140},							//cave
{3000,-1,-1,e0141},{1500,-1,-1,e0142},		//still no 3D
{MAXINT,-1,-1,e01_CLOSE},


{MAXINT,-1,-1,e02_OPEN},
{1800,-1,-1,e0200},{400,-1,-1,e0201},{400,-1,-1,e0202},{400,-1,-1,e0203},{500,-1,-1,e0204},{3300,-1,-1,e0205},		//titre
{MAXINT,-1,-1,e02_CLOSE},


{MAXINT,-1,-1,e03_OPEN},
{7500,-1,-1,e0300},{700,-1,-1,e0301},{2000,-1,-1,e0302},
{5500,-1,-1,e0303},	//tv + pling + bombe
{MAXINT,-1,-1,e03_CLOSE},


{MAXINT,-1,-1,explo_OPEN},
{4000,-1,-1,explo},	//explosion de la mort
{MAXINT,-1,-1,explo_CLOSE},


{MAXINT,-1,-1,e04_OPEN},
{10300,-1,-1,e0401},	//ombres à la ID4
{MAXINT,-1,-1,e04_CLOSE},


{MAXINT,-1,-1,e05_OPEN},
{14000,-1,-1,e0500},	//debarquement
{MAXINT,-1,-1,e05_CLOSE},


{MAXINT,-1,-1,e06_OPEN},
{1000,-1,-1,e0600},{1500,-1,-1,e0601},{1000,-1,-1,e0602},{750,-1,-1,e0603},{750,-1,-1,e0604},{350,-1,-1,e0605},{750,-1,-1,e0606},{750,-1,-1,e0607},{850,-1,-1,e0608},	//canif+mutation
{MAXINT,-1,-1,e06_CLOSE},


{MAXINT,-1,-1,e08_OPEN},
{1500,-1,-1,e0800},{9500,-1,-1,e0801},	//fear + fantome-poulet
{MAXINT,-1,-1,e08_CLOSE},


{MAXINT,-1,-1,e09_OPEN},
{2800,-1,-1,e0900},	//credits
{MAXINT,-1,-1,e09_CLOSE},


{MAXINT,-1,-1,boohf_OPEN},
{2900,-1,-1,boohf},		//booh + gruh
{MAXINT,-1,-1,boohf_CLOSE},


{MAXINT,-1,-1,frapart_OPEN},
{5450,-1,-1,frapart},	//frapart
{MAXINT,-1,-1,frapart_CLOSE},


{MAXINT,-1,-1,e12_OPEN},
{2850,-1,-1,e1200},	//guests
{MAXINT,-1,-1,e12_CLOSE},


{MAXINT,-1,-1,tetonnef_OPEN},
{4140,-1,-1,tetonnef},	//ça t'étonne ?
{MAXINT,-1,-1,tetonnef_CLOSE},


{MAXINT,-1,-1,handsupf1_OPEN},
{5010,-1,-1,handsupf1},	//hands up 1
{MAXINT,-1,-1,handsupf1_CLOSE},


{MAXINT,-1,-1,handsupf2_OPEN},
{5010,-1,-1,handsupf2},	//hands up 2
{MAXINT,-1,-1,handsupf2_CLOSE},


{MAXINT,-1,-1,e10_OPEN},
{3034,-1,-1,e1010},{3034,-1,-1,e1020},{3034,-1,-1,e1030},	//baston avec sheep-defender
{MAXINT,-1,-1,e10_CLOSE},


{MAXINT,-1,-1,birth_OPEN},
{10175,-1,-1,birth},	//timaster + birth
{MAXINT,-1,-1,birth_CLOSE},


{MAXINT,-1,-1,e14_OPEN},
{10200,-1,-1,e1410},	//master
{MAXINT,-1,-1,e14_CLOSE},


{MAXINT,-1,-1,e11_OPEN},
{5520,-1,-1,e1110},	//poulet revelation
{2200,-1,-1,e1120},	//too strong
{MAXINT,-1,-1,e11_CLOSE},


{MAXINT,-1,-1,e13_OPEN},
{600,-1,-1,e1310},{5500,-1,-1,e1311},{700,-1,-1,e1312},	//streeto cochat
{1910,-1,-1,e1320},	//gros plan cochat
{MAXINT,-1,-1,e13_CLOSE},


{MAXINT,-1,-1,started_OPEN},
{7680,-1,-1,e1330},	//cochat mutation has already started
{MAXINT,-1,-1,started_CLOSE},


{MAXINT,-1,-1,e15_OPEN},
//{1500,-1,-1,e1500},
{1416,-1,-1,e1501},{1416,-1,-1,e1502},{1416,-1,-1,e1503},{1416,-1,-1,e1504},{1416,-1,-1,e1505},{1416,-1,-1,e1506},{1416,-1,-1,e1508},{1416,-1,-1,e1507},
//{2000,-1,-1,e1509},	//générique
{MAXINT,-1,-1,e15_CLOSE},


{MAXINT,-1,-1,e16_OPEN},
{1200,-1,-1,e1600},	//gniii
{MAXINT,-1,-1,e16_CLOSE},



/*........*/
{MAXINT,-1,-1,eff_onetimestop},{MAXINT,-1,-1,NULL}};




#endif