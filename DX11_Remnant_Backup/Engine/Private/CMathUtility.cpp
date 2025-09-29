#include "CMathUtility.h"

_float CMathUtility::Random_Float(_float _fMin, _float _fMax)
{
    _float fRandom = static_cast<_float>(rand()) / RAND_MAX;

    return _fMin + fRandom * (_fMax - _fMin);
}

_bool CMathUtility::IsVectorRight(_fvector _vSrcVector, _gvector _vDstVector)
{
    _vector vCross = XMVector3Cross(_vSrcVector, _vDstVector);
    _float fDot = XMVectorGetX(XMVector3Dot(vCross, XMVectorSet(0.f, 1.f, 0.f, 0.f)));

    return fDot > 0.0f;
}

_float CMathUtility::GetBetweenAngle(_fvector _vSrcVector, _gvector _vDstVector)
{
    _bool bIsRight = IsVectorRight(_vSrcVector, _vDstVector);

    _vector vNorm1 = XMVector3Normalize(_vSrcVector);
    _vector vNorm2 = XMVector3Normalize(_vDstVector);

    _float fDot = XMVectorGetX(XMVector3Dot(vNorm1, vNorm2));
    fDot = max(-1.0f, min(1.0f, fDot));

    _float fDegree = XMConvertToDegrees(acosf(fDot));

    if (!bIsRight) fDegree = -fDegree;

    return fDegree;
}