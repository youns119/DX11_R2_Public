#include "pch.h"
#include "CMonster_ManyFaces.h"

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

CMonster_ManyFaces::CMonster_ManyFaces(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    : CCharacter{ _pDevice, _pContext }
{
}

CMonster_ManyFaces::CMonster_ManyFaces(const CMonster_ManyFaces& _Prototype)
    : CCharacter(_Prototype)
{
}

HRESULT CMonster_ManyFaces::Init()
{
    if (FAILED(__super::Init()))
        return E_FAIL;

    return S_OK;
}

HRESULT CMonster_ManyFaces::Init(void* _pDesc)
{
    if (_pDesc == nullptr)
        return S_OK;

    FCharacter* pCharacterDesc = static_cast<FCharacter*>(_pDesc);
    pCharacterDesc->fMoveSpeed = 8.f;
    pCharacterDesc->fRotationSpeed = 10.f;
    pCharacterDesc->fMoveAccel = 8.f;
    pCharacterDesc->fMoveDecel = 6.f;
    pCharacterDesc->fMaxVelocity = 8.f;

    if (FAILED(__super::Init(pCharacterDesc)))
        return E_FAIL;

    return S_OK;
}

void CMonster_ManyFaces::Pre_Update(_float _fTimeDelta)
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

    if (m_fDeath >= 5.f)
    {
        m_bShow = false;
        //SOUND->Stop_Sound(EChannel::ECHANNEL_BGM);

        return;
    }

    if (fDist <= 60.f && fDist > 5.f)
    {
        if (CGameInstance::Mouse_Press(EMouseKey::MOUSEKEY_LB))
        {
            if (pPlayer->Get_Shoot())
            {
                pPlayer->Damage_Boss(4.f);
                pPlayer->Set_Shoot(false);
            }
        }
    }

    if (pPlayer->Get_BossCurrHP() <= 0.f)
    {
        if (!m_bDeath)
        {
            m_pMeshComponent->Set_CurrAnim(m_pMeshComponent->Get_AnimIndex("Impact_Death"), false);
            SOUND->Play_Sound(L"Boss_Death.wav", EChannel::ECHANNEL_ENEMY, 0.5f);

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
            if (m_fAttack >= m_fAttackTime && !m_bAttackSound)
            {
                SOUND->Play_Sound(L"Boss_MultiSlam.wav", EChannel::ECHANNEL_ENEMY, 0.5f);
                m_bAttackSound = true;
            }
        }
        else if (m_iAttackIndex == 1)
        {
            if (m_fAttack >= m_fAttackTime && !m_bAttackSound)
            {
                SOUND->Play_Sound(L"Boss_GroundSmash.wav", EChannel::ECHANNEL_ENEMY, 0.5f);
                m_bAttackSound = true;
            }
        }
        else
        {
            if (m_fAttack >= m_fAttackTime && !m_bAttackSound)
            {
                SOUND->Play_Sound(L"Boss_Combo.wav", EChannel::ECHANNEL_ENEMY, 0.5f);
                m_bAttackSound = true;
            }
        }

        if (m_fAttack >= m_fAttackTime && fDist <= 5.f)
        {
            pPlayer->Damaged(30.f);
            m_fAttackTime = 100.f;

            CSkeletalMeshComponent* pPlayerMesh = pPlayer->Get_MeshComponent();
            pPlayerMesh->Set_CurrAnim(pPlayerMesh->Get_AnimIndex("2R_Impact_Stagger_F"));
        }

        __super::Pre_Update(_fTimeDelta);

        return;
    }

    if (fDist <= 60.f && fDist > 5.f && !m_bAttack)
    {
        if (m_pTarget == nullptr)
        {
            m_pTarget = pPlayer;
        }

        m_pMovementComponent->Add_Velocity(vDist, _fTimeDelta);
        m_pMeshComponent->Set_CurrAnim(m_pMeshComponent->Get_AnimIndex("Walk_F"), true);

        m_fSoundTimeAcc += _fTimeDelta;

        if (m_fSoundTimeAcc >= m_fWalkSoundDist)
        {
            SOUND->Play_Sound(L"Boss_Walk.wav", EChannel::ECHANNEL_ENEMY, 0.05f);
            m_fSoundTimeAcc = 0.f;
        }
    }
    else if (fDist <= 5.f)
    {
        m_iAttackIndex = rand() % 3;

        if (m_iAttackIndex == 0)
        {
            m_pMeshComponent->Set_CurrAnim(m_pMeshComponent->Get_AnimIndex("Atk_MultiSlam"), false);
            m_bAttack = true;
            m_bAttackSound = false;
            m_fAttackAcc = 5.f;
            m_fAttackTime = 2.f;
        }
        else if (m_iAttackIndex == 1)
        {
            m_pMeshComponent->Set_CurrAnim(m_pMeshComponent->Get_AnimIndex("Atk_GroundSmash"), false);
            m_bAttack = true;
            m_bAttackSound = false;
            m_fAttackAcc = 4.5f;
            m_fAttackTime = 2.2f;
        }
        else
        {
            m_pMeshComponent->Set_CurrAnim(m_pMeshComponent->Get_AnimIndex("Atk_ComboLR"), false);
            m_bAttack = true;
            m_bAttackSound = false;
            m_fAttackAcc = 4.5f;
            m_fAttackTime = 1.4f;
        }
    }
    else
        m_pMeshComponent->Set_CurrAnim(m_pMeshComponent->Get_AnimIndex("Idle"), true);

    m_pMovementComponent->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDegree * _fTimeDelta);

    __super::Pre_Update(_fTimeDelta);
}

