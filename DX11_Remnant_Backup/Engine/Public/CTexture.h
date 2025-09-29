#pragma once

#include "CClonable.h"

BEGIN(Engine)

class CShader;

class ENGINE_DLL CTexture final 
	: public CClonable
{
private :
	CTexture(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CTexture(const CTexture& _Prototype);
	virtual ~CTexture() = default;

public :
	virtual HRESULT Init(const _tchar* _pTextureFilePath, _uint _iNumTexture);
	virtual HRESULT Init(void* _pDesc) override;

public :
	ID3D11ShaderResourceView* Get_SRV(_uint _iIndex) { return m_vecSRV[_iIndex]; }
	vector<ID3D11ShaderResourceView*>* Get_SRVs() { return &m_vecSRV; }

public :
	HRESULT Bind_ShaderResource(CShader* _pShader, const _char* _pConstantName, _uint _iIndex);
	HRESULT Bind_ShaderResources(CShader* _pShader, const _char* _pConstantName);

private :
	ID3D11Device* m_pDevice{};
	ID3D11DeviceContext* m_pContext{};

private :
	vector<ID3D11ShaderResourceView*> m_vecSRV;

	_uint m_iNumSRV{};

public :
	static CTexture* Create
	(
		ID3D11Device* _pDevice, 
		ID3D11DeviceContext* _pContext, 
		const _tchar* _pTextureFilePath, 
		_uint _iNumTexture
	);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END