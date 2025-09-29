#include "CCell.h"

#include "CVIBuffer_Cell.h"

CCell::CCell(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice{ _pDevice }
	, m_pContext{ _pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CCell::Init(const _float3* _pPoints, _int _iIndex)
{
	memcpy(m_vPoints, _pPoints, sizeof(_float3) * static_cast<_uint>(EPoint::POINT_END));

	m_iIndex = _iIndex;

	XMStoreFloat4(&m_vPlane, XMPlaneFromPoints
	(
		XMLoadFloat3(&_pPoints[static_cast<_uint>(EPoint::POINT_A)]), 
		XMLoadFloat3(&_pPoints[static_cast<_uint>(EPoint::POINT_B)]),
		XMLoadFloat3(&_pPoints[static_cast<_uint>(EPoint::POINT_C)]))
	);

#ifdef _DEBUG

	m_pVIBuffer = CVIBuffer_Cell::Create(m_pDevice, m_pContext, m_vPoints);
	if (m_pVIBuffer == nullptr)
		return E_FAIL;

#endif

	return S_OK;
}

#ifdef _DEBUG

HRESULT CCell::Render()
{
	m_pVIBuffer->Render();

	return S_OK;
}

#endif

const _bool CCell::IsIn(_fvector _vPosition, _int* _pNeighborIndex)
{
	for (size_t i = 0; i < static_cast<_uint>(ELine::LINE_END); i++)
	{
		_vector vDir = XMVector3Normalize(_vPosition - XMLoadFloat3(&m_vPoints[i]));
		_vector vLine = XMLoadFloat3(&m_vPoints[(i + 1) % 3]) - XMLoadFloat3(&m_vPoints[i]);
		_vector vNormal = XMVector3Normalize(XMVectorSet(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine), 0.f));

		if (XMVectorGetX(XMVector3Dot(vDir, vNormal)) > 0)
		{
			*_pNeighborIndex = m_iNeighborIndices[i];
			return false;
		}
	}

	return true;
}

const _bool CCell::Compare_Points(_fvector _vSrcPoint, _fvector _vDstPoint)
{
	if (XMVector3Equal(_vSrcPoint, XMLoadFloat3(&m_vPoints[static_cast<_uint>(EPoint::POINT_A)])))
	{
		if (XMVector3Equal(_vDstPoint, XMLoadFloat3(&m_vPoints[static_cast<_uint>(EPoint::POINT_B)])))
			return true;
		if (XMVector3Equal(_vDstPoint, XMLoadFloat3(&m_vPoints[static_cast<_uint>(EPoint::POINT_C)])))
			return true;
	}

	if (XMVector3Equal(_vSrcPoint, XMLoadFloat3(&m_vPoints[static_cast<_uint>(EPoint::POINT_B)])))
	{
		if (XMVector3Equal(_vDstPoint, XMLoadFloat3(&m_vPoints[static_cast<_uint>(EPoint::POINT_C)])))
			return true;
		if (XMVector3Equal(_vDstPoint, XMLoadFloat3(&m_vPoints[static_cast<_uint>(EPoint::POINT_A)])))
			return true;
	}

	if (XMVector3Equal(_vSrcPoint, XMLoadFloat3(&m_vPoints[static_cast<_uint>(EPoint::POINT_C)])))
	{
		if (XMVector3Equal(_vDstPoint, XMLoadFloat3(&m_vPoints[static_cast<_uint>(EPoint::POINT_A)])))
			return true;
		if (XMVector3Equal(_vDstPoint, XMLoadFloat3(&m_vPoints[static_cast<_uint>(EPoint::POINT_B)])))
			return true;
	}

	return false;
}

_float CCell::Compute_Height(_fvector _vPosition)
{
	// ax + by + cz + d = 0
	// y = (-ax - cz - d) / b
	return (-m_vPlane.x * _vPosition.m128_f32[0] - m_vPlane.z * _vPosition.m128_f32[2] - m_vPlane.w) / m_vPlane.y;
}

CCell* CCell::Create
(
	ID3D11Device* _pDevice, 
	ID3D11DeviceContext* _pContext, 
	const _float3* _pPoints, 
	_int _iIndex
)
{
	CCell* pInstance = new CCell(_pDevice, _pContext);

	if (FAILED(pInstance->Init(_pPoints, _iIndex)))
	{
		MSG_BOX("Failed To Create : CCell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCell::Free()
{
	__super::Free();

#ifdef _DEBUG

	Safe_Release(m_pVIBuffer);

#endif

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}