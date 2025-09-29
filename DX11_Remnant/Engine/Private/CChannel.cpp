#include "CChannel.h"

#include "CSkeleton.h"

CChannel::CChannel()
{
}

HRESULT CChannel::Init(HANDLE& _hFile, _ulong& _dwByte, const CSkeleton* _pSkeleton)
{
	if (!ReadFile(_hFile, &m_szName, sizeof(_char) * MAX_PATH, &_dwByte, nullptr))
		return E_FAIL;

	m_iBoneIndex = _pSkeleton->Get_BoneIndex(m_szName);

	if (!ReadFile(_hFile, &m_iNumKeyFrame, sizeof(_uint), &_dwByte, nullptr))
		return E_FAIL;

	for (size_t i = 0; i < m_iNumKeyFrame; i++)
	{
		FKeyFrame tKeyFrame{};
		if (!ReadFile(_hFile, &tKeyFrame, sizeof(FKeyFrame), &_dwByte, nullptr))
			return E_FAIL;

		m_vecKeyFrame.push_back(tKeyFrame);
	}

	return S_OK;
}

void CChannel::Set_InterpFrame(FKeyFrame _tKeyFrame)
{
	m_tInterpKeyFrame.vScale = _tKeyFrame.vScale;
	m_tInterpKeyFrame.vRotation = _tKeyFrame.vRotation;
	m_tInterpKeyFrame.vTranslation = _tKeyFrame.vTranslation;
}

void CChannel::Update_Transform_Matrix(_float _fCurrTrackPosition, CSkeleton* _pSkeleton)
{
	_vector vScale{}, vRotation{}, vTranslation{};
	FKeyFrame tLastKeyFrame = m_vecKeyFrame.back();

	if (_fCurrTrackPosition == 0.f)
		m_iCurrKeyFrameIndex = 0;

	if (_fCurrTrackPosition >= tLastKeyFrame.fTrackPosition)
	{
		vScale = XMLoadFloat3(&tLastKeyFrame.vScale);
		vRotation = XMLoadFloat4(&tLastKeyFrame.vRotation);
		vTranslation = XMVectorSetW(XMLoadFloat3(&tLastKeyFrame.vTranslation), 1.f);
	}
	else
	{
		while (_fCurrTrackPosition >= m_vecKeyFrame[m_iCurrKeyFrameIndex + 1].fTrackPosition)
			++m_iCurrKeyFrameIndex;

		FKeyFrame tCurrKeyFrame = m_vecKeyFrame[m_iCurrKeyFrameIndex];
		FKeyFrame tNextKeyFrame = m_vecKeyFrame[m_iCurrKeyFrameIndex + 1];

		_float fRatio = (_fCurrTrackPosition - tCurrKeyFrame.fTrackPosition) / 
			(tNextKeyFrame.fTrackPosition - tCurrKeyFrame.fTrackPosition);

		vScale = XMVectorLerp
		(
			XMLoadFloat3(&tCurrKeyFrame.vScale), 
			XMLoadFloat3(&tNextKeyFrame.vScale), 
			fRatio
		);
		vRotation = XMQuaternionSlerp
		(
			XMLoadFloat4(&tCurrKeyFrame.vRotation), 
			XMLoadFloat4(&tNextKeyFrame.vRotation), 
			fRatio
		);
		vTranslation = XMVectorSetW
		(
			XMVectorLerp
			(
				XMLoadFloat3(&tCurrKeyFrame.vTranslation), 
				XMLoadFloat3(&tNextKeyFrame.vTranslation), 
				fRatio
			), 1.f
		);
	}

	_matrix matTransform = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);
	_pSkeleton->Set_TransformMatrix(m_iBoneIndex, matTransform);
}

void CChannel::Reset()
{
	m_iCurrKeyFrameIndex = 0;
	ZeroMemory(&m_tInterpKeyFrame, sizeof(FKeyFrame));
}

CChannel* CChannel::Create(HANDLE& _hFile, _ulong& _dwByte, const CSkeleton* _pSkeleton)
{
	CChannel* pInstance = new CChannel;

	if (FAILED(pInstance->Init(_hFile, _dwByte, _pSkeleton)))
	{
		MSG_BOX("Failed To Create : CChannel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CChannel* CChannel::Clone() const
{
	return new CChannel(*this);
}

void CChannel::Free()
{
	__super::Free();

	m_vecKeyFrame.clear();
}