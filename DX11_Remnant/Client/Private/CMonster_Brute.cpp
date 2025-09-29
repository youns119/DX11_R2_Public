#include "pch.h"
#include "CMonster_Brute.h"

#include "CGameInstance.h"
#include "CLevel.h"
#include "CPlayer.h"
#include "CTerrain.h"
#include "CSkeletalMeshComponent.h"
#include "CColliderComponent.h"
#include "CSpringArmComponent.h"
#include "CCharacterMovementComponent.h"
#include "CShader.h"
#include "CMathUtility.h"

#include "CMonsterUI.h"

#include "ELevel.h"

CMonster_Brute::CMonster_Brute(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    : CCharacter{ _pDevice, _pContext }
{
}

CMonster_Brute::CMonster_Brute(const CMonster_Brute& _Prototype)
    : CCharacter(_Prototype)
    , m_fMaxHP{ _Prototype.m_fMaxHP }
    , m_fCurrHP{ _Prototype.m_fCurrHP }
{
}

HRESULT CMonster_Brute::Init()
{
    if (FAILED(__super::Init()))
        return E_FAIL;

    m_fMaxHP = 100.f;
    m_fCurrHP = m_fMaxHP;

    return S_OK;
}

HRESULT CMonster_Brute::Init(void* _pDesc)
{
    if (_pDesc == nullptr)
        return S_OK;

    FCharacter* pCharacterDesc = static_cast<FCharacter*>(_pDesc);
    pCharacterDesc->fMoveSpeed = 6.f;
    pCharacterDesc->fRotationSpeed = 10.f;
    pCharacterDesc->fMoveAccel = 6.f;
    pCharacterDesc->fMoveDecel = 4.f;
    pCharacterDesc->fMaxVelocity = 6.f;

    if (FAILED(__super::Init(pCharacterDesc)))
        return E_FAIL;

    return S_OK;
}

void CMonster_Brute::Pre_Update(_float _fTimeDelta)
{
    //CActor* pTarget = CGameInstance::Get_Controller();
    //if (pTarget == nullptr)
    //    return;

    CPlayer* pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Controller());
    if (pPlayer == nullptr)
    {
        __super::Pre_Update(_fTimeDelta);
        return;
    }

    _vector vPos = m_pRootComponent->Get_TransformState(ETransformState::TRANSFORMSTATE_POS);
    _vector vTargetPos = pPlayer->Get_RootComponent()->Get_TransformState(ETransformState::TRANSFORMSTATE_POS);
    _vector vDist = vTargetPos - vPos;
    _vector vLook = m_pRootComponent->Get_TransformState(ETransformState::TRANSFORMSTATE_LOOK);

    _float fDist = XMVectorGetX(XMVector3Length(vDist));
    _float fDegree = CMathUtility::GetBetweenAngle(vLook, vDist);

    if (m_fDeath >= 2.f)
    {
        m_bShow = false;
        return;
    }

    if (fDist <= 20.f && fDist > 2.f)
    {
        if (CGameInstance::Mouse_Press(EMouseKey::MOUSEKEY_LB))
        {
            if (pPlayer->Get_Shoot())
            {
                m_iHit++;
                pPlayer->Set_Shoot(false);
            }
        }
    }

    if (m_iHit >= 5)
    {
        if (!m_bDeath)
        {
            m_pMeshComponent->Set_CurrAnim(m_pMeshComponent->Get_AnimIndex("Impact_Death_Head"), false);
            SOUND->Play_Sound(L"Pan_Brute_Death.wav", EChannel::ECHANNEL_ENEMY, 0.3f);

            m_bDeath = true;
        }

        m_fDeath += _fTimeDelta;
        return;
    }

    if (m_fAttack > m_fAttackAcc)
    {
        m_bAttack = false;
        m_fAttack = 0.f;
    }

    if (m_bAttack)
    {
        m_fAttack += _fTimeDelta;

        if (m_iAttackIndex == 0)
        {
            m_pMovementComponent->Add_Velocity(vLook, _fTimeDelta * 0.1f);

            if (m_fAttack >= m_fAttackTime && !m_bAttackSound)
            {
                SOUND->Play_Sound(L"Pan_Brute_Chop.wav", EChannel::ECHANNEL_ENEMY, 0.3f);
                m_bAttackSound = true;
            }
        }
        else if (m_iAttackIndex == 1)
        {
            m_pMovementComponent->Add_Velocity(vLook, _fTimeDelta * 0.2f);

            if (m_fAttack >= m_fAttackTime && !m_bAttackSound)
            {
                SOUND->Play_Sound(L"Pan_Brute_Chop.wav", EChannel::ECHANNEL_ENEMY, 0.3f);
                m_bAttackSound = true;
            }
        }
        else
        {
            m_pMovementComponent->Add_Velocity(vLook, _fTimeDelta * 0.3f);

            if (m_fAttack >= m_fAttackTime && !m_bAttackSound)
            {
                SOUND->Play_Sound(L"Pan_Brute_Spin.wav", EChannel::ECHANNEL_ENEMY, 0.3f);
                m_bAttackSound = true;
            }
        }

        if (m_fAttack >= m_fAttackTime && fDist <= 2.f)
        {
            pPlayer->Damaged(10.f);
            m_fAttackTime = 100.f;

            CSkeletalMeshComponent* pPlayerMesh = pPlayer->Get_MeshComponent();
            pPlayerMesh->Set_CurrAnim(pPlayerMesh->Get_AnimIndex("2R_Impact_F"));
        }

        __super::Pre_Update(_fTimeDelta);

        return;
    }

    if (fDist <= 20.f && fDist > 2.f && !m_bAttack)
    {
        if (m_pTarget != nullptr)
            m_pTarget = pPlayer;

        m_pMovementComponent->Add_Velocity(vDist, _fTimeDelta);
        m_pMeshComponent->Set_CurrAnim(m_pMeshComponent->Get_AnimIndex("Walk_F"), true);

        m_fSoundTimeAcc += _fTimeDelta;

        if (m_fSoundTimeAcc >= m_fWalkSoundDist)
        {
            SOUND->Play_Sound(L"Pan_Brute_Walk.wav", EChannel::ECHANNEL_ENEMY, 0.05f);
            m_fSoundTimeAcc = 0.f;
        }
    }
    else if (fDist <= 2.f)
    {
        m_iAttackIndex = rand() % 3;

        if (m_iAttackIndex == 0)
        {
            m_pMeshComponent->Set_CurrAnim(m_pMeshComponent->Get_AnimIndex("Atk_Chop"), false);
            m_bAttack = true;
            m_bAttackSound = false;
            m_fAttackAcc = 3.7f;
            m_fAttackTime = 1.f;
        }
        else if (m_iAttackIndex == 1)
        {
            m_pMeshComponent->Set_CurrAnim(m_pMeshComponent->Get_AnimIndex("Atk_Ram"), false);
            m_bAttack = true;
            m_bAttackSound = false;
            m_fAttackAcc = 2.7f;
            m_fAttackTime = 0.5f;
        }
        else
        {
            m_pMeshComponent->Set_CurrAnim(m_pMeshComponent->Get_AnimIndex("Atk_Spin"), false);
            m_bAttack = true;
            m_bAttackSound = false;
            m_fAttackAcc = 4.2f;
            m_fAttackTime = 2.f;
        }
    }
    else
        m_pMeshComponent->Set_CurrAnim(m_pMeshComponent->Get_AnimIndex("Idle"), true);

    m_pMovementComponent->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDegree * _fTimeDelta);

    __super::Pre_Update(_fTimeDelta);
}

