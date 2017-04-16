//#include <windows.h>
#include <stdio.h>		//in/out functions
#include <stdlib.h>		//mem functions
#include <string.h>		//string functions
#include <io.h>			//file functions
#include <direct.h>		//dir functions

#define	data_asm	"data.asm"
#define	data_h		"data.h"

FILE *dasm, *dh;
short int filecount = 0;

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


void includer( char* dir )
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
					includer( nameslashname( dir, specif.name ));
				else
				{
					char* name_ = forbidden2underscore( specif.name );
					fprintf( dasm, "global _%s_\n", name_ );
					//fprintf( dasm, "_%s_ dd %i\n", name_, specif.size+3 );
					fprintf( dasm, "_%s_ dd %i\n", name_, specif.size );
					//fprintf( dasm, "db '%3s'\n", give_ext(specif.name));
					fprintf( dasm, "incbin \"%s/%s\"\n\n", dir, specif.name );

					fprintf( dh, "#ifdef MG_DATA_DEFAULT_ASM\n" );
					fprintf( dh, "extern \"C\" char %s_[];\n", name_ );	//char[] and NOT char* or a value will be placed as argument of some fuctions instead of an address

					fprintf( dh, "#elif defined( MG_DATA_DEFAULT_FILE )\n" );
					fprintf( dh, "#define %s_ \"%s/%s\"\n", name_, dir, specif.name );

					fprintf( dh, "#elif defined( MG_DATA_DEFAULT_ZLIB )\n" );
					fprintf( dh, "#define %s_ \"\\x%02X\\x%02X\"\n", name_, filecount&0xFF, filecount>>8 );

					//***** add some file loading methods if needed

					fprintf( dh, "#endif\n\n" );
					free(name_);
					filecount++;
				}
			}
		}
		_findclose( handler );
	}
	else
		printf( "could not open folder %s for sources generation\n", dir );
}


//----------------------------------------------------------------------


void main(int argc, char* argv[])
{
	char curdir[_MAX_PATH];

	if( argc >= 2 )
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


	if( (dasm = fopen( data_asm, "w" )) == NULL || (dh = fopen( data_h, "w" )) == NULL )
	{
		printf( "could not open files for writing\n" );
		return;
	}
	
	fprintf( dasm, "section .data\n\n" );

	fprintf( dh, "#ifndef _DATA_H_\n#define _DATA_H_\n\n\n" );
	includer( argv[1] );
	fprintf( dh, "#endif\n" );

	fclose( dasm );
	fclose( dh );
}

