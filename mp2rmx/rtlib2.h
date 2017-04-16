//-------------------------------------------------------------------
//RTLIB2.H - REPLACEMENT TRIGO FUNCTIONS
// __cdecl works on Release mode but Debug prefers __stdcall
//-------------------------------------------------------------------
//#pragma once

#ifndef _RTLIB2_H_
#define _RTLIB2_H_


extern "C"
{
	double __stdcall sqrt(double f);
	double __stdcall cos(double d);		//radians
	double __stdcall sin(double d);		//radians
	double __stdcall tan(double a);		//radians
	double __stdcall atan(double a);
	double __stdcall atan2(double d, double b);
	double __stdcall asin(double d);
	double __stdcall acos(double d);
	double __stdcall pow(double d, double e);
	long _declspec () _ftol ();
}

extern "C" double fabs(double a);
#pragma intrinsic (fabs)

//END
#endif