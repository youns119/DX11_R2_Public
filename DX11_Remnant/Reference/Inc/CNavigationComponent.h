#pragma once

#include "CActorComponent.h"

#include "FNavigationComponent.h"

BEGIN(Engine)

#ifdef _DEBUG

class CShader;

#endif

class CCell;

class ENGINE_DLL CNavigationComponent final :
	public CActorComponent
{
private :
	CNavigationComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CNavigationComponent(const CNavigationComponent& _Prototype);
	virtual ~CNavigationComponent() = default;

public :
	virtual HRESULT Init(const _tchar* _pNavigationDataFile);
	virtual HRESULT Init(void* _pDesc) override;

#ifdef _DEBUG

public :
	virtual HRESULT Render() override;

#endif

public :
	void SetUp_WorldMatrix(_fmatrix _matWorld) 
	{
		XMStoreFloat4x4(&m_matWorld, _matWorld);
		XMStoreFloat4x4(&m_matWorldInverse, XMMatrixInverse(nullptr, _matWorld));
	}

public :
	_bool IsMove(_fvector _vWorldPos);
	_float Compute_Height(_fvector _vWorldPos);

private :
	static _float4x4 m_matWorld;
	static _float4x4 m_matWorldInverse;

private :
	_uint m_iCurrCellIndex{};
	vector<CCell*> m_vecCell;

#ifdef _DEBUG

private :
	CShader* m_pShader{};

#endif

private :
	HRESULT SetUp_Neighbors();

public :
	static CNavigationComponent* Create
	(
		ID3D11Device* _pDevice, 
		ID3D11DeviceContext* _pContext, 
		const _tchar* _pNavigationDataFile
	);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;

};

END