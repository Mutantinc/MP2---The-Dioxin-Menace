//-----------------------------------------------------------
//MG_EFFECTS.H - THE HEART OF THE APP !
//-----------------------------------------------------------
#ifndef _MG_EFFECTS_H_
#define _MG_EFFECTS_H_

//-----------------------------------------------------------
//-----------------------------------------------------------


bool jitter()	//to have a jittering effect
{
	static bool state;
	static int lastTime;
	if( mgTime - lastTime > 25 ) //1/40th second
	{
		lastTime = mgTime;
		if( rand()%RAND_MAX > 28000 )
			state = !state;
	}
	return state;
}

int shake(int mod)	//to have a random shake effect
{
	static int lastTime;
	static int lastRand;
	if( mgTime - lastTime > 20 ) //1/50th second
	{
		lastTime = mgTime;
		lastRand = ( rand()%mod );
	}
	return lastRand;
}

bool overlay(int threshold) // to have a random overlay effect
{
	static int lastTime;
	static bool lastOver;
	if( mgTime - lastTime > 25 ) //1/40th second
	{
		lastTime = mgTime;
		lastOver = ( rand() > threshold );
	}
	return lastOver;
}


//---------------------------------------------------------------------------------------------
//LE SCHBOING ELASTIQUE DU DEBUT
//LES SCROLLINGS
//LE GYROPHARE
//---------------------------------------------------------------------------------------------
mgTexture intro01_01;
mgTexture intro01_02;
mgTexture intro02_01;
mgTexture intro02_02;
mgTexture intro03_01;
mgTexture intro03_02;
mgTexture t0;
mgTexture t1;
mgTexture t2;
mgTexture t2_1;
mgTexture t2_2;
mgTexture no_3d;

const uint e01t[] = { 2400, 9200, 8000, 300, 6000, 3000, 1500 };


void e01_OPEN(void)
{
	mgRasterMode();
	mgClearColor(BLACK);

	intro01_01.Load( intro01_01_ );
	intro01_02.Load( intro01_02_ );
	intro02_01.Load( intro02_01_ );
	intro02_02.Load( intro02_02_ );
	intro03_01.Load( intro03_01_ );
	intro03_02.Load( intro03_02_ );
	
	t0.LoadK( t0_, BLACK );
	t1.LoadK( t1_, BLACK );
	t2.LoadK( t2_, BLACK );
	t2_1.LoadK( t2_1_, BLACK );
	t2_2.LoadK( t2_2_, BLACK );

	no_3d.LoadF( no_3d_, true );

//	mgSoundInit(MG___); NO!
	mgSoundLoadModule( "mp2rmx.xm" );
	mgSoundPlay();
}



//schboing élastique
void e0101(void)
{
	mgCls();
	sint w=(int)(640*((float)mgTime/(float)e01t[0])*(0.7+0.3*(1-sin((3*mgPi*mgTime)/e01t[0]))));
	sint h=(int)(360*((float)mgTime/(float)e01t[0])*(0.7+0.3*(cos((2*mgPi*mgTime)/e01t[0]))));
	mgDrawTex( (mgWidth-w)/2, (mgHeight-h)/2 ,w, h, intro01_02, 0, 0, 640, 360, (255*mgTime)/e01t[0] );
	if( overlay(16000))
		mgDrawTex((mgWidth-w)/2, (mgHeight-h)/2, w, h, intro01_01, 0, 0, 640, 360, (80*mgTime)/e01t[0]);

}




//le premier scrolling
void e0102(void)
{
	mgCls();
	if( mgTime>e01t[1] ) return;

	mgDrawTex(0-(546*mgTime)/e01t[1], 60, intro01_02);
	if(overlay(16000))
		mgDrawTex(0-(546*mgTime)/e01t[1], 60, intro01_01, 80);


	uint inttm1=(e01t[1]/2)-1500, inttm2=(e01t[1]/2)+1500;	//l'effet StarWars dure 3000ms
	const sint wf=100;	//largeur finale

	if(mgTime>inttm1 && mgTime<inttm2)
	{
		sint w=640-((640-wf)*(mgTime-inttm1))/3000;
		sint h=(118*w)/640;
		mgDrawTex((mgWidth-w)/2, (mgHeight-h)/2, w, h, t0, 0, 0, 460, 85, 255-(255*(mgTime-inttm1))/3000);

		sint flash=255-(255*(mgTime-inttm1))/((inttm2-inttm1)/3);
		if (flash<0) flash=0;
		mgCls( 0, 60, 640, 360, flash, WHITE );
	}
	
	if( mgTime>e01t[1]-500 )
		mgFade( (255*(e01t[1]-mgTime))/500 );
}






//le second scrolling
void e0122(void)
{
	mgCls();
	
	mgDrawTex( 0-(181*mgTime)/e01t[2], 60, intro02_02 );
	if(overlay(16000))
		mgDrawTex( 0-(181*mgTime)/e01t[2], 60, intro02_01, 80 );


	uint inttm1=(e01t[2]/2)-1500, inttm2=(e01t[2]/2)+1500;	//effet SW dure 3000
	const wf=80;	//largeur finale

	if(mgTime>inttm1 && mgTime<inttm2)
	{
		sint w=640-((640-wf)*(mgTime-inttm1))/3000;
		sint h=(166*w)/640;
		mgDrawTex( (mgWidth-w)/2, (mgHeight-h)/2, w, h, t1, 0, 0, 336, 87, 255-(255*(mgTime-inttm1))/3000 );

		sint flash=255-(255*(mgTime-inttm1))/((inttm2-inttm1)/3);
		if (flash<0) flash=0;
		mgCls( 0, 60, 640, 360, flash, WHITE );
	}
	
	if(mgTime<500)
		mgFade((255*mgTime)/500);
}





//le shift rapide vers la gauche
void e0125(void)
{
	mgCls();
	sint y=e01t[3]%4-2;
	mgDrawTex( 0, 60+y, intro03_01 );


	mgDrawTex( 200, 400, t2 );
	if((shake(100) +e01t[3])%200>100)
		mgDrawTex( 488, 400, t2_2 );
	else
		mgDrawTex( 488, 400, t2_1 );


	mgDrawTex( -181-(640*mgTime)/e01t[3], 60, intro02_02 );
	if(overlay(16000))
		mgDrawTex( -181-(640*mgTime)/e01t[3], 60, intro02_01, 80 );

	mgCls( 0, 420, 640-(640*mgTime)/e01t[3], 20, BLACK );
}







//cave + effet gyrophare
void e0140(void)
{
	sint x=-79+(int)(79*cos((6*(0.8*mgPi)*mgTime)/e01t[4]));
	sint y=mgTime%4-2;

	//zoom sur le bazar que seul Danube sait ce que c'est
	sint sw=610-(int)(30*cos((mgPi*(x-158))/158));
	sint sh=(360*sw)/640;
	sint sx=-x+640-sw;
	sint sy=(360-sh)/2;

	mgCls();
	
	sint opa=128+(int)(127*cos((13*mgPi*mgTime)/e01t[4]));
	mgDrawTex( 0, 60+y, mgWidth, 360, intro03_01, sx, sy, sw, sh );
	mgDrawTex( 0, 60+y, mgWidth, 360, intro03_02, sx, sy, sw, sh, opa );

	mgDrawTex( 200, 400, t2 );
	if( jitter() )
	//if((shake(200) + mgTime)%200>100)
		mgDrawTex( 488, 400, t2_2 );
	else
		mgDrawTex( 488, 400, t2_1 );
}





//still no 3d
void e0141(void)
{
	mgCls();
	const sint wfin= 1710;

	sint wcur= 171+((wfin-171)*(e01t[5]-mgTime))/e01t[5];
	sint hcur= (wcur*276)/171;
	sint xcur=(640-wcur)/2, ycur=(480-hcur)/2;

	int jit = ((shake(5))*(e01t[5]-mgTime))/e01t[5];
	mgDrawTex( xcur+jit, ycur+jit, wcur, hcur, no_3d, 0, 0, 171, 276 );

	sint flash = 255-(255*mgTime)/e01t[5];
	mgCls( flash, WHITE );
}




void e0142(void)
{
	mgCls();
	mgDrawTex( (640-171)/2, (480-276)/2, no_3d );
}




void e01_CLOSE(void)
{
	intro01_01.Unbind();
	intro01_02.Unbind();
	intro02_01.Unbind();
	intro02_02.Unbind();
	intro03_01.Unbind();
	intro03_02.Unbind();
	t0.Unbind();
	t1.Unbind();
	t2.Unbind();
	t2_1.Unbind();
	t2_2.Unbind();
	no_3d.Unbind();
}


//---------------------------------------------------------------------------------------------
//TITRE
//---------------------------------------------------------------------------------------------
mgTexture deux;
mgTexture m;
mgTexture mechant_poulet;
mgTexture mutant;
mgTexture p;
mgTexture pouletz;
mgTexture project;
mgTexture dot;
mgTexture ti00;

const uint e02t[] = { 1800, 400, 400, 400, 500, 3300 };


void e02_OPEN(void)
{
	deux.LoadK( deux_, PURPLE );
	m.LoadK( m_, PURPLE );
	mechant_poulet.LoadK( mechant_poulet_, PURPLE );
	mutant.Load( mutant_ );
	p.LoadK( p_, PURPLE );
	pouletz.Load( pouletz_ );
	project.Load( project_ );
	dot.LoadK( dot_, PURPLE );
	ti00.Load( ti00_ );
}



void e0200(void)
{
	mgCls();
	mgDrawTex( 0, 0, ti00 );

	if(mgTime<500)
	{
		int flash=255-(255*mgTime)/500;
		mgCls( flash, WHITE );
	}
}




void e0201(void)
{
	mgCls();
	mgDrawTex( 0, 0, ti00 );

	mgDrawTex( 0+shake(3), 0+shake(3), mutant );
	mgDrawTex( 209, 140, m );

	if(mgTime<500)
	{
		int flash=255-(255*mgTime)/500;
		mgCls( flash, WHITE );
	}
}




