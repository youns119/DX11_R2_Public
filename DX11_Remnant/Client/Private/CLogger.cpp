#include "pch.h"
#include "CLogger.h"

#include "CGameInstance.h"

#include "CPlayer.h"
#include "CSkeletalMeshComponent.h"
#include "CSpringArmComponent.h"

CLogger::CLogger(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice{ _pDevice }
	, m_pContext{ _pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CLogger::Init()
{
	return S_OK;
}

void CLogger::Pre_Update(_float _fTimeDelta)
{
	m_fTimerAcc += _fTimeDelta;
	++m_iDrawCount;
}

void CLogger::Update(_float _fTimeDelta)
{
	if (m_fTimerAcc >= 1.f)
	{
		m_iFPS = m_iDrawCount;
		wsprintf(m_szFPS, TEXT("FPS : %d"), m_iFPS);

		m_fTimerAcc = 0.f;
		m_iDrawCount = 0;
	}
}

void CLogger::Post_Update(_float _fTimeDelta)
{
}

HRESULT CLogger::Render()
{
	//if (FAILED(CGameInstance::Render_Font(TEXT("Font_Gulim_Default"), m_szFPS, _float2(0.f, 0.f))) ||
	//	FAILED(Log_Player()))
	//	return E_FAIL;

	return S_OK;
}

void CLogger::Register_Player(CPlayer* _pPlayer)
{
	m_pPlayer = _pPlayer;
	Safe_AddRef(m_pPlayer);
}

HRESULT CLogger::Log_Player()
{
	if (m_pPlayer == nullptr)
		return S_OK;

	CSkeletalMeshComponent* pMeshComponent = m_pPlayer->Get_MeshComponent();
	CSpringArmComponent* pSpringArmComponent = static_cast<CSpringArmComponent*>(m_pPlayer->Find_ActorComponent(g_strSpringArmComponent));

	_float3 vPosition = pMeshComponent->Get_Transform(ETransform::TRANSFORM_POSITION);
	_float3 vRotation = pMeshComponent->Get_Transform(ETransform::TRANSFORM_ROTATION);
	_float fMaxHP = m_pPlayer->Get_MaxHP();
	_float fCurrHP = m_pPlayer->Get_CurrHP();

	_vector vLook = pMeshComponent->Get_TransformState(ETransformState::TRANSFORMSTATE_LOOK);
	_vector vSpringArmLook = pSpringArmComponent->Get_TransformState(ETransformState::TRANSFORMSTATE_LOOK);

	_float fDot = XMVectorGetX(XMVector3Dot(vLook, vSpringArmLook));

	_tchar szPosition[MAX_PATH]{};
	_tchar szRotation[MAX_PATH]{};
	_tchar szMaxHP[MAX_PATH]{};
	_tchar szCurrHP[MAX_PATH]{};
	_tchar szDot[MAX_PATH]{};

	swprintf(szPosition, sizeof(szPosition) / sizeof(_tchar), TEXT("Player Pos ( %.1f, %.1f, %.1f )"), vPosition.x, vPosition.y, vPosition.z);
	swprintf(szRotation, sizeof(szRotation) / sizeof(_tchar), TEXT("Player Rot ( %.1f, %.1f, %.1f )"), vRotation.x, vRotation.y, vRotation.z);
	swprintf(szMaxHP, sizeof(szRotation) / sizeof(_tchar), TEXT("Player MaxHP : %.1f"), fMaxHP);
	swprintf(szCurrHP, sizeof(szRotation) / sizeof(_tchar), TEXT("Player CurrHP : %.1f"), fCurrHP);
	swprintf(szDot, sizeof(szDot) / sizeof(_tchar), TEXT("Dot : %.1f"), fDot);

	CGameInstance::Render_Font(TEXT("Font_Gulim_Default"), szPosition, _float2(0.f, 20.f));
	CGameInstance::Render_Font(TEXT("Font_Gulim_Default"), szRotation, _float2(0.f, 35.f));
	CGameInstance::Render_Font(TEXT("Font_Gulim_Default"), szMaxHP, _float2(0.f, 50.f));
	CGameInstance::Render_Font(TEXT("Font_Gulim_Default"), szCurrHP, _float2(0.f, 65.f));
	CGameInstance::Render_Font(TEXT("Font_Gulim_Default"), szDot, _float2(0.f, 80.f));

	return S_OK;
}

CLogger* CLogger::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CLogger* pInstance = new CLogger(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Create : CLogger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLogger::Free()
{
	__super::Free();

	Safe_Release(m_pPlayer);

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}