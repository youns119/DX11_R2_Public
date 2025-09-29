#pragma once

#include "CObject.h"

#include "DebugDraw.h"

#include "ECollider.h"

BEGIN(Engine)

class CCollider abstract
	: public CObject
{
protected :
	CCollider(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CCollider() = default;

public :
	virtual HRESULT Init(void* _pDesc) PURE;
	virtual void Update(_fmatrix _matWorld) PURE;
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* _pBatch, _fvector _vColor) PURE;

public :
	virtual void* Get_Desc() const PURE;

public :
	virtual _bool Intersect(ECollider _eCollider, CCollider* _pTargetCollider) PURE;

protected :
	ID3D11Device* m_pDevice{};
	ID3D11DeviceContext* m_pContext{};

public :
	virtual void Free() override;
};

END