void CMonster_ManyFaces::Update(_float _fTimeDelta)
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

    __super::Update(_fTimeDelta);
}

void CMonster_ManyFaces::Post_Update(_float _fTimeDelta)
{
    __super::Post_Update(_fTimeDelta);
}

HRESULT CMonster_ManyFaces::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CMonster_ManyFaces::Ready_Component()
{
    // SkeletalMeshComponent
    FSkeletalMeshComponent tSkeletalMeshComponent{};
    tSkeletalMeshComponent.pOwner = this;
    tSkeletalMeshComponent.eRenderType = ERender::RENDER_NONBLEND;
    tSkeletalMeshComponent.bCulling = true;
    tSkeletalMeshComponent.fCullingRange = 10.f;
    tSkeletalMeshComponent.szAnimName = "Idle";
    tSkeletalMeshComponent.szRootName = "Bone_MF_Root";
    tSkeletalMeshComponent.vPosition = _float3(20.f, 2.f, 40.f);

    m_pMeshComponent = static_cast<CSkeletalMeshComponent*>(__super::Add_ActorComponent
    (
        static_cast<_uint>(ELevel::LEVEL_STAGE1),
        TEXT("SkeletalMeshComponent_ManyFaces"),
        TEXT("SkeletalMeshComponent_ManyFaces"),
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
    tColliderComponent.vScale = _float3(1.5f, 2.f, 1.5f);
    tColliderComponent.vRotation = _float3(0.f, 0.f, 0.f);
    tColliderComponent.vPosition = _float3(0.f, 2.f, 0.f);

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

HRESULT CMonster_ManyFaces::Ready_Resource()
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

HRESULT CMonster_ManyFaces::Ready_SubActor()
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

    return S_OK;
}

HRESULT CMonster_ManyFaces::Ready_AnimStateMachine()
{
    return S_OK;
}

CMonster_ManyFaces* CMonster_ManyFaces::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
    CMonster_ManyFaces* pInstance = new CMonster_ManyFaces(_pDevice, _pContext);

    if (FAILED(pInstance->Init()))
    {
        MSG_BOX("Failed To Create : CMonster_ManyFaces");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CClonable* CMonster_ManyFaces::Clone(void* _pDesc)
{
    CMonster_ManyFaces* pInstance = new CMonster_ManyFaces(*this);

    if (FAILED(pInstance->Init(_pDesc)))
    {
        MSG_BOX("Failed To Create : CMonster_ManyFaces");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMonster_ManyFaces::Free()
{
    __super::Free();

    Safe_Release(m_pMonsterUI);

    Safe_Release(m_pColliderComponent);
    Safe_Release(m_pMeshComponent);
}