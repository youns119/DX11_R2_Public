#include "CTerrain.h"

#include "CTerrainComponent.h"

CTerrain::CTerrain(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CActor{ _pDevice, _pContext }
{
}

CTerrain::CTerrain(const CTerrain& _Prototype)
	: CActor(_Prototype)
{
}

HRESULT CTerrain::Init()
{
	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	FTerrain* pTerrainDesc = static_cast<FTerrain*>(_pDesc);
	
	FTerrainComponent tTerrainComponent{};
	tTerrainComponent.pOwner = this;
	tTerrainComponent.eRenderType = pTerrainDesc->eRenderType;
	tTerrainComponent.iNumTexture = pTerrainDesc->iNumTexture;
	tTerrainComponent.vScale = pTerrainDesc->vScale;
	tTerrainComponent.vRotation = pTerrainDesc->vRotation;
	tTerrainComponent.vPosition = pTerrainDesc->vPosition;

	m_pTerrainComponent = CTerrainComponent::Create(m_pDevice, m_pContext);
	if (m_pTerrainComponent == nullptr ||
		FAILED(m_pTerrainComponent->Init(&tTerrainComponent)))
		return E_FAIL;

	Add_ActorComponent(g_strTerrainComponent, m_pTerrainComponent);
	Set_RootComponent(m_pTerrainComponent);

	FActor tActor{};
	tActor.pOuter = pTerrainDesc->pOuter;

	if (FAILED(__super::Init(&tActor)))
		return E_FAIL;

	return S_OK;
}

_float CTerrain::Get_Height(const _float3* _pPos)
{
	_uint2 iVerticeNum = m_pTerrainComponent->Get_VerticeNum();
	const _float3* pTerrainPos = m_pTerrainComponent->Get_VerticePos();

	_ulong dwIndex{};
	dwIndex = _ulong(_pPos->z)* iVerticeNum.x + _ulong(_pPos->x);

	_float fRatioX = (_pPos->x - pTerrainPos[dwIndex + iVerticeNum.x].x);
	_float fRatioZ = (pTerrainPos[dwIndex + iVerticeNum.y].z - _pPos->z);

	_float4 vPlane{};

	// ¿ì»ó´Ü
	if (fRatioX > fRatioZ)
	{
		XMStoreFloat4(&vPlane, XMPlaneFromPoints
		(
			XMLoadFloat3(&pTerrainPos[dwIndex + iVerticeNum.x]),
			XMLoadFloat3(&pTerrainPos[dwIndex + iVerticeNum.x + 1]),
			XMLoadFloat3(&pTerrainPos[dwIndex + 1])
		));
	}
	else
	{
		XMStoreFloat4(&vPlane, XMPlaneFromPoints
		(
			XMLoadFloat3(&pTerrainPos[dwIndex + iVerticeNum.x]),
			XMLoadFloat3(&pTerrainPos[dwIndex + 1]),
			XMLoadFloat3(&pTerrainPos[dwIndex])
		));
	}

	_vector vPos = XMLoadFloat3(_pPos);

	return (-vPlane.x * vPos.m128_f32[0] - vPlane.z * vPos.m128_f32[2] - vPlane.w) / vPlane.y;
}

void CTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pTerrainComponent);
}