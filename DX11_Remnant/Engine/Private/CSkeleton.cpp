#include "CSkeleton.h"

#include "CShader.h"
#include "CSkeletalMesh.h"
#include "CBone.h"

CSkeleton::CSkeleton()
{
}

CSkeleton::CSkeleton(const CSkeleton& _Prototype)
{
	for (auto& Bone : _Prototype.m_vecBone)
		m_vecBone.push_back(Bone->Clone());
}

HRESULT CSkeleton::Init(HANDLE& _hFile, _ulong& _dwByte, _int _iParentBoneIndex)
{
	if (FAILED(Ready_Bone(_hFile, _dwByte, _iParentBoneIndex)))
		return E_FAIL;

	return S_OK;
}

_uint CSkeleton::Get_BoneIndex(const _char* _pBoneName) const
{
	_uint iBoneIndex{};

	auto iter = find_if(m_vecBone.begin(), m_vecBone.end(), [&](CBone* _pBone)->bool
		{
			if (strcmp(_pBone->Get_Name(), _pBoneName) == false)
				return true;

			iBoneIndex++;
			return false;
		});

	return iBoneIndex;
}

const _float4x4* CSkeleton::Get_BoneCombinedMatrix_Ptr(const _char* _pBoneName) const
{
	auto iter = find_if(m_vecBone.begin(), m_vecBone.end(), [&](CBone* _pBone)->_bool
		{
			if (strcmp(_pBone->Get_Name(), _pBoneName) == false)
				return true;

			return false;
		});

	if (iter == m_vecBone.end())
		return nullptr;

	return (*iter)->Get_CombinedTransformMatrix_Ptr();
}

void CSkeleton::Set_TransformMatrix(_uint _iIndex, _fmatrix _matTransform)
{
	m_vecBone[_iIndex]->Set_TransformMatrix(_matTransform);
}

void CSkeleton::Update_CombinedTransformMatrix(_fmatrix _matPreTransform)
{
	for (auto& Bone : m_vecBone)
		Bone->Update_CombinedTransformMatrix(this, _matPreTransform);
}

void CSkeleton::Update_RootTransform(_float4x4& _matRootTransform)
{
	_vector vInScale{}, vInRotation{}, vInTranslation{};
	_vector vPrevScale{}, vPrevRotation{}, vPrevTranslation{};

	XMMatrixDecompose(&vInScale, &vInRotation, &vInTranslation, XMLoadFloat4x4(&_matRootTransform));
	XMMatrixDecompose(&vPrevScale, &vPrevRotation, &vPrevTranslation, XMLoadFloat4x4(&m_matRootTransform));

	if (XMVectorGetX(XMVector3Length(vInRotation)) != 0.f ||
		XMVectorGetX(XMVector3Length(vInTranslation)) != 0.f)
	{
		vInRotation = vInRotation - vPrevRotation;
		vInTranslation = vInTranslation - vPrevTranslation;

		XMVectorSetW(vInTranslation, 1.f);

		_matrix matTransform =
			XMMatrixAffineTransformation(vInScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vInRotation, vInTranslation);

		XMStoreFloat4x4(&m_matDeltaTransform, matTransform);
	}
	else
		XMStoreFloat4x4(&m_matDeltaTransform, XMMatrixIdentity());
	
	m_matRootTransform = _matRootTransform;

	vInRotation = XMQuaternionRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));
	vInTranslation = _vector{ 0.f, 0.f, 0.f, 1.f };
	_matrix matTransform =
		XMMatrixAffineTransformation(vInScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vInRotation, vInTranslation);

	XMStoreFloat4x4(&_matRootTransform, matTransform);
}

void CSkeleton::Reset_RootTransform()
{
	XMStoreFloat4x4(&m_matRootTransform, XMMatrixIdentity());
	XMStoreFloat4x4(&m_matDeltaTransform, XMMatrixIdentity());
}

HRESULT CSkeleton::Bind_BoneMatrices
(
	CShader* _pShader, 
	CSkeletalMesh* _pSkeletalMesh,
	const _char* _pConstantName
)
{
	for (size_t i = 0; i < _pSkeletalMesh->Get_NumBone(); i++)
	{
		_float4x4 matOffset = _pSkeletalMesh->Get_Offset(static_cast<_uint>(i));
		_uint iBoneIndex = _pSkeletalMesh->Get_BoneIndice(static_cast<_uint>(i));

		XMStoreFloat4x4
		(
			&m_BoneMatrices[i],
			XMLoadFloat4x4(&matOffset) * m_vecBone[iBoneIndex]->Get_CombinedTransformMatrix()
		);
	}

	if (FAILED(_pShader->Bind_Matrices(_pConstantName, m_BoneMatrices, _pSkeletalMesh->Get_NumBone())))
		return E_FAIL;

	ZeroMemory(m_BoneMatrices, sizeof(_float4x4) * 256);

	return S_OK;
}

HRESULT CSkeleton::Ready_Bone(HANDLE& _hFile, _ulong& _dwByte, _int _iParentBoneIndex)
{
	CBone* pBone = CBone::Create(_hFile, _dwByte, _iParentBoneIndex);
	if (pBone == nullptr)
		return E_FAIL;

	m_vecBone.push_back(pBone);

	_int iParentBoneIndex = static_cast<_int>(m_vecBone.size()) - 1;

	size_t iNumChildren{};
	if (!ReadFile(_hFile, &iNumChildren, sizeof(int), &_dwByte, nullptr))
		return E_FAIL;

	for (size_t i = 0; i < iNumChildren; i++)
		Ready_Bone(_hFile, _dwByte, iParentBoneIndex);

	return S_OK;
}

CSkeleton* CSkeleton::Create(HANDLE& _hFile, _ulong& _dwByte, _int _iParentBoneIndex)
{
	CSkeleton* pInstance = new CSkeleton;

	if (FAILED(pInstance->Init(_hFile, _dwByte, _iParentBoneIndex)))
	{
		MSG_BOX("Failed To Create : CSkeleton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CSkeleton* CSkeleton::Clone() const
{
	return new CSkeleton(*this);
}

void CSkeleton::Free()
{
	__super::Free();

	for (auto& Bone : m_vecBone)
		Safe_Release(Bone);

	m_vecBone.clear();
}