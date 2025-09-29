#include "pch.h"
#include "CBuilding_A.h"

#include "CGameInstance.h"
#include "CLevel.h"
#include "CStaticMeshComponent.h"
#include "CShader.h"

#include "ELevel.h"

CBuilding_A::CBuilding_A(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CActor{ _pDevice, _pContext }
{
}

CBuilding_A::CBuilding_A(const CBuilding_A& _Prototype)
	: CActor(_Prototype)
{
}

HRESULT CBuilding_A::Init()
{
	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBuilding_A::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	if (FAILED(__super::Init(_pDesc)))
		return E_FAIL;

	return S_OK;
}

void CBuilding_A::Pre_Update(_float _fTimeDelta)
{
	__super::Pre_Update(_fTimeDelta);
}

void CBuilding_A::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);
}

void CBuilding_A::Post_Update(_float _fTimeDelta)
{
	__super::Post_Update(_fTimeDelta);
}

HRESULT CBuilding_A::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBuilding_A::Ready_Component()
{
	// StaticMeshComponent
	FStaticMeshComponent tStaticActorComponent{};
	tStaticActorComponent.pOwner = this;
	tStaticActorComponent.eRenderType = ERender::RENDER_NONBLEND;
	tStaticActorComponent.bCulling = true;
	tStaticActorComponent.fCullingRange = 30.f;

	m_pMeshComponent = static_cast<CStaticMeshComponent*>(__super::Add_ActorComponent
	(
		static_cast<_uint>(ELevel::LEVEL_STAGE1),
		TEXT("StaticMeshComponent_Building_A"),
		TEXT("StaticMeshComponent_Building_A"),
		&tStaticActorComponent
	));
	if (m_pMeshComponent == nullptr)
		return E_FAIL;

	m_pMeshComponent->Set_Pass(1);
	Set_RootComponent(m_pMeshComponent);

	return S_OK;
}

HRESULT CBuilding_A::Ready_Resource()
{
	// MeshComponent
	if (FAILED(m_pMeshComponent->Add_Shader
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		L"Shader_VtxStaticMesh"
	)))
		return E_FAIL;

	for (_uint i = 0; i < m_pMeshComponent->Get_NumMesh(); i++)
		if (FAILED(m_pMeshComponent->Bind_Material(i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)) ||
			FAILED(m_pMeshComponent->Bind_Material(i, aiTextureType_NORMALS, "g_NormalTexture", 0)))
			return E_FAIL;

	return S_OK;
}

CBuilding_A* CBuilding_A::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CBuilding_A* pInstance = new CBuilding_A(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Create : CBuilding_A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CBuilding_A::Clone(void* _pDesc)
{
	CBuilding_A* pInstance = new CBuilding_A(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Create : CBuilding_A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBuilding_A::Free()
{
	__super::Free();

	Safe_Release(m_pMeshComponent);
}