void e0202(void)
{
	mgCls();
	mgDrawTex( 0, 0, ti00 );

	mgDrawTex( 0+shake(2), 0+shake(2), mutant );
	mgDrawTex( 209, 140, m );

	mgDrawTex( 188+shake(3), 324+shake(3), pouletz );
	mgDrawTex( 347, 138, p );

	if(mgTime<500)
	{
		int flash=255-(255*mgTime)/500;
		mgCls( flash, WHITE );
	}
}



void e0203(void)
{
	mgCls();
	mgDrawTex( 0, 0, ti00 );

	mgDrawTex( 0+shake(2), 0+shake(2), mutant );
	mgDrawTex( 209, 140, m );

	mgDrawTex( 188+shake(2), 324+shake(2), pouletz );
	mgDrawTex( 347, 138, p );

	mgDrawTex( 410, 273, dot );
	mgDrawTex( 411+shake(2), 322+shake(2), project );

	if(mgTime<500)
	{
		int flash=255-(255*mgTime)/500;
		mgCls( flash, WHITE );
	}
}



void e0204(void)
{
	mgCls();
	mgDrawTex( 0, 0, ti00 );

	mgDrawTex( shake(2), shake(2), mutant );
	mgDrawTex( 209, 140, m );

	mgDrawTex( 188+shake(2), 324+shake(2), pouletz );
	mgDrawTex( 347, 138, p );

	mgDrawTex( 410, 273, dot );
	mgDrawTex( 411+shake(2), 322+shake(2), project );

	mgDrawTex( 30, 120, mechant_poulet );
	mgDrawTex( 460, 139, deux );


	if(mgTime<500)
	{
		int flash=255-(255*mgTime)/500;
		mgCls( flash, WHITE );
	}
}



void e0205(void)
{
	mgCls();
	if( mgTime>e02t[5] ) return;

	mgDrawTex( 0, 0, ti00 );

	mgDrawTex( shake(2), shake(2), mutant );
	mgDrawTex( 209, 140, m );

	mgDrawTex( 188+shake(2), 324+shake(2), pouletz );
	mgDrawTex( 347, 138, p );

	mgDrawTex( 410, 273, dot );
	mgDrawTex( 411+shake(2), 322+shake(2), project );

	mgDrawTex( 30, 120, mechant_poulet );
	mgDrawTex( 460, 139, deux );


	if(mgTime>e02t[5]-2000)
		mgFade( 255-(255*(mgTime-e02t[5]+2000))/2000 );
}



void e02_CLOSE(void)
{
	m.Unbind();
	deux.Unbind();
	mechant_poulet.Unbind();
	mutant.Unbind();
	pouletz.Unbind();
	project.Unbind();
	ti00.Unbind();
	p.Unbind();
	dot.Unbind();
}


//---------------------------------------------------------------------------------------------
//TELEVISION + BOMBE
//---------------------------------------------------------------------------------------------
mgTexture bip01;
mgTexture bip02;
mgTexture bip03;
mgTexture eclat1;
mgTexture eclat2;
mgTexture eclat3;
mgTexture eclat4;
mgTexture eclat5;
mgTexture grenade;
mgTexture mg01;
mgTexture pling00;
mgTexture pling01;
mgTexture pling02;
mgTexture tele1;
mgTexture tele2;
mgTexture star;
mgTexture atomica;
mgTexture atomicb;
mgTexture trash10a;
mgTexture trash10b;
mgTexture trash10c;


const uint e03t[] = { 7500, 700, 2000, 5500 };


void e03_OPEN(void)
{
	bip01.LoadK( bip01_, PURPLE );
	bip02.LoadK( bip02_, PURPLE );
	bip03.LoadK( bip03_, PURPLE );
	eclat1.LoadK( eclat1_, 0x00F701FF );
	eclat2.LoadK( eclat2_, 0x00F701FF );
	eclat3.LoadK( eclat3_, 0x00F701FF );
	eclat4.LoadK( eclat4_, 0x00F701FF );
	eclat5.LoadK( eclat5_, 0x00F701FF );
	pling00.LoadK( pling00_, RED );
	pling01.Load( pling01_ );
	pling02.Load( pling02_ );
	tele1.Load( tele1_ );
	tele2.Load( tele2_ );
	star.LoadK( star_, 0x711616 );
	grenade.Load( grenade_ );
	mg01.LoadK( mg01_, 0x00F000FF );
	trash10a.Load( trash10a_ );
	trash10b.Load( trash10b_ );
	trash10c.Load( trash10c_ );
	atomica.Load( atomica_ );
	atomicb.Load( atomicb_ );
}


void e0300(void)
{
	mgCls();
	mgDrawTex( 0, 60, tele1 );

	if(jitter())
		mgDrawTex( 0, 60, tele2, 50 );


	int tra=mgTime%30;
	if(tra>20)
		mgDrawTex( 0, 60, trash10a, 5+shake(5) );
	else if(tra>10)
		mgDrawTex( 0, 60, trash10b, 5+shake(5) );
	else
		mgDrawTex( 0, 60, trash10c, 5+shake(5) );


	if(mgTime<2500)
		mgFade( (255*mgTime)/2500 );
}



void e0301(void)
{
	mgCls();
	mgDrawTex( 118, 60, pling01 );
	mgDrawTex( 0, 60, pling00 );
	mgDrawTex( 122, 120, star, 128 );

	int tra=mgTime%60;
	if(tra>40)
		mgDrawTex( 0, 60, trash10c, 5+shake(5) );
	else if(tra>20)
		mgDrawTex( 0, 60, trash10a, 5+shake(5) );
	else
		mgDrawTex( 0, 60, trash10b, 5+shake(5) );
}



void e0302(void)
{
	const demit=1400;
	int xb=(mgWidth*mgTime)/demit;
	int yb=((xb-320)*(xb-320))/2000+280;

	if (mgTime<demit/2)
	{
		mgDrawTex( 118, 60, pling01 );
		mgDrawTex( xb, yb, mg01, 100 );
		mgDrawTex( 0, 60, pling00 );
		mgDrawTex( 122, 120, star, 128 );
	}
	else
	{
		mgDrawTex( 0, 60, pling02 );
		mgDrawTex( 122, 120, star, 128 );
		mgDrawTex( xb, yb, mg01 );

		int cx=(mgTime-demit/2);
		int cy=((mgTime-demit/2)*(mgTime-demit/2))/6250;
		int xv1=292-cx/6, yv1=cy+263;
		int xv2=296-cx/5, yv2=cy+301;
		int xv3=338+cx/6, yv3=cy+291;
		int xv4=279-cx/8, yv4=cy+293;
		int xv5=329+cx/4, yv5=cy+247;

		mgDrawTex( xv1, yv1, eclat1 );
		mgDrawTex( xv2, yv2, eclat2 );
		mgDrawTex( xv3, yv3, eclat3 );
		mgDrawTex( xv4, yv4, eclat4 );
		mgDrawTex( xv5, yv5, eclat5 );
	}

	mgCinemaScope();

	int tra=mgTime%60;
	if(tra>40)
		mgDrawTex( 0, 60, trash10a, 5+shake(5) );
	else if(tra>20)
		mgDrawTex( 0, 60, trash10b, 5+shake(5) );
	else
		mgDrawTex( 0, 60, trash10c, 5+shake(5) );
}



void e0303(void)
{
	//mgCls();

	int zw=640-((640-418+159)*mgTime)/e03t[3];
	int zh=480-((480-348+154)*mgTime)/e03t[3];
	int zx=(640-zw)/2;
	int zy=(480-zh)/2;
	mgRasterTransform( zx, zy, zw, zh, 0.f );
	int opa = 210+shake(40);
	mgDrawTex( 0, 60, grenade, opa );


	if (mgTime+750>e03t[3])
		//mgDrawTex( 252, 203, bip03 );
		mgDrawTex( 252, 203, bip03, opa );
	else if(mgTime%770>385)
		//mgDrawTex( 252, 203, bip02 );
		mgDrawTex( 252, 203, bip02, opa );
	else
		//mgDrawTex( 252, 203, bip01 );
		mgDrawTex( 252, 203, bip01, opa );


	int xat=190;
	if (mgTime<e03t[3]-750)
		xat+=((650-190)*(e03t[3]-750-mgTime))/(e03t[3]-750);

	int tra=mgTime%60;
	if(tra>30)
		mgDrawTex( xat, 95, atomica, 5+shake(10) );
	else
		mgDrawTex( xat, 95, atomicb, 5+shake(10) );

	mgRasterMode();

	if(tra>40)
		mgDrawTex( 0, 60, trash10a, 5+shake(5) );
	else if(tra>20)
		mgDrawTex( 0, 60, trash10b, 5+shake(5) );
	else
		mgDrawTex( 0, 60, trash10c, 5+shake(5) );

	mgCinemaScope();
}


void e03_CLOSE(void)
{
	mgRasterMode();
	bip01.Unbind();
	bip02.Unbind();
	bip03.Unbind();
	eclat1.Unbind();
	eclat2.Unbind();
	eclat3.Unbind();
	eclat4.Unbind();
	eclat5.Unbind();
	pling00.Unbind();
	pling01.Unbind();
	pling02.Unbind();
	tele1.Unbind();
	tele2.Unbind();
	star.Unbind();
	grenade.Unbind();
	mg01.Unbind();
	atomica.Unbind();
	atomicb.Unbind();
	trash10a.Unbind();
	trash10b.Unbind();
	trash10c.Unbind();
}





//---------------------------------------------------------------------------------------------
// EXPLOSION
//---------------------------------------------------------------------------------------------
//mgTexture trash1;
//mgTexture trash1b;
mgTexture trash3;
mgTexture trash4;
mgTexture trash4b;
mgTexture explo_tete;
mgTexture explo_fond;

const uint explot = 4000;


void explo_OPEN(void)
{
//	trash1.LoadK( trash1_, BLACK );
//	trash1b.Load( trash1b_ );
	trash3.Load( trash3_ );
	trash4.Load( trash4_ );
	trash4b.Load( trash4b_ );
//	trash10a.Load( trash10a_ );
//	trash10b.Load( trash10b_ );
//	trash10c.Load( trash10c_ );
//	atomica.Load( atomica_ );
//	atomicb.Load( atomicb_ );
	explo_tete.LoadK( explo_tete_, PURPLE );
	explo_fond.Load( explo_fond_ );
}




