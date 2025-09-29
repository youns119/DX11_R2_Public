#include "CTerrainComponent.h"

#include "CGameInstance.h"
#include "CActor.h"
#include "CVIBuffer_Terrain.h"
#include "CTexture.h"

CTerrainComponent::CTerrainComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CPrimitiveComponent{ _pDevice, _pContext }
{
	m_strName = g_strTerrainComponent;
}

CTerrainComponent::CTerrainComponent(const CTerrainComponent& _Prototype)
	: CPrimitiveComponent(_Prototype)
{
}

HRESULT CTerrainComponent::Init()
{
	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrainComponent::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	FTerrainComponent* pDesc = static_cast<FTerrainComponent*>(_pDesc);
	m_iNumTexture = pDesc->iNumTexture;

	m_vecTexture.assign(m_iNumTexture, nullptr);

	if (FAILED(__super::Init(pDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrainComponent::Render()
{
	if (m_pVIBuffer == nullptr)
		return S_OK;

	if (FAILED(Bind_ShaderResource())	||
		FAILED(Begin_Shader())			||
		FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	return S_OK;
}

_uint2 CTerrainComponent::Get_VerticeNum()
{
	return m_pVIBuffer->Get_VerticeNum();
}

const _float3* CTerrainComponent::Get_VerticePos()
{
	return m_pVIBuffer->Get_VerticePos();
}

HRESULT CTerrainComponent::Add_VIBuffer
(
	_uint _iPrototypeLevelIndex, 
	const _wstring& _strPrototypeTag, 
	void* _pDesc
)
{
	m_pVIBuffer = static_cast<CVIBuffer_Terrain*>(CGameInstance::Clone_Prototype
	(
		_iPrototypeLevelIndex,
		_strPrototypeTag,
		_pDesc
	));
	if (m_pVIBuffer == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrainComponent::Add_Texture
(
	_uint _iPrototypeLevelIndex, 
	_uint _iTextureIndex,
	const _wstring& _strPrototypeTag, 
	void* _pDesc
)
{
	if (_iTextureIndex >= m_iNumTexture)
		return E_FAIL;

	CTexture* pTexture = static_cast<CTexture*>(CGameInstance::Clone_Prototype
	(
		_iPrototypeLevelIndex,
		_strPrototypeTag,
		_pDesc
	));
	if (pTexture == nullptr)
		return E_FAIL;

	m_vecTexture[_iTextureIndex] = pTexture;

	return S_OK;
}

HRESULT CTerrainComponent::Bind_Texture
(
	_uint _iTextureType, 
	const _char* _pConstantName, 
	_uint _iTextureIndex
)
{
	if (_iTextureType >= m_vecTexture.size() ||
		m_vecTexture[_iTextureType] == nullptr)
		return E_FAIL;

	return m_vecTexture[_iTextureType]->Bind_ShaderResource(m_pShader, _pConstantName, _iTextureIndex);
}

HRESULT CTerrainComponent::Bind_Textures
(
	_uint _iTextureType, 
	const _char* _pConstantName
)
{
	if (_iTextureType >= m_vecTexture.size() ||
		m_vecTexture[_iTextureType] == nullptr)
		return E_FAIL;

	return m_vecTexture[_iTextureType]->Bind_ShaderResources(m_pShader, _pConstantName);
}

CTerrainComponent* CTerrainComponent::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CTerrainComponent* pInstance = new CTerrainComponent(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Create : CTerrainComponent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CTerrainComponent::Clone(void* _pDesc)
{
	CTerrainComponent* pInstance = new CTerrainComponent(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Create : CTerrainComponent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrainComponent::Free()
{
	__super::Free();

	for (auto& Texture : m_vecTexture)
		Safe_Release(Texture);

	Safe_Release(m_pVIBuffer);
}