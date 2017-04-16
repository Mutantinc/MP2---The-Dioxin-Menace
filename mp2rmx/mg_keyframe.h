//-------------------------------------------------------------------
//MG_KEYFRAME.H
//-------------------------------------------------------------------
#ifndef _MG_KEYFRAME_H_
#define _MG_KEYFRAME_H_


class mgTCB
{
public:
	float T,C,B;
	float EaseFrom;
	float EaseTo;

	inline mgTCB()
	{
		EaseFrom = EaseTo = 0.f;
	}

};



//-------------------------------------------------------------------
//-------------------------------------------------------------------


class mgKeyFrame
{
public:
	uint Frame;
	mgVector Vector;
	mgQuaternion Quaternion;
	float Float;
	mgVector* Mesh;

	mgTCB TCB;
	mgVector TanLeft;
	mgVector TanRight;
	mgQuaternion CtlLeft;
	mgQuaternion CtlRight;

//-------------------------------------------------------------------

	inline void ImportTCB(uint fp)
	{
		TCB.T = mgReadFlt(fp);
		TCB.C = mgReadFlt(fp);
		TCB.B = mgReadFlt(fp);
		TCB.EaseFrom = mgReadFlt(fp);
		TCB.EaseTo = mgReadFlt(fp);
	}

	inline void ImportTangents(uint fp)
	{
		TanLeft.ReadV(fp);
		TanRight.ReadV(fp);
	}

//-------------------------------------------------------------------

	inline void ImportTCBPosKey(uint fp)
	{
		Vector.ReadV(fp);
		ImportTCB(fp);
	}

	inline void ImportBezPosKey(uint fp)
	{
		Vector.ReadV(fp);
		ImportTangents(fp);
	}

	inline void ImportLinPosKey(uint fp)
	{
		Vector.ReadV(fp);
	}

//-------------------------------------------------------------------

	inline void ImportTCBRotKey(uint fp)
	{
		Quaternion.ReadQ(fp);
		ImportTCB(fp);
	}

	inline void ImportBezRotKey(uint fp)
	{
		Quaternion.ReadQ(fp);
	}

	inline void ImportLinRotKey(uint fp)
	{
		Quaternion.ReadQ(fp);
	}

//-------------------------------------------------------------------

	inline void ImportTCBScaleKey(uint fp)
	{
		Vector.ReadV(fp);
		Quaternion.ReadQ(fp);
		ImportTCB(fp);
	}

	inline void ImportBezScaleKey(uint fp)
	{
		Vector.ReadV(fp);
		Quaternion.ReadQ(fp);
		ImportTangents(fp);
	}

	inline void ImportLinScaleKey(uint fp)
	{
		Vector.ReadV(fp);
		Quaternion.ReadQ(fp);
	}

//-------------------------------------------------------------------

	inline void ImportFloatKey(uint fp)
	{
		Float = mgReadFlt(fp);
	}
	
//-------------------------------------------------------------------
//-------------------------------------------------------------------

	//***** ne pas oublier les mesh tracks


};


//END
#endif

