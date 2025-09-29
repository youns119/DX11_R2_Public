#include "pch.h"
#include "CMonsterHP.h"

#include "CGameInstance.h"
#include "CUIComponent.h"

#include "ELevel.h"

CMonsterHP::CMonsterHP(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CActor{ _pDevice, _pContext }
{
}

CMonsterHP::CMonsterHP(const CMonsterHP& _Prototype)
	: CActor(_Prototype)
{
}

HRESULT CMonsterHP::Init()
{
	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonsterHP::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return E_FAIL;

	if (FAILED(__super::Init(_pDesc)))
		return E_FAIL;

	return S_OK;
}

void CMonsterHP::Pre_Update(_float _fTimeDelta)
{
	_vector vWorldPos = XMLoadFloat3(&m_vCurrPos);

	_float4x4 matSrcView = CGameInstance::Get_TransformFloat4x4(ED3DTransform::D3DTS_VIEW);
	_float4x4 matSrcProj = CGameInstance::Get_TransformFloat4x4(ED3DTransform::D3DTS_PROJ);

	_matrix matWorld = XMMatrixIdentity();
	_matrix matView = XMLoadFloat4x4(&matSrcView);
	_matrix matProj = XMLoadFloat4x4(&matSrcProj);

	_matrix matWVP = matWorld * matView * matProj;

	_vector vClipPos = XMVector4Transform(vWorldPos, matWVP);

	_float4 vNDCPos{};
	XMStoreFloat4(&vNDCPos, vClipPos);

	vNDCPos.x /= vNDCPos.w;
	vNDCPos.y /= vNDCPos.w;
	vNDCPos.z /= vNDCPos.w;

	_float fScreenX = (vNDCPos.x + 1.0f) * 0.5f * g_iWinSizeX;
	_float fScreenY = (1.0f - vNDCPos.y) * 0.5f * g_iWinSizeY;

	fScreenX -= 0.5f * g_iWinSizeX;
	fScreenY = -fScreenY + 0.5f * g_iWinSizeY;

	m_UIs[static_cast<_uint>(EMonsterHP::MONSTER_BACK)]->Set_RelativePosition(XMVectorSet(fScreenX, fScreenY, 0.f, 1.f));
	m_UIs[static_cast<_uint>(EMonsterHP::MONSTER_HP)]->Set_RelativePosition(XMVectorSet(fScreenX, fScreenY, 0.f, 1.f));

	__super::Pre_Update(_fTimeDelta);
}

void CMonsterHP::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);
}

void CMonsterHP::Post_Update(_float _fTimeDelta)
{
	__super::Post_Update(_fTimeDelta);
}

HRESULT CMonsterHP::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonsterHP::Ready_Component()
{
	// MonsterHP_Back
	FUIComponent tUIComponent{};
	tUIComponent.pOwner = this;
	tUIComponent.eRenderType = ERender::RENDER_UI;
	tUIComponent.vScale = _float3(200.f, 10.f, 0.f);
	tUIComponent.vPosition = _float3(0.f, 0.f, 0.f);
	tUIComponent.fNear = 0.f;
	tUIComponent.fFar = 1.f;

	m_UIs[static_cast<_uint>(EMonsterHP::MONSTER_BACK)] = static_cast<CUIComponent*>(__super::Add_ActorComponent
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		g_strUIComponent,
		TEXT("Bar_Back"),
		&tUIComponent
	));
	if (m_UIs[static_cast<_uint>(EMonsterHP::MONSTER_BACK)] == nullptr)
		return E_FAIL;

	// MonsterHP_HP
	tUIComponent.pOwner = this;
	tUIComponent.eRenderType = ERender::RENDER_UI;
	tUIComponent.vScale = _float3(195.f, 5.f, 0.f);
	tUIComponent.vPosition = _float3(0.f, 0.f, 0.f);
	tUIComponent.fNear = 0.f;
	tUIComponent.fFar = 1.f;

	m_UIs[static_cast<_uint>(EMonsterHP::MONSTER_HP)] = static_cast<CUIComponent*>(__super::Add_ActorComponent
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		g_strUIComponent,
		TEXT("Bar_HP"),
		&tUIComponent
	));
	if (m_UIs[static_cast<_uint>(EMonsterHP::MONSTER_HP)] == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CMonsterHP::Ready_Resource()
{
	// MonsterHP_Back
	if (FAILED(m_UIs[static_cast<_uint>(EMonsterHP::MONSTER_BACK)]->Add_Shader
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		L"Shader_VtxPosTex"
	)))
		return E_FAIL;

	if (FAILED(m_UIs[static_cast<_uint>(EMonsterHP::MONSTER_BACK)]->Add_Texture
	(
		static_cast<_uint>(ELevel::LEVEL_STAGE1),
		TEXT("Texture_Bar_Back")
	)))
		return E_FAIL;

	if (FAILED(m_UIs[static_cast<_uint>(EMonsterHP::MONSTER_BACK)]->Bind_Material("g_Texture", 0)))
		return E_FAIL;

	// MonsterHP_HP
	if (FAILED(m_UIs[static_cast<_uint>(EMonsterHP::MONSTER_HP)]->Add_Shader
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		L"Shader_VtxPosTex"
	)))
		return E_FAIL;

	if (FAILED(m_UIs[static_cast<_uint>(EMonsterHP::MONSTER_HP)]->Add_Texture
	(
		static_cast<_uint>(ELevel::LEVEL_STAGE1),
		TEXT("Texture_Bar_HP")
	)))
		return E_FAIL;

	if (FAILED(m_UIs[static_cast<_uint>(EMonsterHP::MONSTER_HP)]->Bind_Material("g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

CMonsterHP* CMonsterHP::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CMonsterHP* pInstance = new CMonsterHP(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed to Create : CMonsterHP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CMonsterHP::Clone(void* _pDesc)
{
	CMonsterHP* pInstance = new CMonsterHP(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed to Create : CMonsterHP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonsterHP::Free()
{
	__super::Free();

	for (_uint i = 0; i < static_cast<_uint>(EMonsterHP::MONSTER_END); i++)
		Safe_Release(m_UIs[i]);
}