void explo(void)
{
	
	const uint bl=(2*explot)/3;
	int f;
	if(mgTime<bl)
		f=255-(32*mgTime)/bl;
	else
		f=223-(223*(mgTime-bl))/(explot-bl);

	mgCls((f<<16)+(f<<8)+f);
//	mgCls( 0, 60, 640, 360, (f<<16)+(f<<8)+f, WHITE );


	int we=600+(int)(40*cos(6*mgPi*mgTime/4));
	int he=(360*we)/640;

	mgDrawTex( 0, 60, 640, 360, explo_fond, (640-we)/2, (360-he)/2, we, he );

	
	int xt3=shake(640-311);
	int yt3=60+shake(360-347);

	mgDrawTex( xt3, yt3, trash3, 80 );


	int xt4=shake(640-216);
	int yt4=60+shake(360-270);

	if(jitter())
		mgDrawTex( xt4, yt4, trash4, 80 );
	else
		mgDrawTex( xt4, yt4, trash4b, 80 );



	int xt4b=shake(640-288);
	int yt4b=55+shake(10);

	if (jitter())
		mgDrawTex( xt4b, yt4b, 288, 360, trash4, 30 );
	else
		mgDrawTex( xt4b, yt4b, 288, 360, trash4b, 30 );

/*
	if(tra>40)
		mgDrawTex( 0, 60, trash10a, 80 );
	else if(tra>20)
		mgDrawTex( 0, 60, trash10b, 80 );
	else
		mgDrawTex( 0, 60, trash10c, 80 );
*/

	mgDrawTex( 150, 67, explo_tete, shake(200) );

/*	
	int xat=shake(640-297);
	int yat=55+shake(10);

	if(tra>30)
		mgDrawTex( xat, yat, 297, 360, atomica, 40 );
	else
		mgDrawTex( xat, yat, 297, 360, atomicb, 40 );
*/

//	if(tra>30)
//		mgDrawTex( 0, 85, trash1, 60 );
//	else
//		mgDrawTex( 0, 85, trash1b, 60 );

	mgCinemaScope();

	if( mgTime>=explot-1000 )
		mgFade( (255*(explot-mgTime))/1000 );
}




void explo_CLOSE(void)
{
//	trash1.Unbind();
//	trash1b.Unbind();
	trash3.Unbind();
	trash4.Unbind();
	trash4b.Unbind();
//	atomica.Unbind();
//	atomicb.Unbind();
//	trash10a.Unbind();
//	trash10b.Unbind();
//	trash10c.Unbind();
	explo_tete.Unbind();
	explo_fond.Unbind();
}


//---------------------------------------------------------------------------------------------
//LE CHAMP
//---------------------------------------------------------------------------------------------
mgTexture champ;
mgTexture flotte;
mgTexture fumee1;
mgTexture fumee2;
mgTexture grosyeux;
mgTexture tracteur;
mgTexture ombre;
mgTexture trash5;
mgTexture trash5b;
mgTexture trash13a;
mgTexture trash13b;

const uint e04t = 10300;
  
void e04_OPEN(void)
{
	champ.Load( champ_ );
	flotte.LoadK( flotte_, BLACK );
	fumee1.LoadK( fumee1_, 0x003C736B );
	fumee2.LoadK(fumee2_, 0x003C736B );
	grosyeux.LoadK( grosyeux_, RED );
	tracteur.LoadK( tracteur_, 0x003C736B );
	ombre.LoadK( ombre_, WHITE );
	trash3.Load( trash3_ );
	trash5.Load( trash5_ );
	trash5b.Load( trash5b_ );
	trash13a.Load( trash13a_ );
	trash13b.Load( trash13b_ );
}


void e0401(void)
{
	mgCls();
	mgDrawTex( 0, 60, champ );
	
	int oy=480-(385*mgTime)/e04t;
	int tx=60;

	if (mgTime>700)
	{
		tx+=(200*(mgTime-400))/e04t;

		if((mgTime/200)%2)
			mgDrawTex( tx-28, 109, fumee1 );
		else
			mgDrawTex( tx-28, 109, fumee2 );
	}


	mgDrawTex( tx, 120+mgTime%2, tracteur );
	
	if((mgTime/70)%2)
		mgDrawTex( 482, 141, flotte );

	if ((oy<380 && oy>360) || (oy<350 && oy>310))
		mgDrawTex( 448, 311, grosyeux );

	int tra=mgTime%60;

	if(tra>30)
		mgDrawTex( 375-shake(2), 112-shake(2), trash13a, 5+shake(10) );
	else
		mgDrawTex( 375-shake(2), 112-shake(2), trash13b, 5+shake(10) );
	

	if(tra>30)
		mgDrawTex( 378-shake(4), 60+shake(4), trash5, 3+shake(7) );
	else
		mgDrawTex( 378-shake(4), 60+shake(4), trash5b, 3+shake(7) );
	
	
	mgDrawTex( shake(2), 73+shake(2), trash3, 3+shake(7) );
		
	
	mgDrawTex( 0, oy-421, ombre, 170 );
	mgCinemaScope();

	int opa=255;
	if (mgTime<500)
		opa=(255*mgTime)/500;
	mgFade(opa);


}


void e04_CLOSE(void)
{
	champ.Unbind();
	flotte.Unbind();
	fumee1.Unbind();
	fumee2.Unbind();
	grosyeux.Unbind();
	tracteur.Unbind();
	ombre.Unbind();
	trash3.Unbind();
	trash5.Unbind();
	trash5b.Unbind();
	trash13a.Unbind();
	trash13b.Unbind();
}


//---------------------------------------------------------------------------------------------
//LE DEBARQUEMENT
//---------------------------------------------------------------------------------------------
mgTexture villefond;
mgTexture comm;
mgTexture debsoucoupe;
mgTexture debville;
mgTexture soldier;
mgTexture trash9;
mgTexture trash9b;

const uint e05t = 14000;

void e05_OPEN(void)
{
	villefond.Load( villefond_ );
	comm.LoadK( comm_, PURPLE );
	debsoucoupe.LoadK( debsoucoupe_, PURPLE );
	debville.LoadK( debville_, PURPLE );
	soldier.LoadK( soldier_, PURPLE );
	trash9.Load( trash9_ );
	trash9b.Load( trash9b_ );
}


void e0500(void)
{
	int pp[6][3];
	const int trien=500, tsouc=4000, tray1=500, tray2=2500;

	int vfx=(mgTime/40)%800;
	mgCls();
	mgDrawTex( vfx, 60, 800, 360, villefond, 0, 0, 800, 360 );
	mgDrawTex( vfx-800, 60, 800, 360, villefond, 0, 0, 800, 360 );
	mgDrawTex( 0, 328, debville );

	if (mgTime>=trien+tsouc+tray1+tray2)
	{
		mgDrawTex( 0, 60, debsoucoupe );
		mgCls( 198, 185, 253, 295, 107+(mgTime%200)/5, WHITE );

		int dec[6]={-35, 15, -15, 35, -5, 10};

		for(int i=0;i<6;i++)
		{
			pp[i][0]=1+((442+dec[i])*(mgTime-trien-tsouc-tray1-tray2-i*1000))/(e05t-trien-tsouc-tray1-tray2);
			pp[i][1]=1+(379*(mgTime-trien-tsouc-tray1-tray2-i*1000))/(e05t-trien-tsouc-tray1-tray2);
			pp[i][2]=130+(740*(mgTime-trien-tsouc-tray1-tray2-i*1000))/(e05t-trien-tsouc-tray1-tray2);

			mgDrawTex( (640-pp[i][0])/2+dec[i], pp[i][2], pp[i][0], pp[i][1], soldier, 0, 0, 443, 380 );
		}
	}

	else if (mgTime>=trien+tsouc+tray1)
	{
		int lw=1+(252*(mgTime-trien-tsouc-tray1))/tray2;
		mgDrawTex( 0, 60, debsoucoupe );
		mgCls( (640-lw)/2+5, 185, lw, 295, 107+(mgTime%200)/5, WHITE );

		int a=176-(176*(mgTime-trien-tsouc-tray1))/tray2;
		int xc=14-abs((int)(a*cos((4.5*mgPi*(mgTime-trien-tsouc-tray1))/tray2)));
		mgDrawTex( xc, 60, comm );
	}

	else if (mgTime>=trien+tsouc)
	{
		int lh=(290*(mgTime-trien-tsouc))/tray1;
		mgDrawTex( 0, 60, debsoucoupe );
		mgCls( 324, 185, 1, lh, WHITE );
	}
	
	else if (mgTime>=trien)
	{
		int sy=1+(354*(mgTime-trien))/tsouc;
		mgDrawTex( 0, 60, 640, sy, debsoucoupe, 0, 355-sy, 640, sy );
	}
	else
	{}



	if (mgTime>=e05t-1500)
	{
		int xc2=14-(176*(mgTime-e05t+1500))/1000;
		mgDrawTex( xc2, 60, comm );
	}
	else if (mgTime>=trien+tsouc+tray1+tray2)
		mgDrawTex( 14, 60, comm );


		
	int opa=255;
	if (mgTime>=e05t-1000)
		opa=(255*(e05t-mgTime))/1000;

	int tra=mgTime%60;
	if (tra>30)
		mgDrawTex( shake(4), 60+shake(4), trash9, 10+shake(5) );
	else
		mgDrawTex( shake(4), 60+shake(4), trash9b, 10+shake(5) );

	mgFade(opa);
}	


void e05_CLOSE(void)
{
	villefond.Unbind();
	comm.Unbind();
	debsoucoupe.Unbind();
	debville.Unbind();
	soldier.Unbind();
	trash9.Unbind();
	trash9b.Unbind();
}


//---------------------------------------------------------------------------------------------
//POULET SUISSE + MUTATION
//---------------------------------------------------------------------------------------------
mgTexture mutation;
mgTexture splif2_1;
mgTexture splif2_2;

