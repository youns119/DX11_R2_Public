#include "CSkyBox.h"

#include "CGameInstance.h"
#include "CCubeComponent.h"

CSkyBox::CSkyBox(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CActor{_pDevice, _pContext}
{
}

CSkyBox::CSkyBox(const CSkyBox& _Prototype)
	: CActor(_Prototype)
{
}

HRESULT CSkyBox::Init()
{
	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkyBox::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	FSkyBox* pSkyBoxDesc = static_cast<FSkyBox*>(_pDesc);

	FCubeComponent tCubeComponent{};
	tCubeComponent.pOwner = this;
	tCubeComponent.eRenderType = pSkyBoxDesc->eRenderType;

	m_pCubeComponent = CCubeComponent::Create(m_pDevice, m_pContext);
	if (m_pCubeComponent == nullptr ||
		FAILED(m_pCubeComponent->Init(&tCubeComponent)))
		return E_FAIL;

	Add_ActorComponent(g_strCubeComponent, m_pCubeComponent);
	Set_RootComponent(m_pCubeComponent);

	FActor tActor{};
	tActor.pOuter = pSkyBoxDesc->pOuter;

	if (FAILED(__super::Init(&tActor)))
		return E_FAIL;

	return S_OK;
}

void CSkyBox::Update(_float _fTimeDelta)
{
	_float4 vCamPosition = CGameInstance::Get_CamPos();

	m_pRootComponent->Update_Transform
	(
		XMVectorSet(1.f, 1.f, 1.f, 0.f),
		XMVectorSet(0.f, 0.f, 0.f, 0.f),
		XMLoadFloat4(&vCamPosition)
	);

	__super::Update(_fTimeDelta);
}

void CSkyBox::Free()
{
	__super::Free();

	Safe_Release(m_pCubeComponent);
}