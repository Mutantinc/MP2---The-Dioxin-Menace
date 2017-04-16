//#include <windows.h>
#include <stdio.h>		//in/out functions
#include <stdlib.h>		//mem functions
#include <string.h>		//string functions
#include <io.h>			//file functions
#include <direct.h>		//dir functions
#include "../zlib/zlib.h"		//zlib functionnalities

int zlevel = 9;		//0 to 9
int filecount = 0;
int tempcount = 0;
int totalorigsize = 0;
int totalcompsize = 0;
FILE *zfile;
FILE *file;

//----------------------------------------------------------------------


char* nameslashname( char* name1, char* name2 )
{
	int len1 = strlen(name1);
	int len2 = strlen(name2);
	char* nsn = (char*)malloc( len1 + len2 + 2 );
	strcpy( nsn, name1 );
	nsn[len1] = '/';
	strcpy( nsn+strlen(name1)+1, name2 );
	nsn[len1 + len2 + 1] = '\0';
	return nsn;
}


//----------------------------------------------------------------------


char* forbidden2underscore( char* name )
{
	int p = -1;
	int l = strlen(name)+1;
	char* name_ = (char*)calloc( l, 1 );

	for( int i=0; i<l; i++)
	{
		if( name[i] == '.' || name[i] == '-' || name[i] == ' ')
		{
			p = i;
			name_[i] = '_';
		}
		else
			name_[i] = name[i];
	}

	if( p >= 0 )
		name_[p] = '\0';		//cut the extension

	return name_;
}


//----------------------------------------------------------------------

/*
char* give_ext( char* name )
{
	int p = 0;
	int l = strlen(name);

	for( int i=0; i<l; i++)
	{
		if( name[i] == '.' )
			p = i+1;
	}

	if( !p || l-p>3 )
		return name+l;
	else
		return name+p;

}
*/

//----------------------------------------------------------------------

void zliber_count( char* dir )
{
	struct _finddata_t specif;
	long handler = _findfirst( nameslashname( dir, "*.*"), &specif );
 
	if( handler != -1 )
	{
		while( _findnext( handler, &specif ) != -1 )
		{
			if( strcmp( specif.name,"..") && strcmp(specif.name,"."))
			{
				if( specif.attrib&_A_SUBDIR )
					zliber_count( nameslashname( dir, specif.name ));
				else
					filecount++;
			}
		}
		_findclose( handler );
	}
	else
		printf( "problem occured with folder %s while counting files\n" );
}


//----------------------------------------------------------------------


void zliber( char* dir )
{
	struct _finddata_t specif;
	long handler = _findfirst( nameslashname( dir, "*.*"), &specif );
 
	if( handler != -1 )
	{
		while( _findnext( handler, &specif ) != -1 )
		{
			if( strcmp( specif.name,"..") && strcmp(specif.name,"."))
			{
				char* tempname = nameslashname( dir, specif.name);
				if( specif.attrib&_A_SUBDIR )
					zliber( tempname );
				else
				{
					Bytef* source = (Bytef*) malloc( specif.size );	//add a check

					if( (file = fopen( tempname, "rb" )) == NULL )
					{
						printf( "could not open file %s for compression\n", specif.name );
						free( source );
						exit(0);
					}
					fread( source, specif.size, 1, file );	//add a check
					fclose( file );

					uLong destLen = specif.size*1001/1000 + 12;		//at least original size + 0.1% + 12 bytes
					Bytef* dest = (Bytef*) malloc( destLen );		//and add a check

					if( compress2( dest, &destLen, source, specif.size, zlevel ) != Z_OK )
					{
						printf( "compression of file %s was unsuccessful\n", specif.name );
						free( source ); free( dest );
						exit(0);
					}

					free( source );

					int offset = ftell( zfile );
					fseek( zfile, (tempcount++)*2*sizeof(uLong)+sizeof(int), SEEK_SET );
					fwrite( &specif.size, 1, sizeof(uLong), zfile );	//add a check
					fwrite( &offset, 1, sizeof(uLong), zfile );	//offset from the begining; add a check

					fseek( zfile, 0, SEEK_END );
					fwrite( dest, 1, destLen, zfile );	//add a check

					free( dest );

					printf( "%s : %d -> %d\n", specif.name, specif.size, destLen );
					totalorigsize += specif.size;
					totalcompsize += destLen;
				}
				free(tempname);
			}
		}
		_findclose( handler );
	}
	else
		printf( "could not open folder %s for zfile generation\n", dir );
}


//----------------------------------------------------------------------


void main(int argc, char* argv[])
{
	char curdir[_MAX_PATH];

	if( argc >= 4 )
	{
		_fullpath( curdir, ".", _MAX_PATH );

		if( _chdir( argv[1] ) == -1 )
		{
			printf( "invalid dir specified\n" );
			return;
		}

		_chdir( curdir );
	}
	else
	{
		printf( "not enough args" );
		return;
	}


	if( (zfile = fopen( argv[2], "wb" )) == NULL )
	{
		printf( "could not open zfile for writing\n" );
		return;
	}
	
	sscanf( argv[3], "%d", &zlevel );
	zliber_count( argv[1] );
	fwrite( &filecount, sizeof(int), 1, zfile );		//number of files
	int zeroint = 0;
	fwrite( &zeroint, 2*sizeof(uLong), filecount, zfile );
	zliber( argv[1] );		//the file handle must point to the current end of the file
	printf( "\nOriginal size : %d - Final size : %d\nRatio : %f.2%% (%d files)\n", totalorigsize, totalcompsize, (float)totalcompsize*100.f/(float)totalorigsize, tempcount );

	fclose( zfile );
}