const uint e06t[] = {1000, 1500, 1000, 750, 750, 350, 750, 750, 850 };

void e06_OPEN(void)
{
	mutation.Load( mutation_ );
	splif2_1.Load( splif2_1_ );
	splif2_2.LoadK( splif2_2_, PURPLE );
}


void e0600(void)
{
	mgCls();

	int bz=shake(2)-1;
	mgDrawTex( bz, 60+bz, splif2_1 );


	if(mgTime<800)
		mgFade((255*mgTime)/800);
}


void e0601(void)
{
	mgCls();

	int bz=shake(2)-1;
	mgDrawTex( bz, 60+bz, splif2_1 );

	int xps=80+(int)(560*(1-sqrt((float)mgTime/(float)e06t[1])));
	mgDrawTex( xps, 113, splif2_2 );
}



void e0602(void)
{
	int bz=shake(2)-1;
	mgDrawTex( bz, 60+bz, splif2_1 );
	mgDrawTex( 80, 113, splif2_2 );
}



void e0603(void)
{
	int bz=shake(2)-1;
	mgDrawTex( bz, 60+bz, splif2_1, 20 );
	mgDrawTex( 80, 113, splif2_2 );

	int xm=640-(627*mgTime)/e06t[3];
	mgDrawTex( xm, 65, 1254-xm, 23, mutation, 0 ,0, 614, 23 );
}




void e0604(void)
{
	int bz=shake(2)-1;
	mgDrawTex( bz, 60+bz, splif2_1, 20 );
	mgDrawTex( 80, 113, splif2_2 );

	int wm=1241-(627*mgTime)/e06t[4];
	mgDrawTex( 13, 65, wm, 23, mutation, 0 ,0, 614, 23 );
}



void e0605(void)
{
	int bz=shake(2)-1;
	mgDrawTex( bz, 60+bz, splif2_1, 20 );
	mgDrawTex( 80, 113, splif2_2 );
	mgDrawTex( 13, 65, 614, 23, mutation, 0 ,0, 614, 23 );
}




void e0606(void)
{
	int bz=shake(2)-1;
	mgDrawTex( bz, 60+bz, splif2_1, 20 );
	mgDrawTex( 80, 113, splif2_2 );

	int xm=13-(627*mgTime)/e06t[6];
	mgDrawTex( xm, 65, 627-xm, 23, mutation, 0 ,0, 614, 23 );
}



void e0607(void)
{
	int bz=shake(2)-1;
	mgDrawTex( bz, 60+bz, splif2_1, 20 );
	mgDrawTex( 80, 113, splif2_2 );

	int wm=1241-(627*mgTime)/e06t[7];
	mgDrawTex( -614, 65, wm, 23, mutation, 0 ,0, 614, 23 );
}



void e0608(void)
{
	int bz=shake(2)-1;
	mgDrawTex( bz, 60+bz, splif2_1, 20 );
	mgDrawTex( 80, 113, splif2_2 );
}



void e06_CLOSE(void)
{
	mutation.Unbind();
	splif2_1.Unbind();
	splif2_2.Unbind();
}


//---------------------------------------------------------------------------------------------
//FANTOME + FEAR
//---------------------------------------------------------------------------------------------
mgTexture splif1_1;
mgTexture splif1_2;
mgTexture fear;

const uint e08t[] = { 1500, 9500 };


void e08_OPEN(void)
{
	splif1_1.Load( splif1_1_ );
	splif1_2.LoadK( splif1_2_, PURPLE );
	fear.LoadK( fear_, 0xDCFEFF );
}


void e0800(void)
{
	mgCls();
	mgDrawTex( 0, 60, splif1_1 );

	int fy=500-((500-155)*mgTime)/e08t[0];
	mgDrawTex( 200, fy, splif1_2 );

	if (e08t[0]-mgTime<1000)
		mgDrawTex( 60+shake(3), 198+shake(3), fear, 255-(255*(e08t[0]-mgTime))/1000 );

	int flh=255-(255*mgTime)/500;
	if (flh<0)	flh=0;

	int cflh=0xFF000000+(flh<<16)+(flh<<8)+flh;
	mgCls( 0, 0, 640, 60, cflh );
	mgCls( 0, 420, 640, 60, cflh );
}



void e0801(void)
{
	mgCls();
	if( mgTime>e08t[1]) return;

	mgDrawTex( 0, 60, splif1_1 );

	int px=60-(60*mgTime)/e08t[1];
	mgDrawTex( px+shake(3), 198+shake(3), fear );

	
	int fx=200+(100*mgTime)/e08t[1], fy=165-(int)(10*cos((6*mgPi*mgTime)/e08t[1]));
	mgDrawTex( fx, fy, splif1_2 );

	mgCls( 0, 420, 640, 60 );
	
//	int flh=255-(255*mgTime)/500;
//	if (flh<0)	flh=0;

//	int cflh=(flh<<16)+(flh<<8)+flh;
//	clsrect(0,0,640,60,cflh);
//	clsrect(0,420,640,60,cflh);


	if (e08t[1]-mgTime<2790)
		mgFade((255*(e08t[1]-mgTime))/2790);
}



void e08_CLOSE(void)
{
	fear.Unbind();
	splif1_1.Unbind();
	splif1_2.Unbind();
}



//---------------------------------------------------------------------------------------------
//LES CREDITS
//---------------------------------------------------------------------------------------------
mgTexture cred_danube;
mgTexture cred_rez;
mgTexture cred_mars;
mgTexture cred_fond;
mgTexture credits;
mgTexture trash8a;
mgTexture trash8b;
mgTexture trash8c;

const uint e09t = 2800;

void e09_OPEN(void)
{
	cred_danube.LoadK( cred_danube_, WHITE );
	cred_rez.LoadK( cred_rez_, WHITE );
	cred_fond.Load( cred_fond_ );
	cred_mars.LoadK( cred_mars_, WHITE );
	trash8a.Load( trash8a_ );
	trash8b.Load( trash8b_ );
	trash8c.Load( trash8c_ );
	credits.LoadK( credits_, PURPLE );
}


void e0900(void)
{
	mgCls();
	mgDrawTex( 0, 95, 640, 325, cred_fond, 20+(mgTime%400)/20, (mgTime%300)/15, 640, 325 );
	mgDrawTex( 0, 60, credits );


	mgDrawTex( 390+shake(2), 129+shake(2), cred_mars );
	mgDrawTex( 364+shake(2), 248+shake(2), cred_rez );
	mgDrawTex( 224+shake(2), 318+shake(2), cred_danube );

	int tra=mgTime%240;

	if (tra>160)
	{
		mgDrawTex( 281, 60, trash8a, 5+shake(5) );
		mgDrawTex( 281, 180, trash8b, 5+shake(5) );
		mgDrawTex( 281, 300, trash8c, 5+shake(5) );
	}
	else if (tra>80)
	{
		mgDrawTex( 281, 60, trash8b, 5+shake(5) );
		mgDrawTex( 281, 180, trash8c, 5+shake(5) );
		mgDrawTex( 281, 300, trash8a, 5+shake(5) );
	}
	else
	{
		mgDrawTex( 281, 60, trash8c, 5+shake(10) );
		mgDrawTex( 281, 180, trash8a, 5+shake(10) );
		mgDrawTex( 281, 300, trash8b, 5+shake(10) );
	}


		if(mgTime<1000)
	{
		int flash=255-(255*mgTime)/1000;
		mgCls( flash, WHITE );
	}
}



void e09_CLOSE(void)
{
	cred_danube.Unbind();
	cred_mars.Unbind();
	cred_rez.Unbind();
	cred_fond.Unbind();
	credits.Unbind();
	trash8a.Unbind();
	trash8b.Unbind();
	trash8c.Unbind();
}


//---------------------------------------------------------------------------------------------
//BOOH
//---------------------------------------------------------------------------------------------
mgTexture gruh1;
mgTexture gruh2;
mgTexture booh;
mgTexture trash6a;
mgTexture trash6b;
mgTexture trash6c;

const uint boohft = 2900;


void boohf_OPEN(void)
{
	gruh1.LoadK( gruh1_, WHITE );
	gruh2.LoadK( gruh2_, WHITE );
	booh.Load( booh_ );
	trash6a.Load( trash6a_ );
	trash6b.Load( trash6b_ );
	trash6c.Load( trash6c_ );
}



void boohf(void)
{
	mgCls();
	mgDrawTex( 0, 60, booh );


	int tra=mgTime%240;
	if (tra>160)
		mgDrawTex( 0, 265+shake(5), trash6a, 10+shake(10) );
	else if (tra>80)
		mgDrawTex( 0, 265+shake(5), trash6b, 10+shake(10) );
	else
		mgDrawTex( 0, 265+shake(5), trash6c, 10+shake(10) );


	int xr=27+shake(6), yr=67+shake(6);
	mgDrawTex( xr, yr, gruh2 );
	mgDrawTex(xr+4,yr-2, gruh1 );
}


void boohf_CLOSE(void)
{
	gruh1.Unbind();
	gruh2.Unbind();
	booh.Unbind();
	trash6a.Unbind();
	trash6b.Unbind();
	trash6c.Unbind();
}


//---------------------------------------------------------------------------------------------
//FRA PART
//---------------------------------------------------------------------------------------------
mgTexture fra_fond;
mgTexture fra_layer;

const uint frapartt = 5450;

void frapart_OPEN(void)
{
	fra_fond.Load( fra_fond_ );
	fra_layer.Load( fra_layer_ );
}



void frapart(void)
{
	mgDrawTex( 0, 0, fra_fond );

	int yl1=60-(240*mgTime)/frapartt;
	int yl2=yl1+300;

	mgDrawTex( 0, yl1, fra_layer );
	mgDrawTex( 10, yl2, fra_layer );

	mgCls( 0, 0, 377, 60 );
	mgCls( 0, 420, 377, 60 );


	if(mgTime<500)
	{
		int flash=255-(255*mgTime)/500;
		mgCls( flash, WHITE );
	}
}



