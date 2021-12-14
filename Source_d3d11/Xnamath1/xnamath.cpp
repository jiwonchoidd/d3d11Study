#include "xnamath.h"
#include <windows.h>
#include <xnamath.h>
#include <iostream>

std::ostream& operator<<(std::ostream& os, FXMVECTOR v)
{
	XMFLOAT3 dest;
	XMStoreFloat3(&dest, v);
	os << "(" << dest.x << ", " << dest.y << ", " << dest.z << ")";
	return os;
}
int main()
{
	std::cout.setf(std::ios_base::boolalpha);

	//sse2 지원 여부 사양체크
	if (!XMVerifyCPUSupport())
	{
		std::cout << "xna math를 지원하지 않음";
		return 0;
	}
	//0 백터를 돌려줌
	XMVECTOR zero	= XMVectorZero();
	//1111
	XMVECTOR splat	= XMVectorSplatOne();
	XMVECTOR u = XMVectorSet(1.0f, 2.0f, 3.0f, 0.0f);
	//한 스칼라값으로 모든 인자 복사
	XMVECTOR v = XMVectorReplicate(-2.0f);
	//백터의 Z 축으로 모든 인자 복사
	XMVECTOR w = XMVectorSplatZ(u);
	
	std::cout << "zero :" << zero << std::endl;
	std::cout << "splat :" << splat << std::endl;
	std::cout << "u :" << u << std::endl;
	std::cout << "v :" << v << std::endl;
	std::cout << "w :" << w << std::endl;


	//백터의 길이 1^2+2^2+3^2 14
	std::cout << "u :" << u << std::endl;
	std::cout << "u length  루트 14의 근사값은 3.7416573867:" << XMVector3Length(u) << std::endl;
	return 0;
}

