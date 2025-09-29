#include "CLight.h"

#include "CLightComponent.h"

#include "FSceneComponent.h"

CLight::CLight(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    : CActor{ _pDevice, _pContext }
{
}

CLight::CLight(const CLight& _Prototype)
    : CActor(_Prototype)
{
}

HRESULT CLight::Init()
{
    if (FAILED(__super::Init()))
        return E_FAIL;

    return S_OK;
}

HRESULT CLight::Init(void* _pDesc)
{
    if (_pDesc == nullptr)
        return S_OK;

    FLight* pLightDesc = static_cast<FLight*>(_pDesc);

    FLightComponent tLightComponent{};
    tLightComponent.pOwner = this;
    tLightComponent.eLight = pLightDesc->eLight;
    tLightComponent.vPosition = pLightDesc->vPosition;
    tLightComponent.vDirection = pLightDesc->vDirection;
    tLightComponent.vDiffuse = pLightDesc->vDiffuse;
    tLightComponent.vAmbient = pLightDesc->vAmbient;
    tLightComponent.vSpecular = pLightDesc->vSpecular;
    tLightComponent.fRange = pLightDesc->fRange;

    m_pLightComponent = CLightComponent::Create(m_pDevice, m_pContext);
    if (m_pLightComponent == nullptr ||
        FAILED(m_pLightComponent->Init(&tLightComponent)))
        return E_FAIL;

    Add_ActorComponent(g_strLightComponent, m_pLightComponent);
    Set_RootComponent(m_pLightComponent);

    FActor tActor{};
    tActor.pOuter = pLightDesc->pOuter;

    if (FAILED(__super::Init(&tActor)))
        return E_FAIL;

    return S_OK;
}

void CLight::Set_LightOnOff(_bool _bOnOff)
{
    m_pLightComponent->Set_OnOff(_bOnOff);
}

void CLight::Free()
{
    __super::Free();

    Safe_Release(m_pLightComponent);
}