void frapart_CLOSE(void)
{
	fra_fond.Unbind();
	fra_layer.Unbind();
}


//---------------------------------------------------------------------------------------------
//GUESTS GFX
//---------------------------------------------------------------------------------------------
mgTexture guests;
mgTexture guests_acetone;
mgTexture guests_ak;
mgTexture guests_fond;
mgTexture guests_splif;
mgTexture guests_fra;

const uint e12t = 2850;

void e12_OPEN(void)
{
	guests.LoadK( guests_, PURPLE );
	guests_acetone.LoadK( guests_acetone_, WHITE );
	guests_ak.LoadK( guests_ak_, WHITE );
	guests_splif.LoadK( guests_splif_, WHITE );
	guests_fra.LoadK( guest_fra_, WHITE );
	guests_fond.LoadK( guests_fond_, WHITE );
}


void e1200(void)
{
	mgCls();
	mgDrawTex( 0, 95, 640, 325, guests_fond, (mgTime%400)/20, (mgTime%300)/15, 640, 325 );
	mgDrawTex(0, 60, guests);

	mgDrawTex( 177+shake(2), 101+shake(2), guests_acetone );
	mgDrawTex( 80+shake(2), 158+shake(2), guests_splif );
	mgDrawTex( 213+shake(2), 215+shake(2), guests_ak );
	mgDrawTex( 432+shake(2), 282+shake(2), guests_fra );

	if(mgTime<500)
	{
		int flash=255-(255*mgTime)/500;
		mgCls( flash, WHITE);
	}

	if(mgTime>e12t-1500)
		mgFade( 255-(255*(mgTime-e12t+1500))/1500 );
}



void e12_CLOSE(void)
{
	guests.Unbind();
	guests_acetone.Unbind();
	guests_ak.Unbind();
	guests_fond.Unbind();
	guests_splif.Unbind();
	guests_fra.Unbind();
}


//---------------------------------------------------------------------------------------------
//TETONNE
//---------------------------------------------------------------------------------------------
mgTexture tetonne;
mgTexture tetonne_fond;
mgTexture tetonne_texte;
mgTexture trash7a;
mgTexture trash7b;
mgTexture trash7c;

const uint tetonneft = 4140;

void tetonnef_OPEN(void)
{
	tetonne.LoadK( tetonne_, PURPLE );
	tetonne_fond.LoadF( tetonne_fond_, false );
	tetonne_texte.LoadK( tetonne_texte_, PURPLE );
	trash7a.Load( trash7a_ );
	trash7b.Load( trash7b_ );
	trash7c.Load( trash7c_ );
	trash8a.Load( trash8a_ );
	trash8b.Load( trash8b_ );
	trash8c.Load( trash8c_ );
}



void tetonnef(void)
{
	mgCls();

	int xc=0-(168*mgTime/10.1f)/tetonneft;

	mgDrawTex( 10.1f*xc, 60, 808, 360, tetonne_fond, 0, 0, 80, 90 );
	mgTV(0,60,640,360);


	mgDrawTex( 0, 82, tetonne );
	mgDrawTex( 224+shake(4), 122+shake(4), tetonne_texte );


	int tra=mgTime%240;

	if (tra>160)
	{
		mgDrawTex( 0, 60, trash7a, 5+shake(10) );
		mgDrawTex( 0, 180, trash7b, 5+shake(10) );
		mgDrawTex( 0, 300, trash7c, 5+shake(10) );

		mgDrawTex( 281, 60, trash8a, 5+shake(10) );
		mgDrawTex( 281, 180, trash8b, 5+shake(10) );
		mgDrawTex( 281, 300, trash8c, 5+shake(10) );
	}
	else if (tra>80)
	{
		mgDrawTex( 0, 60, trash7b, 5+shake(10) );
		mgDrawTex( 0, 180, trash7c, 5+shake(10) );
		mgDrawTex( 0, 300, trash7a, 5+shake(10) );

		mgDrawTex( 281, 60, trash8b, 5+shake(10) );
		mgDrawTex( 281, 180, trash8c, 5+shake(10) );
		mgDrawTex( 281, 300, trash8a, 5+shake(10) );
	}
	else
	{
		mgDrawTex( 0, 60, trash7c, 5+shake(10) );
		mgDrawTex( 0, 180, trash7a, 5+shake(10) );
		mgDrawTex( 0, 300, trash7b, 5+shake(10) );

		mgDrawTex( 281, 60, trash8c, 5+shake(10) );
		mgDrawTex( 281, 180, trash8a,5+shake(10) );
		mgDrawTex( 281, 300, trash8b, 5+shake(10) );
	}



	if(mgTime<500)
		mgFade((255*mgTime)/500);
	else if(mgTime>tetonneft-500)
		mgFade(255-(255*(mgTime-tetonneft+500))/500);
}



void tetonnef_CLOSE(void)
{
	tetonne.Unbind();
	tetonne_fond.Unbind();
	tetonne_texte.Unbind();
	trash7a.Unbind();
	trash7b.Unbind();
	trash7c.Unbind();
	trash8a.Unbind();
	trash8b.Unbind();
	trash8c.Unbind();
}


//---------------------------------------------------------------------------------------------
//HANDS-UP 1
//---------------------------------------------------------------------------------------------
mgTexture handsup;
mgTexture handsup_fond1;
mgTexture handsup_fond2;
mgTexture handsup_fond3;

const uint handsupf1t = 5010;


void handsupf1_OPEN(void)
{
	handsup.LoadK( handsup_, PURPLE );
	handsup_fond1.LoadK( handsup_fond1_, PURPLE );
	handsup_fond2.LoadK( handsup_fond2_, PURPLE );
	handsup_fond3.LoadK( handsup_fond3_, PURPLE );
}



void handsupf1(void)
{
	mgCls();

//	static int x1,x2,x3;
	int xhu=(243*mgTime)/handsupf1t;
//	if (xhu<0)
//	{xhu=0;}
//	else
//	{
		int x3=( 66*mgTime)/handsupf1t;
		int x2=(125*mgTime)/handsupf1t;
		int x1=(184*mgTime)/handsupf1t;
//	}


	mgDrawTex( 0, 60, 640, 360, handsup_fond3, x3, 0, 640, 360 );
	mgDrawTex( 0, 60, 640, 360, handsup_fond2, x2, 0, 640, 360 );
	mgDrawTex( 0, 60, 640, 360, handsup_fond1, x1, 0, 640, 360 );

	int opa2=max(0, (255*((int)handsupf1t-(int)mgTime))/750);
	if(opa2<255)
		mgDrawTex( 0, 114, 640, 306, handsup, xhu, 0, 640, 306, opa2 );
	else
		mgDrawTex( 0, 114, 640, 306, handsup, xhu, 0, 640, 306 );

	int opa=(255*mgTime)/750;
	if(opa<255)
		mgFade(opa);
}



void handsupf1_CLOSE(void)
{
	handsup.Unbind();
}



//---------------------------------------------------------------------------------------------
//HANDS-UP 2
//---------------------------------------------------------------------------------------------
mgTexture handsup2;

const uint handsupf2t = 5010;


void handsupf2_OPEN(void)
{
	handsup2.LoadK( handsup2_, PURPLE );
}



void handsupf2(void)
{
	mgCls();

	int xhu=232-(232*mgTime)/handsupf2t;
	int x3=66-(66*mgTime)/handsupf2t;
	int x2=125-(125*mgTime)/handsupf2t;
	int x1=184-(184*mgTime)/handsupf2t;

	mgDrawTex( 0, 60, 640, 360, handsup_fond3, x3, 0, 640, 360 );
	mgDrawTex( 0, 60, 640, 360, handsup_fond2, x2, 0, 640, 360 );
	mgDrawTex( 0, 60, 640, 360, handsup_fond1, x1, 0, 640, 360 );

	int opa2=(255*mgTime)/750;
//	if(opa2<255)
//		mgDrawTex( 0, 60, 640, 360, handsup2, xhu, 0, 640, 360, opa2 );
//	else
		mgDrawTex( 0, 60, 640, 360, handsup2, xhu, 0, 640, 360 );

	sint opa=max(0, (255*((int)handsupf2t-(int)mgTime))/750);
	if(opa<255)
		mgFade(opa);
}



void handsupf2_CLOSE(void)
{
	handsup2.Unbind();
	handsup_fond1.Unbind();
	handsup_fond2.Unbind();
	handsup_fond3.Unbind();
}


//---------------------------------------------------------------------------------------------
//LA BASTON: SHEEP DEFENDER vs POULETZ
//---------------------------------------------------------------------------------------------
mgTexture fight01;
mgTexture fight02;
mgTexture fight03;
mgTexture netoile;
mgTexture spirale;
mgTexture traitsbleus;
mgTexture sheepdef;
mgTexture vs;
mgTexture evilpoulet;
mgTexture combo;
mgTexture ko;

const uint e10t[] = {3034, 3034, 3034 };


void e10_OPEN(void)
{
	fight01.LoadK( fight01_, PURPLE );
	fight02.LoadK( fight02_, PURPLE );
	fight03.LoadK( fight03_, PURPLE );
	netoile.Load( netoile_ );
	spirale.Load( spirale_ );
	traitsbleus.Load( traitsbleus_ );
	sheepdef.LoadK( sheepdef_, BLACK );
	vs.LoadK( vs_, BLACK );
	evilpoulet.LoadK( evilpoulet_, BLACK );
	combo.LoadK( combo_, BLACK );
	ko.LoadK( ko_, 0x00008AFF );
}


void e1010(void)
{
	int xf = mgTime/2;
	mgMosaic( xf, 60, traitsbleus);
	mgDrawTex( 0, 105, fight01 );
	mgCinemaScope();

	if(mgTime>e10t[0]/5)
		mgDrawTex( 10, 120, sheepdef );
	if(mgTime>2*e10t[0]/5)
		mgDrawTex(250,280, vs );
	if(mgTime>3*e10t[0]/5)
		mgDrawTex(345,330, evilpoulet );


	if(mgTime<500)
	{
		int flash=255-(255*mgTime)/500;
		mgCls( flash, WHITE );
	}
}


