#pragma once

#include "CClonable.h"

BEGIN(Engine)

class CSceneComponent;
class CTexture;

class ENGINE_DLL CShader final 
	: public CClonable
{
private :
	CShader(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CShader(const CShader& _Prototype);
	virtual ~CShader() = default;

public :
	virtual HRESULT Init
	(
		const _tchar* _pShaderFilePath, 
		const D3D11_INPUT_ELEMENT_DESC* _pVertexElements,
		_uint _iNumElement
	);
	virtual HRESULT Init(void* _pDesc) override;

public :
	HRESULT Begin(_uint _iPassIndex);

public :
	HRESULT Bind_RawValue(const _char* _pConstantName, const void* _pData, _uint _iSize);

	HRESULT Bind_Matrix(const _char* _pConstantName, const _float4x4* _pMatrix);
	HRESULT Bind_Matrix(const _char* _pConstantName, CSceneComponent* _pSceneComponent);
	HRESULT Bind_Matrices(const _char* _pConstantName, const _float4x4* _pMatrix, _uint _iNumMatrix);

	HRESULT Bind_SRV(const _char* _pConstantName, ID3D11ShaderResourceView* _pSRV);
	HRESULT Bind_SRV(const _char* _pConstantName, CTexture* _pTexture, _uint _iIndex);
	HRESULT Bind_SRVs(const _char* _pConstantName, ID3D11ShaderResourceView** _ppSRV, _uint _iNumSRV);
	HRESULT Bind_SRVs(const _char* _pConstantName, CTexture* _pTexture);

private :
	ID3D11Device* m_pDevice{};
	ID3D11DeviceContext* m_pContext{};

private :
	ID3DX11Effect* m_pEffect{};

private :
	vector<ID3D11InputLayout*> m_vecInputLayout;

	_uint m_iNumPass{};

public :
	static CShader* Create
	(
		ID3D11Device* _pDevice, 
		ID3D11DeviceContext* _pContext, 
		const _tchar* _pShaderFilePath, 
		const D3D11_INPUT_ELEMENT_DESC* _pVertexElements, 
		_uint _iNumElement
	);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END