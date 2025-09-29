#include "CCubeComponent.h"

#include "CGameInstance.h"
#include "CVIBuffer_Cube.h"
#include "CTexture.h"

CCubeComponent::CCubeComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CPrimitiveComponent{ _pDevice, _pContext }
{
	m_strName = g_strCubeComponent;
}

CCubeComponent::CCubeComponent(const CCubeComponent& _Prototype)
	: CPrimitiveComponent(_Prototype)
	, m_pVIBuffer{ _Prototype.m_pVIBuffer }
{
	Safe_AddRef(m_pVIBuffer);
}

HRESULT CCubeComponent::Init()
{
	if (FAILED(__super::Init()))
		return E_FAIL;

	m_pVIBuffer = CVIBuffer_Cube::Create(m_pDevice, m_pContext);
	if (m_pVIBuffer == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CCubeComponent::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	if (FAILED(__super::Init(_pDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCubeComponent::Render()
{
	if (m_pVIBuffer == nullptr)
		return S_OK;

	if (FAILED(Bind_ShaderResource())	||
		FAILED(Bind_Texture())			||
		FAILED(Begin_Shader())			||
		FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCubeComponent::Add_Texture
(
	_uint _iPrototypeLevelIndex, 
	const _wstring& _strPrototypeTag, 
	void* _pDesc
)
{
	m_pTexture = static_cast<CTexture*>(CGameInstance::Clone_Prototype
	(
		_iPrototypeLevelIndex,
		_strPrototypeTag,
		_pDesc
	));
	if (m_pTexture == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CCubeComponent::Bind_Material
(
	const _char* _pConstantName, 
	_uint _iTextureIndex
)
{
	m_pConstantName = _pConstantName;
	m_iTextureIndex = _iTextureIndex;

	return S_OK;
}

HRESULT CCubeComponent::Bind_Texture()
{
	if (m_pTexture == nullptr)
		return E_FAIL;

	return m_pTexture->Bind_ShaderResource(m_pShader, m_pConstantName, m_iTextureIndex);
}

CCubeComponent* CCubeComponent::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CCubeComponent* pInstance = new CCubeComponent(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Create : CCubeComponent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CCubeComponent::Clone(void* _pDesc)
{
	CCubeComponent* pInstance = new CCubeComponent(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Create : CCubeComponent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCubeComponent::Free()
{
	__super::Free();

	Safe_Release(m_pTexture);
	Safe_Release(m_pVIBuffer);
}