void e1020(void)
{
	int yf = (2*mgTime)/5;
	mgMosaic( 0, -yf, spirale );
	mgDrawTex( 0, 60, fight02 );
	mgCinemaScope();

	if(mgTime>e10t[1]/4 && mgTime%600 > 200)
		mgDrawTex( 465, 10, combo );


	if(mgTime<500)
	{
		int flash=255-(255*mgTime)/500;
		mgCls( flash, WHITE);
	}
}


void e1030(void)
{
	int pf = mgTime/2;
	mgMosaic( -pf, -pf, netoile );
	mgDrawTex( 0, 60, fight03 );
	mgCinemaScope();

	if(mgTime>e10t[2]/4 && mgTime%600 > 200)
		mgDrawTex( 7, 270, ko );


	if(mgTime<500)
	{
		int flash=255-(255*mgTime)/500;
		mgCls( flash, WHITE);
	}
}


void e10_CLOSE(void)
{
	fight01.Unbind();
	fight02.Unbind();
	fight03.Unbind();
	netoile.Unbind();
	spirale.Unbind();
	traitsbleus.Unbind();
	sheepdef.Unbind();
	vs.Unbind();
	evilpoulet.Unbind();
	combo.Unbind();
	ko.Unbind();
}



//---------------------------------------------------------------------------------------------
//BIRTH
//---------------------------------------------------------------------------------------------
mgTexture birth_fond;
mgTexture poulet_acet1;
mgTexture timast;
mgTexture trash11a;
mgTexture trash11b;
mgTexture trash12a;
mgTexture trash12b;

const uint birtht = 10175;

void birth_OPEN(void)
{
	timast.LoadK( timast_, PURPLE );
	birth_fond.LoadF( birth_fond_, false );
	poulet_acet1.LoadK( poulet_acet1_, PURPLE );
	trash11a.Load( trash11a_ );
	trash11b.Load( trash11b_ );
	trash12a.Load( trash12a_ );
	trash12b.Load( trash12b_ );
}



void birth(void)
{

	//clsrect(0,60,640,360,noir);
	const int wf=800;
	int wc=(640+wf)/2+(int)(((wf-640)/2)*cos((11*mgPi*mgTime)/birtht));
	int hc=(wc*360)/640;
	int xc=(640-wc)/2, yc=(480-hc)/2;

	mgDrawTex( xc, yc, wc, hc, birth_fond, 0, 0, 80, 45 );

	mgCinemaScope();

	const unsigned int cp1=birtht/8;
	const unsigned int cp3=(3*birtht)/9;
	const unsigned int cp5=(5*birtht)/9;
	const unsigned int cp7=(7*birtht)/9;
	const unsigned int cp8=(8*birtht)/9;

	bool tra = jitter();

	if( mgTime < cp1 )
	{
		mgDrawTex( 0, 60, timast, (255*mgTime)/cp1 );
		if(tra)
			mgDrawTex( 398, 60, trash11a, (35*mgTime)/cp1 );
		else
			mgDrawTex( 398, 60, trash11b, (35*mgTime)/cp1 );
	}

	
	else if( mgTime < cp3 )
	{
		mgDrawTex( 0, 60, timast );

		if(tra)
			mgDrawTex( 398, 60, trash11a, 35+shake(15) );
		else
			mgDrawTex( 398, 60, trash11b, 35+shake(15) );
	}



	else if( mgTime < cp5 )
	{
		mgDrawTex( 0, 60, timast, 255-(255*(mgTime-cp3))/(cp5-cp3) );
		mgDrawTex( 395, 60, poulet_acet1, (255*(mgTime-cp3))/(cp5-cp3) );

		if(tra)
		{
			mgDrawTex( 398, 60, trash11a, 35-(35*(mgTime-cp3))/(cp5-cp3) );
			mgDrawTex( 0, 60, trash12a, (25*(mgTime-cp3))/(cp5-cp3) );
		}
		else
		{
			mgDrawTex( 398, 60, trash11b, 35-(35*(mgTime-cp3))/(cp5-cp3) );
			mgDrawTex( 0, 60, trash12b, (25*(mgTime-cp3))/(cp5-cp3) );
		}
	}



	else if ( mgTime < cp7 )
	{
		mgDrawTex( 395, 60, poulet_acet1 );

		if(tra)
			mgDrawTex( 0, 60, trash12a, 20+shake(15) );
		else
			mgDrawTex( 0, 60, trash12b, 20+shake(15) );
	}




	else if ( mgTime < cp8 )
	{
		mgDrawTex( 395, 60, poulet_acet1, 255-(255*(mgTime-cp7))/(cp8-cp7) );
		if(tra)
			mgDrawTex( 0, 60, trash12a, 25-(25*(mgTime-cp7))/(cp8-cp7) );
		else
			mgDrawTex( 0, 60, trash12b, 25-(25*(mgTime-cp7))/(cp8-cp7) );
	}


	if(mgTime>birtht-1500)
		mgFade( 255-(255*(mgTime-birtht+1500))/1500 );
}



void birth_CLOSE(void)
{
	timast.Unbind();
	birth_fond.Unbind();
	poulet_acet1.Unbind();
	trash11a.Unbind();
	trash11b.Unbind();
	trash12a.Unbind();
	trash12b.Unbind();
}


//---------------------------------------------------------------------------------------------
//VILLE MASTERIZED + GREETZ
//---------------------------------------------------------------------------------------------
mgTexture city;
mgTexture master;

mgTexture bomb;
mgTexture calodox;
mgTexture condense;
mgTexture dim4;
mgTexture digitalmurder;
mgTexture eclipse;
mgTexture equinox;
mgTexture etron;
mgTexture logofactory;
mgTexture mandarine;
mgTexture move;
mgTexture ojuice;
mgTexture orion;
mgTexture pixelfr;
mgTexture popsyteam;
mgTexture purple;
mgTexture threestate;
mgTexture tpolm;

const uint e14t = 10200;

void e14_OPEN(void)
{
	city.LoadK( city_, PURPLE );
	master.LoadK( master_, PURPLE );
	villefond.LoadF( villefond_, true );

	bomb.LoadK( bomb_, 0x674F4F );
	calodox.LoadK( calodox_, 0x3352B8 );
	condense.LoadK( condense_, 0x674F4F );
	dim4.LoadK( dim4_, 0x3352B8 );
	digitalmurder.LoadK( digitalmurder_, 0x2C4895 );
	eclipse.LoadK( eclipse_, 0x674F4F );
	etron.LoadK( etron_, 0x674F4F );
	equinox.LoadK( equinox_, 0x3352B8 );
	logofactory.LoadK( logofactory_, 0x674F4F );
	mandarine.LoadK( mandarine_, 0x674F4F );
	move.LoadK( move_, 0x674F4F );
	ojuice.LoadK( ojuice_, 0x303441 );
	orion.LoadK( orion_, 0x303441 );
	pixelfr.LoadK( pixelfr_, 0x303441 );
	popsyteam.LoadK( popsyteam_, 0x2C4895 );
	purple.LoadK( purple_, 0x3352B8 );
	threestate.LoadK( threestate_, 0x303441 );
	tpolm.LoadK( tpolm_, 0x2C4895 );
}



void e1410(void)
{
	const int arrivee=4000;
	int vfx=(mgTime/30)%800;
	mgCls();
	mgDrawTex( 0-vfx, 60, 800, 360, villefond, 0, 0, 800, 360 );
	mgDrawTex( 800-vfx, 60, 800, 360, villefond, 0, 0, 800, 360 );
	
	int ctx=(641*mgTime)/e14t;
	mgDrawTex( 0, 148, 640, 272, city, ctx, 0, 640, 272 );
	
	int msx;
	if (mgTime>arrivee)
	{
		//msx=290+(int)(350*(1-sqrtf((float)(mgTime-e14t+6000)/(float)2000)));
		msx=640-((640+322)*(mgTime-arrivee))/(e14t-arrivee-1000);
		//if (msx<290) msx=290;
		mgDrawTex( msx, 81, master );
	}
	else
		msx=9999;
	
	
	mgTexture greetsg1[10]={dim4, mandarine, eclipse, orion, digitalmurder, calodox, move, popsyteam, bomb, ojuice};
	mgTexture greetsg2[8]={equinox, threestate, pixelfr, condense, tpolm, purple, etron, logofactory};
	static int blink1[10][4]={{450,150,500,0},{440,330,700,0},{370,250,900,0},{340,370,1100,0},{300,100,1300,0},{200,200,1500,0},{220,310,1700,0},{100,100,1900,0},{100,230,2100,0},{50,350,2300,0}};
	static int blink2[8][4]={{450,150,e14t-500,0},{433,290,e14t-700,0},{450,380,e14t-900,0},{250,370,e14t-1100,0},{250,100,e14t-1300,0},{120,200,e14t-1500,0},{100,310,e14t-1700,0}, {275,250,e14t-1200,0}};
	
	if(mgTime >= e14t-1000 )
	{
		for (int gind2=0; gind2<8; gind2++)
			if( mgFrame%( 
//1+(mgTime-e14t+1000)/170
						2 ) == 0)
				mgDrawTex( blink2[gind2][0], blink2[gind2][1], greetsg2[gind2] );
			
			mgFade( 255-(255*(mgTime-e14t+1000))/1000 );
	}
	else if ( mgTime < e14t-1000)
	{
		
		for (int gind1=0; gind1<10; gind1++)
		{
			if( blink1[gind1][3] == 1 )
				mgDrawTex( blink1[gind1][0] - 600*(mgTime-blink1[gind1][2])/500, blink1[gind1][1], greetsg1[gind1] );
			
			else if (msx <= (signed)(blink1[gind1][0] + greetsg1[gind1].Width + 80))
			{
				blink1[gind1][2]=mgTime;
				blink1[gind1][3]=1;
			}
			
			else if( mgTime > (unsigned)blink1[gind1][2] && mgTime < (unsigned)blink1[gind1][2]+500 )
			{
				if( mgFrame%( 1+(500-mgTime+blink1[gind1][2])/170 ) == 0)
				{
					mgDrawTex( blink1[gind1][0], blink1[gind1][1], greetsg1[gind1] );
				}
			}
			
			else if (mgTime >= (unsigned)blink1[gind1][2]+500)
				mgDrawTex( blink1[gind1][0], blink1[gind1][1], greetsg1[gind1] );
		}
		
		
		
		
		for (int gind2=0; gind2<8; gind2++)
		{
			
			if (blink2[gind2][3] != 0)
			{
				int gtx=640-((640-blink2[gind2][0])*(mgTime-blink2[gind2][3]))/500;
				if( gtx < blink2[gind2][0] )
					gtx=blink2[gind2][0];
				
				mgDrawTex( gtx, blink2[gind2][1], greetsg2[gind2] );
			}
			
			
			else if( msx+322+5 <= blink2[gind2][0] )
				blink2[gind2][3]=mgTime;
		}
	}
}



