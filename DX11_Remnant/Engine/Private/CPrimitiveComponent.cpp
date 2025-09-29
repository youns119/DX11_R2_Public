#include "CPrimitiveComponent.h"

#include "CGameInstance.h"
#include "CActor.h"
#include "CShader.h"

CPrimitiveComponent::CPrimitiveComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    : CSceneComponent{ _pDevice, _pContext }
{
}

CPrimitiveComponent::CPrimitiveComponent(const CPrimitiveComponent& _Prototype)
    : CSceneComponent(_Prototype)
    , m_matView{ _Prototype.m_matView }
    , m_matProj{ _Prototype.m_matProj }
{
}

HRESULT CPrimitiveComponent::Init()
{
    if (FAILED(__super::Init()))
        return E_FAIL;

    XMStoreFloat4x4(&m_matView, XMMatrixIdentity());
    XMStoreFloat4x4(&m_matProj, XMMatrixIdentity());

    return S_OK;
}

HRESULT CPrimitiveComponent::Init(void* _pDesc)
{
    if (_pDesc == nullptr)
        return E_FAIL;

    FPrimitiveComponent* pPrimitiveDesc = static_cast<FPrimitiveComponent*>(_pDesc);
    m_eRenderType = pPrimitiveDesc->eRenderType;
    m_bCulling = pPrimitiveDesc->bCulling;
    m_fCullingRange = pPrimitiveDesc->fCullingRange;

    if (FAILED(__super::Init(_pDesc)))
        return E_FAIL;

    return S_OK;
}

void CPrimitiveComponent::Pre_Update(_float _fTimeDelta)
{
    __super::Pre_Update(_fTimeDelta);
}

void CPrimitiveComponent::Update(_float _fTimeDelta)
{
    __super::Update(_fTimeDelta);
}

void CPrimitiveComponent::Post_Update(_float _fTimeDelta)
{
    if (m_pOwner->Get_Show() && m_bShow)
    {
        if (m_bCulling)
        {
            _vector vPosition = Get_TransformState(ETransformState::TRANSFORMSTATE_POS);

            if (CGameInstance::IsIn_WorldSpace(vPosition, m_fCullingRange))
                CGameInstance::Add_RenderGroup(m_eRenderType, this);
        }
        else
            CGameInstance::Add_RenderGroup(m_eRenderType, this);
    }

    __super::Post_Update(_fTimeDelta);
}

HRESULT CPrimitiveComponent::Render()
{
    return S_OK;
}

HRESULT CPrimitiveComponent::Add_Shader(_uint _iPrototypeLevelIndex, const _wstring& _strPrototypeTag)
{
    CShader* pShader = static_cast<CShader*>(CGameInstance::Clone_Prototype
    (
        _iPrototypeLevelIndex,
        _strPrototypeTag
    ));
    if (pShader == nullptr)
        return E_FAIL;

    Safe_Release(m_pShader);

    m_pShader = pShader;

    return S_OK;
}

HRESULT CPrimitiveComponent::Begin_Shader()
{
    if (m_pShader == nullptr)
        return E_FAIL;

    m_pShader->Begin(m_iPassIndex);

    return S_OK;
}

HRESULT CPrimitiveComponent::Bind_ShaderResource()
{
    if (m_pShader == nullptr)
        return S_OK;

    m_matView = CGameInstance::Get_TransformFloat4x4(ED3DTransform::D3DTS_VIEW);
    m_matProj = CGameInstance::Get_TransformFloat4x4(ED3DTransform::D3DTS_PROJ);

    if (FAILED(m_pShader->Bind_Matrix("g_matView", &m_matView)) ||
        FAILED(m_pShader->Bind_Matrix("g_matProj", &m_matProj)))
        return E_FAIL;

    if (FAILED(m_pShader->Bind_Matrix("g_matWorld", this)))
        return E_FAIL;

    return S_OK;
}

void CPrimitiveComponent::Free()
{
    __super::Free();

    Safe_Release(m_pShader);
}