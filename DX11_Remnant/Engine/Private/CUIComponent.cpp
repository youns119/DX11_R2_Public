#include "CUIComponent.h"

#include "CGameInstance.h"
#include "CVIBuffer_Rect.h"
#include "CTexture.h"
#include "CShader.h"

CUIComponent::CUIComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CPrimitiveComponent{_pDevice, _pContext}
{
}

CUIComponent::CUIComponent(const CUIComponent& _Prototype)
	: CPrimitiveComponent(_Prototype)
	, m_pVIBuffer{ _Prototype.m_pVIBuffer }
{
	Safe_AddRef(m_pVIBuffer);
}

HRESULT CUIComponent::Init()
{
	if (FAILED(__super::Init()))
		return E_FAIL;

	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (m_pVIBuffer == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CUIComponent::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	FUIComponent* pUIComponentDesc = static_cast<FUIComponent*>(_pDesc);

	_uint2 vViewportSize = CGameInstance::Get_ViewportSize();

	_float3 vPosition = pUIComponentDesc->vPosition;
	vPosition = _float3(vPosition.x - (vViewportSize.x * 0.5f), -vPosition.y + (vViewportSize.y * 0.5f), 0.f);
	pUIComponentDesc->vPosition = vPosition;

	XMStoreFloat4x4
	(
		&m_matProj, 
		XMMatrixOrthographicLH
		(
			static_cast<_float>(vViewportSize.x), 
			static_cast<_float>(vViewportSize.y),
			pUIComponentDesc->fNear,
			pUIComponentDesc->fFar
		)
	);

	if (FAILED(__super::Init(pUIComponentDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIComponent::Render()
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

HRESULT CUIComponent::Add_Texture
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

HRESULT CUIComponent::Bind_Material(const _char* _pConstantName, _uint _iTextureIndex)
{
	m_pConstantName = _pConstantName;
	m_iTextureIndex = _iTextureIndex;

	return S_OK;
}

HRESULT CUIComponent::Bind_Texture()
{
	if (m_pTexture == nullptr)
		return E_FAIL;

	return m_pTexture->Bind_ShaderResource(m_pShader, m_pConstantName, m_iTextureIndex);
}

HRESULT CUIComponent::Bind_ShaderResource()
{
	if (m_pShader == nullptr || !m_bShow)
		return S_OK;

	if (FAILED(m_pShader->Bind_Matrix("g_matView", &m_matView)) ||
		FAILED(m_pShader->Bind_Matrix("g_matProj", &m_matProj)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_matWorld", this)))
		return E_FAIL;

	return S_OK;
}

CUIComponent* CUIComponent::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CUIComponent* pInstance = new CUIComponent(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Create : CUIComponent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CUIComponent::Clone(void* _pDesc)
{
	CUIComponent* pInstance = new CUIComponent(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Create : CUIComponent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIComponent::Free()
{
	__super::Free();

	Safe_Release(m_pTexture);
	Safe_Release(m_pVIBuffer);
}