void e14_CLOSE(void)
{
	city.Unbind();
	master.Unbind();
	villefond.Unbind();
	bomb.Unbind();
	calodox.Unbind();
	condense.Unbind();
	dim4.Unbind();
	digitalmurder.Unbind();
	eclipse.Unbind();
	etron.Unbind();
	equinox.Unbind();
	logofactory.Unbind();
	mandarine.Unbind();
	move.Unbind();
	ojuice.Unbind();
	orion.Unbind();
	pixelfr.Unbind();
	popsyteam.Unbind();
	purple.Unbind();
	threestate.Unbind();
	tpolm.Unbind();
}


//---------------------------------------------------------------------------------------------
//POULETZ REVELATION
//---------------------------------------------------------------------------------------------
mgTexture reveal;
mgTexture revealfond;
mgTexture toostrong;
mgTexture upw;
mgTexture upwfond;
mgTexture dontresist;

mgMesh nihon;

const uint e11t[] = { 5520, 2200 };

void e11_OPEN(void)
{
	reveal.LoadK( reveal_, PURPLE );
	revealfond.Load( revealfond_ );
	dontresist.LoadK( dontresist_, PURPLE );
	toostrong.LoadK( toostrong_, PURPLE );
	upw.LoadK( ultimatepouletwarrior_, PURPLE );
	upwfond.Load( upwfond_ );
	trash10a.Load( trash10a_ );
	trash10b.Load( trash10b_ );
	trash10c.Load( trash10c_ );
}

void e1110(void)
{
	int fy=mgTime/3;
	mgMosaic( 0, -fy, revealfond );

	int ry=-275+(695*mgTime)/e11t[0];
//	if(ry>70) ry=70;
	mgDrawTex( 50, ry, reveal );
	mgDrawTex( 0, 60, dontresist );
	mgCinemaScope();

	int tra=mgTime%240;

	if (tra>160)
		mgDrawTex( 0, 60, trash10a, 15+shake(8) );
	else if (tra>80)
		mgDrawTex( 0, 60, trash10b, 15+shake(8) );
	else
		mgDrawTex( 0, 60, trash10c, 15+shake(8) );


//	int opa=(255*mgTime)/750;
//	if(opa>255) opa=255;
//	dIf(lpDDSBack,0,0,opa);
}



void e1120(void)
{
	mgCls();
	float alpha = (float)mgTime/15.0;
	double beta = DEG_TO_RAD( alpha );
	float x = 370.f - 320.f*cos(beta) + 240.f*sin(beta);
	float y = 370.f - 320.f*sin(beta) - 240.f*cos(beta);

	mgRasterTransform( x, y, 640, 480, alpha );

	mgDrawTex( 0, 0, 740, 740, upwfond );
	mgRasterMode();

	mgCinemaScope();
	mgDrawTex( 171, 76, upw );
	mgDrawTex( 310, 275, toostrong );


	if(mgTime<500)
	{
		int flash=255-(255*mgTime)/500;
		mgCls( flash, WHITE );
	}

	if(mgTime>e11t[1]-1000)
		mgFade( 255-(255*(mgTime-e11t[1]+1000))/1000 );

 }


void e11_CLOSE(void)
{
	reveal.Unbind();
	revealfond.Unbind();
	dontresist.Unbind();
	toostrong.Unbind();
	upw.Unbind();
	upwfond.Unbind();
	trash10a.Unbind();
	trash10b.Unbind();
	trash10c.Unbind();
}


//---------------------------------------------------------------------------------------------
//STREETO
//---------------------------------------------------------------------------------------------
mgTexture strciel;
mgTexture strcocha;
mgTexture streyes;
mgTexture strmurs;
mgTexture strstreum;
mgTexture strtoits;
mgTexture trash2;
mgTexture trash2b;

const uint e13t[] = { 6800, 1910 };


void e13_OPEN(void)
{
	strciel.Load( streeto_ciel_ );
	strcocha.LoadK( streeto_cocha_, PURPLE );
	streyes.Load( streeto_eyes_ );

	strmurs.LoadK( streeto_murs_, PURPLE );
	strstreum.LoadK( streeto_streum_, PURPLE );
	strtoits.LoadK( streeto_toits_, PURPLE );
	trash2.Load( trash2_ );
	trash2b.Load( trash2b_ );
}


void e1311(void)
{
	int xciel=50-(50*mgTime)/1000;
	//int xtoits=;
	int ystreum=440-((int)sqrt((90000*(mgTime-600))/5500));
	const int ytoits=90;
	int xcocha=69-(45*mgTime)/5500+mgTime%2;

	mgDrawTex( xciel, 60, strciel );
	mgDrawTex( xciel+500, 60, strciel );

	mgDrawTex( 120, ytoits, strtoits );
	mgCls( 0, ytoits+120, 640, 420-120-ytoits );

	mgDrawTex( -30, 60, strmurs );


	int tra=mgTime%240;
	if(tra>120)
		mgDrawTex( 391, 60, trash2, 5+shake(10) );
	else	
		mgDrawTex( 391, 60, trash2b, 5+shake(10) );

	
	if( mgTime > e13t[0]-700 )
	{
		mgDrawTex( 0, 140, strstreum );
		mgCls( 0, 140+119, 640, 420 );
	}
	else if( mgTime > 600 )
	{
		mgDrawTex( 0, ystreum, strstreum );
		mgCls( 0, ystreum+119, 640, 420 );
	}

	mgCinemaScope();
	mgDrawTex( xcocha, 165, strcocha );

	if( mgTime < 600 )
	mgFade( (255*mgTime)/600 );	

	if( mgTime > e13t[0]-700 )
		mgFade( (255*(e13t[0]-mgTime))/700 );


}


void e1320(void)
{
	mgCls();
	mgDrawTex( shake(4), 60+shake(4), streyes );
}


void e13_CLOSE(void)
{
	strciel.Unbind();
	streyes.Unbind();
	strstreum.Unbind();
	strcocha.Unbind();
	strmurs.Unbind();
	strtoits.Unbind();
	trash2.Unbind();
	trash2b.Unbind();
}



//---------------------------------------------------------------------------------------------
//MUTATION HAS NOW STARTED
//---------------------------------------------------------------------------------------------
mgTexture brume01;
mgTexture brume02;
mgTexture brume03;
mgTexture brume04;
mgTexture brume05;
mgTexture mutachat;
mgTexture mutapig;
mgTexture ruelle03;
mgTexture mutation2;
mgTexture hasnow;
mgTexture started;

const uint startedt = 7680;

void started_OPEN(void)
{
	brume01.LoadK( brume01_, PURPLE);
	brume02.LoadK( brume02_, PURPLE);
	brume03.LoadK( brume03_, PURPLE);
	brume04.LoadK( brume04_, PURPLE);
	brume05.LoadK( brume05_, PURPLE);
	mutachat.LoadK( mutachat_, PURPLE);
	mutapig.LoadK( mutapig_, PURPLE);
	ruelle03.Load( ruelle03_ );
	mutation2.LoadK( mutation2_, BLACK );
	hasnow.LoadK( hasnow_, BLACK );
	started.LoadK( started_, BLACK );
}


void e1330(void)
{
	if( mgTime > startedt )
		return;

	const int opa = mgTime%4-2+150-(150*mgTime)/startedt;
	const int xb1 = mgTime%4-2+0-(565*mgTime)/startedt;
	const int xb2 = mgTime%4-2+188+(452*mgTime)/startedt;
	const int xb3 = mgTime%4-2+0-(363*mgTime)/startedt;
	const int xb4 = mgTime%4-2+154+(486*mgTime)/startedt;
	const int yb5 = mgTime%2-1+311+(109*mgTime)/startedt;

	mgDrawTex( 0, 60, ruelle03 );
	mgDrawTex( 345+(80*mgTime)/startedt, 89, mutapig );
	mgDrawTex( -5-(80*mgTime)/startedt, 104, mutachat );

	mgDrawTex( xb1, 137, brume01, opa );
	mgDrawTex( xb2, 175, brume02, opa );
	mgDrawTex( xb3, 60, brume03, opa );
	mgDrawTex( xb4, 60, brume04, opa );
	mgDrawTex( 95, yb5, brume05, opa );

//	int xs = 240+(int)(5*cos((9*mgPi*mgTime)/startedt))+mgTime%4-2;
//	int ys = 51+(int)(5*sin((7.5*mgPi*mgTime)/startedt))+mgTime%4-2;
//	mgDrawTex( xs, ys, started );	


	int fade;
	if( mgTime < startedt/5 )
	{
		fade = (255*mgTime)/(startedt/5);
		mgDrawTex( 0, 60, mutation2, fade );
	}
	else if( mgTime < 2*startedt/5 )
	{
		fade=(255*(mgTime-startedt/5))/(startedt/5);
		mgDrawTex( 0, 60, mutation2, 255-fade );
		mgDrawTex( 220, 60, hasnow, fade );
	}
	else if( mgTime < 3*startedt/5 )
	{
		fade=(255*(mgTime-2*startedt/5))/(startedt/5);
		mgDrawTex( 220, 60, hasnow, 255-fade );
		mgDrawTex( 464, 60, started, fade );
	}
	else if( mgTime < 4*startedt/5 )
	{
		fade=(255*(mgTime-3*startedt/5))/(startedt/5);
		mgDrawTex( 464, 60, started, 255-fade );
	}
	mgCinemaScope();
}


