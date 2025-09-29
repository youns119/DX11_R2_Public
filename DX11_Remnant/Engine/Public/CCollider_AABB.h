#pragma once

#include "CCollider.h"

BEGIN(Engine)

class CCollider_AABB final 
	: public CCollider
{
private :
	CCollider_AABB(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CCollider_AABB() = default;

public :
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Update(_fmatrix _matWorld) override;
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* _pBatch, _fvector _vColor) override;

public :
	virtual void* Get_Desc() const override { return m_pDesc; }

public :
	virtual _bool Intersect(ECollider _eCollider, CCollider* _pTargetCollider) override;

private :
	BoundingBox* m_pOriginalDesc{};
	BoundingBox* m_pDesc{};

public :
	static CCollider_AABB* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, void* _pDesc);
	virtual void Free() override;
};

END