#pragma once

#include "CClonable.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract
	: public CClonable
{
protected :
	CVIBuffer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CVIBuffer(const CVIBuffer& _Prototype);
	virtual ~CVIBuffer() = default;

public :
	virtual HRESULT Init();
	virtual HRESULT Init(void* _pDesc) override;
	virtual HRESULT Render();

public :
	virtual HRESULT Bind_InputAssembler();

protected :
	ID3D11Device* m_pDevice{};
	ID3D11DeviceContext* m_pContext{};

protected :
	ID3D11Buffer* m_pVB{};
	ID3D11Buffer* m_pIB{};

	D3D11_BUFFER_DESC m_tBufferDesc{};
	D3D11_SUBRESOURCE_DATA m_tInitialData{};

protected :
	DXGI_FORMAT	m_eIndexFormat{};
	D3D11_PRIMITIVE_TOPOLOGY m_ePrimitiveTopology{};

	_uint m_iNumVertexBuffer{};
	_uint m_iNumVertice{};
	_uint m_iVertexStride{};
	_uint m_iNumIndice{};
	_uint m_iIndexStride{};

protected :
	_bool m_bCloned{};

protected :
	HRESULT Create_Buffer(ID3D11Buffer** _ppOut);

public :
	virtual CClonable* Clone(void* _pDesc) PURE;
	virtual void Free() override;
};

END