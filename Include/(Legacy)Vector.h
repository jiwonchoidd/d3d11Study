//#pragma once
//#include <Windows.h>
//#include <math.h>
//
//#define EPSILON		((FLOAT)  0.001f)
//#define PI			((FLOAT)  3.141592654f)
//#define DegreeToRadian( degree ) ((degree) * (PI / 180.0f))
//#define RadianToDegree( radian ) ((radian) * (180.0f / PI))
//#define MAKECOLOR_ARGB(a, r, g, b)			(((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)
//#define IS_IN_RANGE(value,r0,r1) (( ((r0) <= (value)) && ((value) <= (r1)) ) ? 1 : 0)
//
//struct float2
//{
//	union
//	{
//		struct { float x, y; };
//		float v[2];
//	};
//};
//
//struct float3
//{
//	union
//	{
//		struct { float x, y, z; };
//		float v[3];
//	};
//};
//struct float4
//{
//	union
//	{
//		struct { float x, y, z, w; };
//		float v[4];
//	};
//};
//class Vector2 : public float2
//{
//public:
//	Vector2();
//	Vector2(const Vector2& v0);
//	Vector2(float fX, float fY);
//};
//class Vector3 : public float3
//{
//public:
//	Vector3();
//	Vector3(const Vector3& v0);
//	Vector3(float fX, float fY, float fZ);
//	// 연산자 재정의 
//	Vector3 operator + (Vector3 const& v0);
//	Vector3 operator - (Vector3 const& v0);
//	Vector3 operator * (float const& fScala);
//	Vector3 operator + (float const& fScala);
//	Vector3 operator / (float const& fScala);
//	// Dot Product
//	float operator | (Vector3 const& v0);
//	// Cross Product
//	Vector3 operator ^ (Vector3 const& v0);
//	bool	operator == (Vector3 const& v0);
//	// 제곱
//	float LengthSquared();
//	// 원점으로 부터의 거리
//	float Length();
//	Vector3 Normal();
//	float Angle(Vector3& v0);
//};
//class Vector4 : public float4
//{
//public:
//	Vector4();
//	Vector4(const Vector4& v0);
//	Vector4(float fX, float fY, float fZ, float fW);
//};
//
