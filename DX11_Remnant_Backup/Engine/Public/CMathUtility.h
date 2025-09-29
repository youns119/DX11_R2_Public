#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CMathUtility
{
public :
	static _float Random_Float(_float _fMin, _float _fMax);

	static _bool IsVectorRight(_fvector _vSrcVector, _gvector _vDstVector);
	static _float GetBetweenAngle(_fvector _vSrcVector, _gvector _vDstVector);
};

END