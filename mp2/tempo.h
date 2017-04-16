//-------------------------------------------------------------------
//TEMPO.H - MISE AU POINT EXTERNE DES ENCHAINEMENTS
//-------------------------------------------------------------------
#ifndef _TEMPO_H_
#define _TEMPO_H_



int GetFirstInt(char** lpbuf)	//renvoie NULL et lpbuf devient NULL si il n'y avait rien à chercher
{
	int look1=0, look2=0;
	char* buf = *lpbuf;

	while(( buf[look1] < 0x30 || buf[look1] > 0x39 ) && buf[look1] != '\0' )	//vire les premiers caractères qui ne sont pas des chiffres
	{look1++;}
	
	while(( buf[look1+look2] >= 0x30 && buf[look1+look2] <= 0x39 ) && buf[look1+look2] != '\0' )	//va jusqu'au prochain caractère qui n'est pas un chiffre et le remplace par un '\0'
	{look2++;}

	if(look1 == look1+look2)	//il n'y a pas de chiffre dans cette chaîne
	{
		*lpbuf = NULL;
		return NULL;
	}




	if ( buf[look1+look2] == '\0' )	//on est arrivé à la fin de la chaîne
		*lpbuf = *lpbuf+look1+look2;	//casts pourris à cause des additions sur des adresses qui sont multipliées par 4 (1 int=4 oct)
	else
	{
		buf[look1+look2] = '\0';
		*lpbuf = *lpbuf+look1+look2+1;
	}

	return str2int( &(buf[look1]) );	//convertit le chiffre à partir de la copie du pointeur
}











int SetTempo(eff effets[])	//retourne l'effet de départ et modifie les structures d'effets en fonction de tempo.ini
{
	FILE* tempo;
	int eff_nb=0;
	int start_nb=0, start_ord, start_row;
	
	if ((tempo=fopen("tempo.ini","r")) != NULL)
	{
		
		char buf[1024];
		char* bufcp;
		int tmp1,tmp2;
		
		while(fgets(buf,1024,tempo) != NULL)
		{
			if (buf[0] != ';' && buf[0] != '\n')	//commentaire ou rien
			{
				bufcp = buf;						//copie de l'adresse seulement. on va chercher des chiffres dans cette ligne

				if (buf[0] == 'S')					//effet start
				{
					start_nb=GetFirstInt( &bufcp );
					start_ord=GetFirstInt( &bufcp );
					start_row=GetFirstInt( &bufcp );
				}
				else if (strcmp("MAXINT\n",buf) == 0)
				{
					effets[eff_nb].tm=MAXINT;
					effets[eff_nb-1].ord=-1;
					effets[eff_nb-1].row=-1;
					eff_nb++;
				}
				else
				{
					tmp1=GetFirstInt( &bufcp );
					tmp2=GetFirstInt( &bufcp );		//soit un chiffre soit NULL et buffpt NULL aussi

					if(bufcp != NULL)						//il y avait quelque chose à chercher
					{
						effets[eff_nb].ord=tmp1;
						effets[eff_nb].row=tmp2;
					}
					else
					{
						effets[eff_nb].tm=tmp1;
						effets[eff_nb].ord=-1;
						effets[eff_nb].row=-1;
					}
					
					eff_nb++;

				}
			}
		}
		
		fclose(tempo);
	}

	if (start_nb != 0)
	{
		effets[start_nb-1].ord=start_ord;
		effets[start_nb-1].row=start_row;
		return start_nb;
	}
	else
		return 1;
}






#endif