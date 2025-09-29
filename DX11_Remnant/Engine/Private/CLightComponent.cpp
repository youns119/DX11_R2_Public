#include "CLightComponent.h"

#include "CGameInstance.h"
#include "CShader.h"
#include "CVIBuffer_Rect.h"

CLightComponent::CLightComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    : CSceneComponent{ _pDevice, _pContext }
{
    m_strName = g_strLightComponent;
}

CLightComponent::CLightComponent(const CLightComponent& _Prototype)
    : CSceneComponent(_Prototype)
    , m_bOnOff{ _Prototype.m_bOnOff }
{
}

HRESULT CLightComponent::Init()
{
    if (FAILED(__super::Init()))
        return E_FAIL;

    return S_OK;
}

HRESULT CLightComponent::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

    FLightComponent* pLightDesc = static_cast<FLightComponent*>(_pDesc);
    m_eLight = pLightDesc->eLight;
    m_vDiffuse = pLightDesc->vDiffuse;
    m_vAmbient = pLightDesc->vAmbient;
    m_vSpecular = pLightDesc->vSpecular;
    m_fRange = pLightDesc->fRange;

    FSceneComponent tSceneComponent{};
    tSceneComponent.pOwner = pLightDesc->pOwner;
    tSceneComponent.vPosition = pLightDesc->vPosition;
    tSceneComponent.vRotation = pLightDesc->vDirection;

    if (m_eLight == ELight::LIGHT_DIRECTIONAL)
        m_iPassIndex = 1;
    else if(m_eLight == ELight::LIGHT_DIRECTIONAL)
        m_iPassIndex = 2;

	if (FAILED(__super::Init(&tSceneComponent)))
		return E_FAIL;

	return S_OK;
}

void CLightComponent::Post_Update(_float _fTimeDelta)
{
    if(m_bOnOff) CGameInstance::Add_Light(this);

    __super::Post_Update(_fTimeDelta);
}

HRESULT CLightComponent::Render(CShader* _pShader, CVIBuffer_Rect* _pVIBuffer)
{
    if (m_iPassIndex == 1)
    {
        _vector vLook = Get_TransformState(ETransformState::TRANSFORMSTATE_LOOK);

        if (FAILED(_pShader->Bind_RawValue("g_vLightDir", &vLook, sizeof(_float4))))
            return E_FAIL;
    }
    else if (m_iPassIndex == 2)
    {
        _float3 vPosition = Get_Transform(ETransform::TRANSFORM_POSITION);
        _float4 vLightPosition = _float4(vPosition.x, vPosition.y, vPosition.z, 1.f);

        if (FAILED(_pShader->Bind_RawValue("g_vLightPos", &vLightPosition, sizeof(_float4))) ||
            FAILED(_pShader->Bind_RawValue("g_fRange", &m_fRange, sizeof(_float))))
            return E_FAIL;
    }

    if (FAILED(_pShader->Bind_RawValue("g_vLightDiffuse", &m_vDiffuse, sizeof(_float4))) ||
        FAILED(_pShader->Bind_RawValue("g_vLightAmbient", &m_vAmbient, sizeof(_float4))) ||
        FAILED(_pShader->Bind_RawValue("g_vLightSpecular", &m_vSpecular, sizeof(_float4))))
        return E_FAIL;

    _pShader->Begin(m_iPassIndex);

    _pVIBuffer->Render();

    return S_OK;
}

CLightComponent* CLightComponent::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
    CLightComponent* pInstance = new CLightComponent(_pDevice, _pContext);

    if (FAILED(pInstance->Init()))
    {
        MSG_BOX("Failed To Create : CLightComponent");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CClonable* CLightComponent::Clone(void* _pDesc)
{
    CLightComponent* pInstance = new CLightComponent(*this);

    if (FAILED(pInstance->Init(_pDesc)))
    {
        MSG_BOX("Failed To Create : CLightComponent");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLightComponent::Free()
{
    __super::Free();
}