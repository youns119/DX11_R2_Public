#include "CFrustum.h"

#include "CGameInstance.h"

CFrustum::CFrustum()
{
}

HRESULT CFrustum::Init()
{
	m_vOriginalPoints[0] = _float4(-1.f, 1.f, 0.f, 1.f);
	m_vOriginalPoints[1] = _float4(1.f, 1.f, 0.f, 1.f);
	m_vOriginalPoints[2] = _float4(1.f, -1.f, 0.f, 1.f);
	m_vOriginalPoints[3] = _float4(-1.f, -1.f, 0.f, 1.f);

	m_vOriginalPoints[4] = _float4(-1.f, 1.f, 1.f, 1.f);
	m_vOriginalPoints[5] = _float4(1.f, 1.f, 1.f, 1.f);
	m_vOriginalPoints[6] = _float4(1.f, -1.f, 1.f, 1.f);
	m_vOriginalPoints[7] = _float4(-1.f, -1.f, 1.f, 1.f);

	return S_OK;
}

void CFrustum::Update()
{
	_matrix	matViewInverse = CGameInstance::Get_TransformMatrix_Inverse(ED3DTransform::D3DTS_VIEW);
	_matrix	matProjInverse = CGameInstance::Get_TransformMatrix_Inverse(ED3DTransform::D3DTS_PROJ);

	for (size_t i = 0; i < 8; i++)
	{
		XMStoreFloat4(&m_vWorldPoints[i], 
			XMVector3TransformCoord(XMLoadFloat4(&m_vOriginalPoints[i]), matProjInverse));
		XMStoreFloat4(&m_vWorldPoints[i],
			XMVector3TransformCoord(XMLoadFloat4(&m_vWorldPoints[i]), matViewInverse));
	}

	Make_Planes(m_vWorldPoints, m_vWorldPlanes);
}

_bool CFrustum::IsIn_WorldSpace(_fvector _vPosition, _float _fRange)
{
	for (size_t i = 0; i < 6; i++)
	{
		_vector vWorldPlane = XMLoadFloat4(&m_vWorldPlanes[static_cast<_uint>(i)]);

		if (XMVectorGetX(XMPlaneDotCoord(vWorldPlane, _vPosition)) > _fRange)
			return false;
	}

	return true;
}

void CFrustum::Make_Planes(const _float4* _pPoints, _float4* _pPlanes)
{
	// X+
	XMStoreFloat4
	(
		&_pPlanes[0],
		XMPlaneFromPoints
		(
			XMLoadFloat4(&_pPoints[1]), 
			XMLoadFloat4(&_pPoints[5]), 
			XMLoadFloat4(&_pPoints[6])
		)
	);
	// X-
	XMStoreFloat4
	(
		&_pPlanes[1],
		XMPlaneFromPoints
		(
			XMLoadFloat4(&_pPoints[4]), 
			XMLoadFloat4(&_pPoints[0]), 
			XMLoadFloat4(&_pPoints[3])
		)
	);

	// Y+
	XMStoreFloat4
	(
		&_pPlanes[2],
		XMPlaneFromPoints
		(
			XMLoadFloat4(&_pPoints[4]), 
			XMLoadFloat4(&_pPoints[5]),
			XMLoadFloat4(&_pPoints[1])
		)
	);
	// Y-
	XMStoreFloat4
	(
		&_pPlanes[3],
		XMPlaneFromPoints
		(
			XMLoadFloat4(&_pPoints[3]), 
			XMLoadFloat4(&_pPoints[2]), 
			XMLoadFloat4(&_pPoints[6])
		)
	);

	// Z+
	XMStoreFloat4
	(
		&_pPlanes[4],
		XMPlaneFromPoints
		(
			XMLoadFloat4(&_pPoints[5]), 
			XMLoadFloat4(&_pPoints[4]), 
			XMLoadFloat4(&_pPoints[7])
		)
	);
	// Z-
	XMStoreFloat4
	(
		&_pPlanes[5],
		XMPlaneFromPoints
		(
			XMLoadFloat4(&_pPoints[0]), 
			XMLoadFloat4(&_pPoints[1]),
			XMLoadFloat4(&_pPoints[2])
		)
	);
}

CFrustum* CFrustum::Create()
{
	CFrustum* pInstance = new CFrustum();

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Created : CFrustum");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFrustum::Free()
{
	__super::Free();
}