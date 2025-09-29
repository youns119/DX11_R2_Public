#include "CParticleComponent.h"

#include "CVIBuffer_Instancing.h"
#include "CShader.h"
#include "CTexture.h"

CParticleComponent::CParticleComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CPrimitiveComponent{ _pDevice, _pContext }
{
	m_strName = g_strParticleComponent;
}

CParticleComponent::CParticleComponent(const CParticleComponent& _Prototype)
	: CPrimitiveComponent(_Prototype)
{
}

HRESULT CParticleComponent::Init()
{
	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticleComponent::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	if (FAILED(__super::Init(_pDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticleComponent::Render()
{
	return S_OK;
}

HRESULT CParticleComponent::Add_VIBuffer
(
	_uint _iPrototypeLevelIndex, 
	const _wstring& _strPrototypeTag, 
	void* _pDesc
)
{
	return S_OK;
}

HRESULT CParticleComponent::Add_Texture
(
	_uint _iPrototypeLevelIndex, 
	_uint _iTextureIndex, 
	const _wstring& _strPrototypeTag, 
	void* _pDesc
)
{
	return S_OK;
}

HRESULT CParticleComponent::Bind_Texture
(
	_uint _iTextureType, 
	const _char* _pConstantName, 
	_uint _iTextureIndex
)
{
	return S_OK;
}

CParticleComponent* CParticleComponent::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CParticleComponent* pInstance = new CParticleComponent(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Create : CParticleComponent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CParticleComponent::Clone(void* _pDesc)
{
	CParticleComponent* pInstance = new CParticleComponent(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Create : CParticleComponent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticleComponent::Free()
{
	__super::Free();

	Safe_Release(m_pTexture);
	Safe_Release(m_pVIBuffer);
}