void started_CLOSE(void)
{
	brume01.Unbind();
	brume02.Unbind();
	brume03.Unbind();
	brume04.Unbind();
	brume05.Unbind();
	mutapig.Unbind();
	mutachat.Unbind();
	ruelle03.Unbind();
	mutation2.Unbind();
	hasnow.Unbind();
	started.Unbind();
}


//---------------------------------------------------------------------------------------------
//GENERIQUE
//---------------------------------------------------------------------------------------------
mgTexture gene[9];
mgTexture genefond;

const uint e15t = 1416*8;	//*9

void e15_OPEN(void)
{
	gene[0].Load( ge1_ );
	gene[1].Load( ge2_ );
	gene[2].Load( ge3_ );
	gene[3].Load( ge4_ );
	gene[4].Load( ge5_ );
	gene[5].Load( ge6_ );
	gene[6].Load( generique_fra_ );
	gene[7].Load( ge7_ );
//	gene[8].Load( ge8_ );
	genefond.Load( genefond_ );
}


void e1500(void)
{
	mgCls();
	int idx = mgTime*8/e15t; //mgTime*9/e15t;
	mgDrawTex( 97, 62, genefond );
	mgDrawTex( 126, 220, gene[idx] );
}



void e15_CLOSE(void)
{
	gene[0].Unbind();
	gene[1].Unbind();
	gene[2].Unbind();
	gene[3].Unbind();
	gene[4].Unbind();
	gene[5].Unbind();
	gene[6].Unbind();
	gene[7].Unbind();
	gene[8].Unbind();
	genefond.Unbind();
}

//---------------------------------------------------------------------------------------------
//GNIII !!!!
//---------------------------------------------------------------------------------------------
mgTexture main1;
mgTexture main2;
mgTexture gniii_tete;
mgTexture gni_fond;

const uint e16t = 1200;

void e16_OPEN(void)
{
	main1.LoadK( main1_, PURPLE );
	main2.LoadK( main2_, PURPLE );
	gniii_tete.LoadK( gniii_tete_, PURPLE );
	gni_fond.Load( gni_fond_ );
}

void e1600(void)
{
	mgCls();

	int sk=shake(10)-5;
	mgDrawTex( sk, 60, gni_fond );
	mgDrawTex( 150, 60, gniii_tete );

	sk=shake(10)-5;
	mgDrawTex( 130+sk, 301, main1 );
	mgDrawTex( 295+sk, 304, main2 );
}



void e16_CLOSE(void)
{
	main1.Unbind();
	main2.Unbind();
	gniii_tete.Unbind();
	gni_fond.Unbind();
}



//-----------------------------------------------------------
//-----------------------------------------------------------


void e00_OPEN(void)
{
	mgRasterMode();
	mgClearColor(RED);
//	mgSoundInit(MG___); NO!
	mgSoundLoadModule( "mp2rmx.xm" );
	mgSoundPlay();
}


//-----------------------------------------------------------
//EFFECTS' LIST
//-----------------------------------------------------------
mgEffect CurEffects[]={{1000, -1, -1, mgEffectNull, mgEffectTrue, mgCls },
/*....BEGIN....*/


//{-1, -1, -1, e00_OPEN, mgEffectTrue, mgEffectNull },

{e01t[0], -1, -1, e01_OPEN, mgEffectTrue, e0101 },
{e01t[1], 0, 40, mgEffectNull, mgEffectTrue, e0102 },
{e01t[2], -1, -1, mgEffectNull, mgEffectTrue, e0122 },
{e01t[3], -1, -1, mgEffectNull, mgEffectTrue, e0125 },
{e01t[4], 2, 0, mgEffectNull, mgEffectTrue, e0140 },
{e01t[5], -1, -1, mgEffectNull, mgEffectTrue, e0141 },
{e01t[6], 2, 15, mgEffectNull, mgEffectTrue, e0142 },
{-1, -1, -1, e01_CLOSE, mgEffectTrue, mgEffectNull },

{e02t[0], 3, 8, e02_OPEN, mgEffectTrue, e0200 },
{e02t[1], 3, 16, mgEffectNull, mgEffectTrue, e0201 },
{e02t[2], 3, 24, mgEffectNull, mgEffectTrue, e0202 },
{e02t[3], 3, 32, mgEffectNull, mgEffectTrue, e0203 },
{e02t[4], -1, -1, mgEffectNull, mgEffectTrue, e0204 },
{e02t[5], 5, 0, mgEffectNull, mgEffectTrue, e0205 },
{-1, -1, -1, e02_CLOSE, mgEffectTrue, mgEffectNull },

{e03t[0], 5, 32, e03_OPEN, mgEffectTrue, e0300 },
{e03t[1], -1, -1, mgEffectNull, mgEffectTrue, e0301 },
{e03t[2], 7, 0, mgEffectNull, mgEffectTrue, e0302 },
{e03t[3], 8, 0, mgEffectNull, mgEffectTrue, e0303 },
{-1, -1, -1, e03_CLOSE, mgEffectTrue, mgEffectNull },

{explot, 9, 0, explo_OPEN, mgEffectTrue, explo },
{-1, -1, -1, explo_CLOSE, mgEffectTrue, mgEffectNull },

{e04t, 9, 42, e04_OPEN, mgEffectTrue, e0401 },
{-1, -1, -1, e04_CLOSE, mgEffectTrue, mgEffectNull },

{e05t, 12, 0, e05_OPEN, mgEffectTrue, e0500 },
{-1, -1, -1, e05_CLOSE, mgEffectTrue, mgEffectNull },

{e06t[0], -1, -1, e06_OPEN, mgEffectTrue, e0600 },
{e06t[1], -1, -1, mgEffectNull, mgEffectTrue, e0601 },
{e06t[2], -1, -1, mgEffectNull, mgEffectTrue, e0602 },
{e06t[3], -1, -1, mgEffectNull, mgEffectTrue, e0603 },
{e06t[4], -1, -1, mgEffectNull, mgEffectTrue, e0604 },
{e06t[5], -1, -1, mgEffectNull, mgEffectTrue, e0605 },
{e06t[6], -1, -1, mgEffectNull, mgEffectTrue, e0606 },
{e06t[7], -1, -1, mgEffectNull, mgEffectTrue, e0607 },
{e06t[8], 14, 0, mgEffectNull, mgEffectTrue, e0608 },
{-1, -1, -1, e06_CLOSE, mgEffectTrue, mgEffectNull },

{e08t[0], -1, -1, e08_OPEN, mgEffectTrue, e0800 },
{e08t[1], 15, 0, mgEffectNull, mgEffectTrue, e0801 },
{-1, -1, -1, e08_CLOSE, mgEffectTrue, mgEffectNull },

{e09t, 16, 0, e09_OPEN, mgEffectTrue, e0900 },
{-1, -1, -1, e09_CLOSE, mgEffectTrue, mgEffectNull },

{boohft, 16, 28, boohf_OPEN, mgEffectTrue, boohf },
{-1, -1, -1, boohf_CLOSE, mgEffectTrue, mgEffectNull },

{frapartt, 19, 0, frapart_OPEN, mgEffectTrue, frapart },
{-1, -1, -1, frapart_CLOSE, mgEffectTrue, mgEffectNull },

{e12t, 20, 0, e12_OPEN, mgEffectTrue, e1200 },
{-1, -1, -1, e12_CLOSE, mgEffectTrue, mgEffectNull },

{tetonneft, 20, 16, tetonnef_OPEN, mgEffectTrue, tetonnef },
{-1, -1, -1, tetonnef_CLOSE, mgEffectTrue, mgEffectNull },

{handsupf1t, 22, 0, handsupf1_OPEN, mgEffectTrue, handsupf1 },
{-1, -1, -1, handsupf1_CLOSE, mgEffectTrue, mgEffectNull },

{handsupf2t, 23, 0, handsupf2_OPEN, mgEffectTrue, handsupf2 },
{-1, -1, -1, handsupf2_CLOSE, mgEffectTrue, mgEffectNull },

{e10t[0], -1, -1, e10_OPEN, mgEffectTrue, e1010 },
{e10t[1], -1, -1, mgEffectNull, mgEffectTrue, e1020 },
{e10t[2], 25, 0, mgEffectNull, mgEffectTrue, e1030 },
{-1, -1, -1, e10_CLOSE, mgEffectTrue, mgEffectNull },

{birtht, 27, 0, birth_OPEN, mgEffectTrue, birth },
{-1, -1, -1, birth_CLOSE, mgEffectTrue, mgEffectNull },

{e14t, 29, 0, e14_OPEN, mgEffectTrue, e1410 },
{-1, -1, -1, e14_CLOSE, mgEffectTrue, mgEffectNull },

{e11t[0], -1, -1, e11_OPEN, mgEffectTrue, e1110 },
{e11t[1], 31, 0, mgEffectNull, mgEffectTrue, e1120 },
{-1, -1, -1, e11_CLOSE, mgEffectTrue, mgEffectNull },

{e13t[0], 32, 0, e13_OPEN, mgEffectTrue, e1311 },
{e13t[1], -1, -1, mgEffectNull, mgEffectTrue, e1320 },
{-1, -1, -1, e13_CLOSE, mgEffectTrue, mgEffectNull },

{startedt, 35, 0, started_OPEN, mgEffectTrue, e1330 },
{-1, -1, -1, started_CLOSE, mgEffectTrue, mgEffectNull },

{e15t, 36, 0, e15_OPEN, mgEffectTrue, e1500 },
{-1, -1, -1, e15_CLOSE, mgEffectTrue, mgEffectNull },

{e16t, -1, -1, e16_OPEN, mgEffectTrue, e1600 },
{-1, -1, -1, e16_CLOSE, mgEffectTrue, mgEffectNull },


/*....END....*/
{0,-1,-1,mgEffectNull,mgEffectFalse,NULL}};






//END
#endif 