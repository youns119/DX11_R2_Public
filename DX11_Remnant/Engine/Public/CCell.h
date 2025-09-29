#pragma once

#include "CObject.h"

#include "ECell.h"

BEGIN(Engine)

#ifdef _DEBUG

class CVIBuffer_Cell;

#endif

class CCell final
	: public CObject
{
private :
	CCell(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CCell() = default;

public :
	HRESULT Init(const _float3* _pPoints, _int _iIndex);

#ifdef _DEBUG

public :
	HRESULT Render();

#endif

public :
	const _vector Get_Point(EPoint _ePoint)
	{ return XMLoadFloat3(&m_vPoints[static_cast<_uint>(_ePoint)]); }

public :
	void Set_Neighbor(ELine _eLine, CCell* _pNeighborCell)
	{ m_iNeighborIndices[static_cast<_uint>(_eLine)] = _pNeighborCell->m_iIndex; }

public :
	const _bool IsIn(_fvector _vPosition, _int* _pNeighborIndex);
	const _bool Compare_Points(_fvector _vSrcPoint, _fvector _vDstPoint);
	_float Compute_Height(_fvector _vPosition);

private :
	ID3D11Device* m_pDevice{};
	ID3D11DeviceContext* m_pContext{};

#ifdef _DEBUG

private :
	CVIBuffer_Cell* m_pVIBuffer{};

#endif

private :
	_float3 m_vPoints[static_cast<_uint>(EPoint::POINT_END)]{};
	_int m_iNeighborIndices[static_cast<_uint>(ELine::LINE_END)]{};

private :
	_float4 m_vPlane{};

	_int m_iIndex{};

public :
	static CCell* Create
	(
		ID3D11Device* _pDevice, 
		ID3D11DeviceContext* _pContext, 
		const _float3* _pPoints,
		_int _iIndex
	);
	virtual void Free() override;
};

END