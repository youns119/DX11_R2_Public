#include "CBone.h"

#include "CSkeleton.h"

CBone::CBone()
{
}

HRESULT CBone::Init(HANDLE& _hFile, _ulong& _dwByte, _int _iParentBoneIndex)
{
	if (!ReadFile(_hFile, &m_szName, sizeof(_char) * MAX_PATH, &_dwByte, nullptr) ||
		!ReadFile(_hFile, &m_matTransform, sizeof(_float4x4), &_dwByte, nullptr))
		return E_FAIL;

	XMStoreFloat4x4(&m_matCombinedTransform, XMMatrixIdentity());

	m_iParentBoneIndex = _iParentBoneIndex;

	return S_OK;
}

void CBone::Update_CombinedTransformMatrix(CSkeleton* _pSkeleton, _fmatrix _matPreTransform)
{
	if (m_iParentBoneIndex == -1)
		XMStoreFloat4x4(&m_matCombinedTransform, _matPreTransform * XMLoadFloat4x4(&m_matTransform));
	else
	{
		const _float4x4* matParentCombined = 
			_pSkeleton->Get_Bone(m_iParentBoneIndex)->Get_CombinedTransformMatrix_Ptr();

		XMStoreFloat4x4
		(
			&m_matCombinedTransform,
			XMLoadFloat4x4(&m_matTransform) * XMLoadFloat4x4(matParentCombined)
		);

		if (!strcmp(m_szName, _pSkeleton->Get_RootName()))
			_pSkeleton->Update_RootTransform(m_matCombinedTransform);
	}
}

CBone* CBone::Create(HANDLE& _hFile, _ulong& _dwByte, _int _iParentBoneIndex)
{
	CBone* pInstance = new CBone;

	if (FAILED(pInstance->Init(_hFile, _dwByte, _iParentBoneIndex)))
	{
		MSG_BOX("Failed To Create : CBone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBone* CBone::Clone() const
{
	return new CBone(*this);
}

void CBone::Free()
{
	__super::Free();
}