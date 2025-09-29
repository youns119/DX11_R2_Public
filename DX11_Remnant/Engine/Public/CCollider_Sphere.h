#pragma once

#include "CCollider.h"

BEGIN(Engine)

class CCollider_Sphere final 
	: public CCollider
{
private :
	CCollider_Sphere(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CCollider_Sphere() = default;

public :
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Update(_fmatrix _matWorld) override;
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* _pBatch, _fvector _vColor) override;

public :
	virtual void* Get_Desc() const override { return m_pDesc; }

public :
	virtual _bool Intersect(ECollider _eCollider, CCollider* _pTargetCollider) override;

private :
	BoundingSphere* m_pOriginalDesc{};
	BoundingSphere* m_pDesc{};

public :
	static CCollider_Sphere* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, void* _pDesc);
	virtual void Free() override;
};

END