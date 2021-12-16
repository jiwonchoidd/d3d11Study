#pragma once
#include "Vector.h"
struct f_Matrix
{
	union {
		struct {
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};
};
class Matrix : public f_Matrix
{
public:
	//생성자 단위행렬로 만들어준다.
	Matrix()
	{
		Identity();
	}
	void Identity()
	{
		_12 = _13 = _14 = 0;
		_21 = _23 = _24 = 0;
		_31 = _32 = _34 = 0;
		_41 = _42 = _43 = 0;
		_11 = _22 = _33 = _44 = 1;
	}
	//Z축으로 회전
	static Matrix RotationZ(float fRadian)
	{
		Matrix matRet;
		float fCos = cosf(fRadian);
		float fSin = sinf(fRadian);
		//싸코코싸
		matRet._11 = fCos; matRet._12 = fSin;
		matRet._21 = -fSin; matRet._22 = fCos;
		return matRet;
	}
	//X축으로 회전
	static Matrix RotationX(float fRadian)
	{
		Matrix matRet;
		float fCos = cosf(fRadian);
		float fSin = sinf(fRadian);
		//싸코코싸
		matRet._22 = fCos; matRet._32 = fSin;
		matRet._32 = -fSin; matRet._33 = fCos;
		return matRet;
	}
	static Matrix RotationXZ(float fRadian)
	{
		Matrix matRet;
		float fCos = cosf(fRadian);
		float fSin = sinf(fRadian);
		//싸코코싸
		matRet._11 = fCos;
		matRet._12 = fSin;
		matRet._21 = -fSin;
		matRet._22 = fCos;
		matRet._22 = fCos;
		matRet._32 = fSin;
		matRet._32 = -fSin;
		matRet._33 = fCos;
		return matRet;
	}
	//바꾼다.
	Matrix Transpose()
	{
		Matrix Matrix;
		Matrix._11 = _11; Matrix._12 = _21; Matrix._13 = _31; Matrix._14 = _41;
		Matrix._21 = _12; Matrix._22 = _22; Matrix._23 = _32; Matrix._24 = _42;
		Matrix._31 = _13; Matrix._32 = _23; Matrix._33 = _33; Matrix._34 = _43;
		Matrix._41 = _14; Matrix._42 = _24; Matrix._43 = _34; Matrix._44 = _44;
		return Matrix;
	}
	static Matrix ViewLookAt(Vector3& vPos,
		Vector3& vTarget,
		Vector3& vUp)
	{
		Matrix matRet;
		Vector3 vZ = (vTarget - vPos).Normal();
		Vector3 vX = (vUp ^ vZ).Normal();
		Vector3 vY = (vZ ^ vX).Normal();
		matRet._11 = vX.x; matRet._21 = vX.y; matRet._31 = vX.z;
		matRet._12 = vY.x; matRet._22 = vY.y; matRet._32 = vY.z;
		matRet._13 = vZ.x; matRet._23 = vZ.y; matRet._33 = vZ.z;
		matRet._41 = -(vPos | vX);
		matRet._42 = -(vPos | vY);
		matRet._43 = -(vPos | vZ);
		return matRet;
	}
	// w / h =1.33
	static Matrix PerspectiveFovLH(
		float fNearPlane, float fFarPlane,
		float fovy, float Aspect)
	{
		float    h, w, Q;
		// 1/tans(x) = cot(x)
		h = 1 / tan(fovy * 0.5f); 
		w = h / Aspect;
		Q = fFarPlane / (fFarPlane - fNearPlane);
		Matrix ret;
		ZeroMemory(&ret, sizeof(Matrix));
		ret._11 = w;
		ret._22 = h;
		ret._33 = Q;
		ret._43 = -Q * fNearPlane;
		ret._34 = 1;
		return ret;
	}
};

