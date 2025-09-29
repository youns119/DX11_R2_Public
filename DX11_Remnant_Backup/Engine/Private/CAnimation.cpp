#include "CAnimation.h"

#include "CSkeleton.h"
#include "CChannel.h"

CAnimation::CAnimation()
{
}

CAnimation::CAnimation(const CAnimation& _Prototype)
	: m_iNumChannel{ _Prototype.m_iNumChannel }
	, m_fDuration{ _Prototype.m_fDuration }
	, m_fTickPerSecond{ _Prototype.m_fTickPerSecond }
	, m_fInterpTime{ _Prototype.m_fInterpTime }
	, m_fBlendTime{ _Prototype.m_fBlendTime }
{
	strcpy_s(m_szName, _Prototype.m_szName);

	for (auto& Channel : _Prototype.m_vecChannel)
		m_vecChannel.push_back(Channel->Clone());
}

HRESULT CAnimation::Init
(
	HANDLE& _hFile,
	_ulong& _dwByte,
	const CSkeleton* _pSkeleton,
	_wstring _strAnimName
)
{
	if (!ReadFile(_hFile, &m_szName, sizeof(_char) * MAX_PATH, &_dwByte, nullptr)	||
		!ReadFile(_hFile, &m_iNumChannel, sizeof(_uint), &_dwByte, nullptr)			||
		!ReadFile(_hFile, &m_fDuration, sizeof(_float), &_dwByte, nullptr)			||
		!ReadFile(_hFile, &m_fTickPerSecond, sizeof(_float), &_dwByte, nullptr))
		return E_FAIL;

	const _tchar* szAnim = _strAnimName.c_str();
	_tchar szAnimName[MAX_PATH]{};

	_wsplitpath_s(szAnim, nullptr, 0, nullptr, 0, szAnimName, MAX_PATH, nullptr, 0);

	int iSize = WideCharToMultiByte(CP_UTF8, 0, szAnimName, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, szAnimName, -1, m_szName, iSize, NULL, NULL);

	for (size_t i = 0; i < m_iNumChannel; i++)
	{
		CChannel* pChannel = CChannel::Create(_hFile, _dwByte, _pSkeleton);
		if (pChannel == nullptr)
			return E_FAIL;

		m_vecChannel.push_back(pChannel);
	}

	return S_OK;
}

_bool CAnimation::Update_Transform_Matrix(_float _fTimeDelta, CSkeleton* _pSkeleton, _bool _bAnimLoop)
{
	m_fCurrTrackPosition += m_fTickPerSecond * _fTimeDelta;

	if (m_fCurrTrackPosition >= m_fDuration)
	{
		if (!_bAnimLoop) 
			return true;
			
		m_fCurrTrackPosition = 0.f;
	}

	_uint iChannelIndex{};

	for (auto& Channel : m_vecChannel)
		Channel->Update_Transform_Matrix(m_fCurrTrackPosition, _pSkeleton);

	return false;
}

_bool CAnimation::Interp_Transform_Matrix
(
	_float _fTimeDelta,
	CAnimation* _pPrevAnimation, 
	CSkeleton* _pSkeleton
)
{
	_bool bFinished{};

	m_fCurrInterpTime += _fTimeDelta;

	_pPrevAnimation->Update_Transform_Matrix(_fTimeDelta, _pSkeleton, true);
	Update_Transform_Matrix(_fTimeDelta, _pSkeleton, true);

	vector<CChannel*> vecPrevChannel = _pPrevAnimation->m_vecChannel;

	for (size_t i = 0; i < vecPrevChannel.size(); i++)
	{
		_vector vScale{}, vRotation{}, vTranslation{};

		FKeyFrame tPrevKeyFrame{};
		FKeyFrame tCurrKeyFrame = m_vecChannel[i]->Get_CurrKeyFrame();

		if (_pPrevAnimation->m_fCurrInterpTime == 0.f)
			tPrevKeyFrame = vecPrevChannel[i]->Get_CurrKeyFrame();
		else
			tPrevKeyFrame = vecPrevChannel[i]->Get_InterpKeyFrame();

		if (m_fCurrInterpTime >= m_fInterpTime)
		{
			vScale = XMLoadFloat3(&tCurrKeyFrame.vScale);
			vRotation = XMLoadFloat4(&tCurrKeyFrame.vRotation);
			vTranslation = XMLoadFloat3(&tCurrKeyFrame.vTranslation);

			bFinished = true;
		}
		else
		{
			_float fRatio = m_fCurrInterpTime / m_fInterpTime;

			vScale = XMVectorLerp
			(
				XMLoadFloat3(&tPrevKeyFrame.vScale),
				XMLoadFloat3(&tCurrKeyFrame.vScale),
				fRatio
			);
			vRotation = XMQuaternionSlerp
			(
				XMLoadFloat4(&tPrevKeyFrame.vRotation),
				XMLoadFloat4(&tCurrKeyFrame.vRotation),
				fRatio
			);
			vTranslation = XMVectorSetW
			(
				XMVectorLerp
				(
					XMLoadFloat3(&tPrevKeyFrame.vTranslation),
					XMLoadFloat3(&tCurrKeyFrame.vTranslation),
					fRatio
				), 1.f
			);

			bFinished = false;

			FKeyFrame tKeyFrame{};
			XMStoreFloat3(&tKeyFrame.vScale, vScale);
			XMStoreFloat4(&tKeyFrame.vRotation, vRotation);
			XMStoreFloat3(&tKeyFrame.vTranslation, vTranslation);

			m_vecChannel[i]->Set_InterpFrame(tKeyFrame);
		}

		_matrix matTransform = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);
		_pSkeleton->Set_TransformMatrix(vecPrevChannel[i]->Get_BoneIndex(), matTransform);
	}

	if (bFinished)
	{
		_pPrevAnimation->Reset();
		m_fCurrInterpTime = 0.f;
	}

	return bFinished;
}

void CAnimation::Reset()
{
	for (auto& Channel : m_vecChannel)
		Channel->Reset();

	m_fCurrTrackPosition = 0.f;
	m_fCurrInterpTime = 0.f;
}

CAnimation* CAnimation::Create
(
	HANDLE& _hFile,
	_ulong& _dwByte,
	const CSkeleton* _pSkeleton,
	_wstring _strAnimName
)
{
	CAnimation* pInstance = new CAnimation;

	if (FAILED(pInstance->Init(_hFile, _dwByte, _pSkeleton, _strAnimName)))
	{
		MSG_BOX("Failed To Create : CAnimation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimation* CAnimation::Clone() const
{
	return new CAnimation(*this);
}

void CAnimation::Free()
{
	__super::Free();

	for (auto& Channel : m_vecChannel)
		Safe_Release(Channel);

	m_vecChannel.clear();
}