void CMonster_Brute::Update(_float _fTimeDelta)
{
    // Terrain Tracking
    {
        CActor* pActor = m_pOuter->Get_Actor(L"Layer_BackGround", L"Actor_Terrain_Test");
        CTerrain* pTerrain = static_cast<CTerrain*>(pActor);

        _vector vSrcPos = m_pRootComponent->Get_TransformState(ETransformState::TRANSFORMSTATE_POS);
        _float3 vPos{};
        XMStoreFloat3(&vPos, vSrcPos);

        _float fHeight = pTerrain->Get_Height(&vPos);
        m_pRootComponent->Update_Position(XMVectorSetY(vSrcPos, fHeight));
    }

    if (m_pMonsterUI)
    {
        _float3 vPos = m_pRootComponent->Get_Transform(ETransform::TRANSFORM_POSITION);
        m_pMonsterUI->Set_Pos(vPos);
    }

    __super::Update(_fTimeDelta);
}

void CMonster_Brute::Post_Update(_float _fTimeDelta)
{
    __super::Post_Update(_fTimeDelta);
}

HRESULT CMonster_Brute::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CMonster_Brute::Ready_Component()
{
    // SkeletalMeshComponent
    FSkeletalMeshComponent tSkeletalMeshComponent{};
    tSkeletalMeshComponent.pOwner = this;
    tSkeletalMeshComponent.eRenderType = ERender::RENDER_NONBLEND;
    tSkeletalMeshComponent.bCulling = true;
    tSkeletalMeshComponent.fCullingRange = 10.f;
    tSkeletalMeshComponent.szAnimName = "Idle";
    tSkeletalMeshComponent.szRootName = "Bone_PB_Root";

    m_pMeshComponent = static_cast<CSkeletalMeshComponent*>(__super::Add_ActorComponent
    (
        static_cast<_uint>(ELevel::LEVEL_STAGE1),
        TEXT("SkeletalMeshComponent_Pan_Brute"),
        TEXT("SkeletalMeshComponent_Pan_Brute"),
        &tSkeletalMeshComponent
    ));
    if (m_pMeshComponent == nullptr)
        return E_FAIL;

    Set_RootComponent(m_pMeshComponent);
    m_pMovementComponent->Set_UpdateComponent(m_pRootComponent);

    // ColliderComponent
    FColliderComponent tColliderComponent{};
    tColliderComponent.pOwner = this;
    tColliderComponent.bCulling = true;
    tColliderComponent.fCullingRange = 10.f;
    tColliderComponent.vScale = _float3(0.7f, 1.2f, 0.7f);
    tColliderComponent.vRotation = _float3(0.f, 0.f, 0.f);
    tColliderComponent.vPosition = _float3(0.f, 1.2f, 0.f);

    m_pColliderComponent = static_cast<CColliderComponent*>(__super::Add_ActorComponent
    (
        static_cast<_uint>(ELevel::LEVEL_STATIC),
        g_strColliderComponent_OBB,
        g_strColliderComponent_OBB,
        &tColliderComponent
    ));
    if (m_pColliderComponent == nullptr)
        return E_FAIL;

    m_pColliderComponent->AttachToComponent(m_pRootComponent);

    return S_OK;
}

