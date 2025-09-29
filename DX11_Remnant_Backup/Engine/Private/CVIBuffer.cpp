#include "CVIBuffer.h"

CVIBuffer::CVIBuffer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice{ _pDevice }
	, m_pContext{ _pContext }
	, m_bCloned{ false }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CVIBuffer::CVIBuffer(const CVIBuffer& _Prototype)
	: m_pDevice{ _Prototype.m_pDevice }
	, m_pContext{ _Prototype.m_pContext }
	, m_pVB{ _Prototype.m_pVB }
	, m_pIB{ _Prototype.m_pIB }
	, m_eIndexFormat{ _Prototype.m_eIndexFormat }
	, m_ePrimitiveTopology{ _Prototype.m_ePrimitiveTopology }
	, m_iNumVertexBuffer{ _Prototype.m_iNumVertexBuffer }
	, m_iNumVertice{ _Prototype.m_iNumVertice }
	, m_iVertexStride{ _Prototype.m_iVertexStride }
	, m_iNumIndice{ _Prototype.m_iNumIndice }
	, m_iIndexStride{ _Prototype.m_iIndexStride }
	, m_bCloned{ true }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

HRESULT CVIBuffer::Init()
{
	return S_OK;
}

HRESULT CVIBuffer::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	if (m_pContext == nullptr ||
		FAILED(Bind_InputAssembler()))
		return E_FAIL;

	m_pContext->DrawIndexed(m_iNumIndice, 0, 0);

	return S_OK;
}

HRESULT CVIBuffer::Bind_InputAssembler()
{
	ID3D11Buffer* pVertexBuffers[] = 
	{
		m_pVB,
	};

	_uint iVertexStrides[] = 
	{
		m_iVertexStride,
	};

	_uint iOffsets[] = 
	{
		0
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffer, pVertexBuffers, iVertexStrides, iOffsets);
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_ePrimitiveTopology);

	return S_OK;
}

HRESULT CVIBuffer::Create_Buffer(ID3D11Buffer** _ppOut)
{
	return m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitialData, _ppOut);
}

void CVIBuffer::Free()
{
	__super::Free();

	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}