#pragma once

#include "CCollider.h"

BEGIN(Engine)

class CCollider_OBB final : 
	public CCollider
{
private :
	CCollider_OBB(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CCollider_OBB() = default;

public :
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Update(_fmatrix _matWorld) override;
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* _pBatch, _fvector _vColor) override;

public :
	virtual void* Get_Desc() const override { return m_pDesc; }

public :
	virtual _bool Intersect(ECollider _eCollider, CCollider* _pTargetCollider) override;

private :
	BoundingOrientedBox* m_pOriginalDesc{};
	BoundingOrientedBox* m_pDesc{};

public :
	static CCollider_OBB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* _pDesc);
	virtual void Free() override;
};

END