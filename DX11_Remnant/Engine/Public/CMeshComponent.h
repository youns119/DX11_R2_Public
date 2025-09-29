#pragma once

#include "CPrimitiveComponent.h"

BEGIN(Engine)

class CMesh;
class CMaterial;

class ENGINE_DLL CMeshComponent abstract
	: public CPrimitiveComponent
{
protected :
	CMeshComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CMeshComponent(const CMeshComponent& _Prototype);
	virtual ~CMeshComponent() = default;

public :
	virtual HRESULT Init(const _tchar* _szModelName, _fmatrix _matPreTransform);
	virtual HRESULT Init(void* _pDesc) override;

public :
	_uint Get_NumMesh() const { return m_iNumMesh; }

public :
	HRESULT Bind_Material
	(
		_uint _iMeshIndex,
		aiTextureType _eMaterialType,
		const _char* _pConstantName,
		_uint _iTextureIndex = 0
	);
	HRESULT Unbind_Material(_uint _iMeshIndex, aiTextureType _eMaterialType);

protected :
	vector<CMesh*> m_vecMesh;
	vector<CMaterial*> m_vecMaterial;

protected :
	_float4x4 m_matPreTransform{};

protected :
	_uint m_iNumMesh{};
	_uint m_iNumMaterial{};

protected :
	HRESULT Bind_MeshMaterial(const CMesh* _Mesh);

protected :
	HRESULT Ready_Material(HANDLE& _hFile, _ulong& _dwByte, const _tchar* _szModelName);

public :
	virtual CClonable* Clone(void* _pDesc) PURE;
	virtual void Free() override;
};

END