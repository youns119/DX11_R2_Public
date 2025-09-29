#include "pch.h"
#include "CLight_Directional.h"

#include "CGameInstance.h"
#include "CLightComponent.h"

CLight_Directional::CLight_Directional(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CLight{ _pDevice, _pContext }
{
}

CLight_Directional::CLight_Directional(const CLight_Directional& _Prototype)
	: CLight(_Prototype)
{
}

HRESULT CLight_Directional::Init()
{
	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLight_Directional::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	FLight* pLightDesc = static_cast<FLight*>(_pDesc);
	pLightDesc->eLight = ELight::LIGHT_DIRECTIONAL;
	pLightDesc->vPosition = _float3(0.f, 0.f, 0.f);
	pLightDesc->vDirection = _float3(45.f, 45.f, 45.f);
	pLightDesc->vDiffuse = _float4(0.17f, 0.17f, 0.17f, 1.f);
	pLightDesc->vAmbient = _float4(0.22f, 0.22f, 0.22f, 1.f);
	pLightDesc->vSpecular = _float4(0.1f, 0.1f, 0.1f, 1.f);

	if (FAILED(__super::Init(pLightDesc)))
		return E_FAIL;

	return S_OK;
}

void CLight_Directional::Pre_Update(_float _fTimeDelta) 
{
	__super::Pre_Update(_fTimeDelta);
}

void CLight_Directional::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);
}

void CLight_Directional::Post_Update(_float _fTimeDelta)
{
	__super::Post_Update(_fTimeDelta);
}

HRESULT CLight_Directional::Ready_Component()
{
	return S_OK;
}

CLight_Directional* CLight_Directional::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CLight_Directional* pInstance = new CLight_Directional(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Create : CLight_Directional");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CLight_Directional::Clone(void* _pDesc)
{
	CLight_Directional* pInstance = new CLight_Directional(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Create : CLight_Directional");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLight_Directional::Free()
{
	__super::Free();
}