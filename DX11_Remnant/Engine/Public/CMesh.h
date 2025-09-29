#pragma once

#include "CVIBuffer.h"

#include "FMeshMaterial.h"

BEGIN(Engine)

class CMesh abstract
	: public CVIBuffer
{
protected:
	CMesh(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CMesh(const CMesh& _Prototype);
	virtual ~CMesh() = default;

public :
	virtual HRESULT Init(HANDLE& _hFile, _ulong& _dwByte);
	virtual HRESULT Init(void* _pDesc) override;

public :
	_uint Get_MaterialIndex() const { return m_iMaterialIndex; }
	FMeshMaterial Get_MaterialDesc(_uint _iTextureType) const
	{ return m_MeshMaterials[_iTextureType]; }

public :
	HRESULT Bind_Material(aiTextureType _eTextureType, const _char* _pConstantName, _uint _iTextureIndex);
	HRESULT Unbind_Material(aiTextureType _eTextureType);

protected :
	_char m_szName[MAX_PATH]{};
	_uint m_iMaterialIndex{};

protected :
	FMeshMaterial m_MeshMaterials[MATERIAL_MAX];

protected :
	virtual HRESULT Ready_IndexBuffer(HANDLE& _hFile, _ulong& _dwByte);

public :
	virtual CClonable* Clone(void* _pDesc) PURE;
	virtual void Free() override;
};

END