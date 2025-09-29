#include "CVIBuffer_Instancing.h"

CVIBuffer_Instancing::CVIBuffer_Instancing(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CVIBuffer{ _pDevice, _pContext }
{
}

CVIBuffer_Instancing::CVIBuffer_Instancing(const CVIBuffer_Instancing& _Prototype)
	: CVIBuffer(_Prototype)
	, m_tInstanceBufferDesc{ _Prototype.m_tInstanceBufferDesc }
	, m_tInstanceInitialData{ _Prototype.m_tInstanceInitialData }
	, m_iInstanceVertexStride{ _Prototype.m_iInstanceVertexStride }
	, m_iNumIndexPerInstance{ _Prototype.m_iNumIndexPerInstance }
	, m_iNumInstance{ _Prototype.m_iNumInstance }
	, m_pVBInstance{ _Prototype.m_pVBInstance }
	, m_pInstanceVertices{ _Prototype.m_pInstanceVertices }
	, m_pSpeeds{ _Prototype.m_pSpeeds }
	, m_bLoop{ _Prototype.m_bLoop }
	, m_vPivot{ _Prototype.m_vPivot }
{
	Safe_AddRef(m_pVBInstance);
}

HRESULT CVIBuffer_Instancing::Init(const FVIBuffer_Instancing* _pDesc)
{
	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Instancing::Init(void* _pDesc)
{
	return m_pDevice->CreateBuffer(&m_tInstanceBufferDesc, &m_tInstanceInitialData, &m_pVBInstance);
}

HRESULT CVIBuffer_Instancing::Render()
{
	if (m_pContext == nullptr ||
		Bind_InputAssembler())
		return E_FAIL;

	m_pContext->DrawIndexedInstanced(m_iNumIndexPerInstance, m_iNumInstance, 0, 0, 0);

	return S_OK;
}

HRESULT CVIBuffer_Instancing::Bind_InputAssembler()
{
	if (m_pContext == nullptr)
		return E_FAIL;

	ID3D11Buffer* pVertexBuffers[] = 
	{
		m_pVB,
		m_pVBInstance,
	};

	_uint iVertexStrides[] = 
	{
		m_iVertexStride,
		m_iInstanceVertexStride,
	};

	_uint iOffsets[] = 
	{
		0,
		0
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffer, pVertexBuffers, iVertexStrides, iOffsets);
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_ePrimitiveTopology);

	return S_OK;
}

void CVIBuffer_Instancing::Drop(_float _fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE tSubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &tSubResource);

	VTXINSTANCING* pVertices = static_cast<VTXINSTANCING*>(tSubResource.pData);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pVertices[i].vTranslation.y -= m_pSpeeds[i] * _fTimeDelta;
		pVertices[i].vLifeTime.y += _fTimeDelta;

		if (pVertices[i].vLifeTime.x <= pVertices[i].vLifeTime.y)
		{
			if (m_bLoop)
			{
				pVertices[i].vTranslation.y = m_pInstanceVertices[i].vTranslation.y;
				pVertices[i].vLifeTime.y = 0.f;
			}
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Instancing::Rise(_float _fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE tSubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &tSubResource);

	VTXINSTANCING* pVertices = static_cast<VTXINSTANCING*>(tSubResource.pData);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pVertices[i].vTranslation.y += m_pSpeeds[i] * _fTimeDelta;
		pVertices[i].vLifeTime.y += _fTimeDelta;

		if (pVertices[i].vLifeTime.x <= pVertices[i].vLifeTime.y)
		{
			if (m_bLoop)
			{
				pVertices[i].vTranslation.y = m_pInstanceVertices[i].vTranslation.y;
				pVertices[i].vLifeTime.y = 0.f;
			}
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Instancing::Explosion(_float _fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE tSubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &tSubResource);

	VTXINSTANCING* pVertices = static_cast<VTXINSTANCING*>(tSubResource.pData);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		_vector vMoveDir = XMVectorSetW
		(
			XMVector3Normalize(XMLoadFloat4(&pVertices[i].vTranslation) - XMLoadFloat3(&m_vPivot)), 
			0.f
		);

		XMStoreFloat4
		(
			&pVertices[i].vTranslation, 
			XMLoadFloat4(&pVertices[i].vTranslation) + vMoveDir * m_pSpeeds[i] * _fTimeDelta
		);
		pVertices[i].vLifeTime.y += _fTimeDelta;

		if (pVertices[i].vLifeTime.x <= pVertices[i].vLifeTime.y)
		{
			if (m_bLoop)
			{
				pVertices[i].vTranslation = m_pInstanceVertices[i].vTranslation;
				pVertices[i].vLifeTime.y = 0.f;
			}
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Instancing::Free()
{
	__super::Free();

	if (!m_bCloned)
	{
		Safe_Delete_Array(m_pSpeeds);
		Safe_Delete_Array(m_pInstanceVertices);
	}

	Safe_Release(m_pVBInstance);
}