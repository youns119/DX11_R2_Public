#include "CActorComponent.h"

#include "CActor.h"

_uint CActorComponent::m_iStaticID = 0;

CActorComponent::CActorComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice{ _pDevice }
	, m_pContext{ _pContext }
	, m_iComponentID{ m_iStaticID++ }
	, m_bCloned{ false }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CActorComponent::CActorComponent(const CActorComponent& _Prototype)
	: m_pDevice{ _Prototype.m_pDevice }
	, m_pContext{ _Prototype.m_pContext }
	, m_strName{ _Prototype.m_strName }
	, m_iComponentID{ m_iStaticID++ }
	, m_bCloned{ true }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CActorComponent::Init()
{
	return S_OK;
}

HRESULT CActorComponent::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	FActorComponent* pActorComponentDesc = static_cast<FActorComponent*>(_pDesc);
	m_pOwner = pActorComponentDesc->pOwner;

	Safe_AddRef(m_pOwner);

	return S_OK;
}

void CActorComponent::Pre_Update(_float _fTimeDelta)
{
}

void CActorComponent::Update(_float _fTimeDelta)
{
}

void CActorComponent::Post_Update(_float _fTimeDelta)
{
}

HRESULT CActorComponent::Render()
{
	return S_OK;
}

void CActorComponent::Clear()
{
	Safe_Release(m_pOwner);
}

void CActorComponent::Free()
{
	__super::Free();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}