HRESULT CMonster_Brute::Ready_Resource()
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

HRESULT CMonster_Brute::Ready_SubActor()
{
    // Actor_UI_MonsterUI
    FActor tActor{};
    tActor.pOuter = m_pOuter;

    //m_pMonsterUI = static_cast<CMonsterUI*>(__super::Add_SubActor
    //(
    //    static_cast<_uint>(ELevel::LEVEL_STAGE1),
    //    TEXT("Actor_UI_MonsterUI"),
    //    TEXT("MonsterUI"),
    //    &tActor
    //));
    //if (m_pMonsterUI == nullptr)
    //    return E_FAIL;

    //m_pMonsterUI->Ready_HPUI(&m_fMaxHP);

    return S_OK;
}

HRESULT CMonster_Brute::Ready_AnimStateMachine()
{
    return S_OK;
}

CMonster_Brute* CMonster_Brute::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
    CMonster_Brute* pInstance = new CMonster_Brute(_pDevice, _pContext);

    if (FAILED(pInstance->Init()))
    {
        MSG_BOX("Failed To Create : CMonster_Brute");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CClonable* CMonster_Brute::Clone(void* _pDesc)
{
    CMonster_Brute* pInstance = new CMonster_Brute(*this);

    if (FAILED(pInstance->Init(_pDesc)))
    {
        MSG_BOX("Failed To Create : CMonster_Brute");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMonster_Brute::Free()
{
    __super::Free();

    Safe_Release(m_pMonsterUI);

    Safe_Release(m_pColliderComponent);
    Safe_Release(m_pMeshComponent);
}