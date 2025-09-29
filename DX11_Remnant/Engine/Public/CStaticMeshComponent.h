#pragma once

#include "CMeshComponent.h"

#include "FStaticMeshComponent.h"

BEGIN(Engine)

class ENGINE_DLL CStaticMeshComponent final
	: public CMeshComponent
{
private :
	CStaticMeshComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CStaticMeshComponent(const CStaticMeshComponent& _Prototype);
	virtual ~CStaticMeshComponent() = default;

public :
	virtual HRESULT Init(const _tchar* _szModelName, _fmatrix _matPreTransform) override;
	virtual HRESULT Init(void* _pDesc) override;
	virtual HRESULT Render() override;

private :
	virtual HRESULT Ready_Mesh(HANDLE& _hFile, _ulong& _dwByte, _fmatrix _matPreTransform);

public :
	static CStaticMeshComponent* Create
	(
		ID3D11Device* _pDevice, 
		ID3D11DeviceContext* _pContext, 
		const _tchar* _szModelName,
		_fmatrix _matPreTransform = XMMatrixIdentity()
	);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END