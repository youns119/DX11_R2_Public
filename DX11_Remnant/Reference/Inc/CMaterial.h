#pragma once

#include "CObject.h"

BEGIN(Engine)

class CShader;

class CMaterial final
	: public CObject
{
private :
	CMaterial(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CMaterial() = default;

public :
	HRESULT Init(HANDLE& _hFile, _ulong& _dwByte, const _tchar* _szModelName);

public :
	HRESULT Bind_Material
	(
		CShader* _pShader, 
		aiTextureType _eTextureType, 
		const _char* _pConstantName, 
		_uint _iTextureIndex
	);

private :
	ID3D11Device* m_pDevice{};
	ID3D11DeviceContext* m_pContext{};

private :
	vector<ID3D11ShaderResourceView*> m_vecMtrlTexture[AI_TEXTURE_TYPE_MAX];

public :
	static CMaterial* Create
	(
		ID3D11Device* _pDevice,
		ID3D11DeviceContext* _pContext,
		HANDLE& _hFile,
		_ulong& _dwByte,
		const _tchar* _szModelName
	);
	virtual void Free() override;
};

END