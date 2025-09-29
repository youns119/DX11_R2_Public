#include "pch.h"
#include "CAssaultRifle.h"

#include "CGameInstance.h"
#include "CLevel.h"
#include "CSkeletalMeshComponent.h"
#include "CShader.h"

#include "ELevel.h"

CAssaultRifle::CAssaultRifle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CActor{ _pDevice, _pContext }
{
}

CAssaultRifle::CAssaultRifle(const CAssaultRifle& _Prototype)
	: CActor(_Prototype)
{
}

HRESULT CAssaultRifle::Init()
{
	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAssaultRifle::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	if (FAILED(__super::Init(_pDesc)))
		return E_FAIL;

	m_pMeshComponent->Set_CurrAnim(m_pMeshComponent->Get_AnimIndex("Idle"), true);

	return S_OK;
}

void CAssaultRifle::Pre_Update(_float _fTimeDelta)
{
	__super::Pre_Update(_fTimeDelta);
}

void CAssaultRifle::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);
}

void CAssaultRifle::Post_Update(_float _fTimeDelta)
{
	__super::Post_Update(_fTimeDelta);
}

HRESULT CAssaultRifle::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAssaultRifle::Ready_Component()
{
	// SkeletalMeshComponent
	FSkeletalMeshComponent tSkeletalMeshComponent{};
	tSkeletalMeshComponent.pOwner = this;
	tSkeletalMeshComponent.eRenderType = ERender::RENDER_NONBLEND;
	tSkeletalMeshComponent.bCulling = true;
	tSkeletalMeshComponent.fCullingRange = 10.f;
	tSkeletalMeshComponent.szAnimName = "Idle";

	m_pMeshComponent = static_cast<CSkeletalMeshComponent*>(__super::Add_ActorComponent
	(
		static_cast<_uint>(ELevel::LEVEL_STAGE1),
		TEXT("SkeletalMeshComponent_AssaultRifle_Earth"),
		TEXT("SkeletalMeshComponent_AssaultRifle_Earth"),
		&tSkeletalMeshComponent
	));
	if (m_pMeshComponent == nullptr)
		return E_FAIL;

	Set_RootComponent(m_pMeshComponent);

	return S_OK;
}

HRESULT CAssaultRifle::Ready_Resource()
{
	// MeshComponent
	if (FAILED(m_pMeshComponent->Add_Shader
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		L"Shader_VtxSkeletalMesh"
	)))
		return E_FAIL;

	for (_uint i = 0; i < m_pMeshComponent->Get_NumMesh(); i++)
		if (FAILED(m_pMeshComponent->Bind_Material(i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)) ||
			FAILED(m_pMeshComponent->Bind_Material(i, aiTextureType_NORMALS, "g_NormalTexture", 0)))
			return E_FAIL;

	return S_OK;
}

CAssaultRifle* CAssaultRifle::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CAssaultRifle* pInstance = new CAssaultRifle(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Create : CAssaultRifle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CAssaultRifle::Clone(void* _pDesc)
{
	CAssaultRifle* pInstance = new CAssaultRifle(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Create : CAssaultRifle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAssaultRifle::Free()
{
	__super::Free();

	Safe_Release(m_pMeshComponent);
}