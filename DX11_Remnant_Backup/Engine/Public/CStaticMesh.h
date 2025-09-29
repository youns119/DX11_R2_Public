#pragma once

#include "CMesh.h"

#include "FVertex_StaticMesh.h"

BEGIN(Engine)

class CStaticMesh final
	: public CMesh
{
private :
	CStaticMesh(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CStaticMesh(const CStaticMesh& _Prototype);
	virtual ~CStaticMesh() = default;

public :
	virtual HRESULT Init(HANDLE& _hFile, _ulong& _dwByte, _fmatrix _matPreTransform);
	virtual HRESULT Init(void* _pDesc) override;

public:
	static CStaticMesh* Create
	(
		ID3D11Device* _pDevice,
		ID3D11DeviceContext* _pContext,
		HANDLE& _hFile,
		_ulong& _dwByte,
		_fmatrix _matPreTransform
	);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END