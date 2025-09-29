#include "CSkeletalMeshComponent.h"

#include "CActor.h"
#include "CSkeleton.h"
#include "CSkeletalMesh.h"
#include "CAnimation.h"
#include "CFileUtility.h"

CSkeletalMeshComponent::CSkeletalMeshComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CMeshComponent{ _pDevice, _pContext }
{
	m_strName = g_strSkeletalMeshComponent;
}

CSkeletalMeshComponent::CSkeletalMeshComponent(const CSkeletalMeshComponent& _Prototype)
	: CMeshComponent(_Prototype)
	, m_iNumAnimation{ _Prototype.m_iNumAnimation }
{
	m_pSkeleton = _Prototype.m_pSkeleton->Clone();

	for (auto& Animation : _Prototype.m_vecAnimation)
		m_vecAnimation.push_back(Animation->Clone());
}

HRESULT CSkeletalMeshComponent::Init(const _tchar* _szModelName, _fmatrix _matPreTransform)
{
	XMStoreFloat4x4(&m_matPreTransform, _matPreTransform);

	_wstring strModelName = _wstring(_szModelName);

	vector<_wstring> vecPath;
	vector<_wstring> vecSrcPath;
	vecSrcPath.push_back(MESH + strModelName);
	vecSrcPath.push_back(MATERIAL + strModelName);
	vecSrcPath.push_back(ANIM + strModelName);

	// Mesh
	CFileUtility::TraverseDirectory(vecSrcPath[0], vecPath);

	_ulong dwByte{};
	HANDLE hFile = CreateFile(vecPath[0].c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == 0)
		return E_FAIL;

	if (FAILED(Ready_Skeleton(hFile, dwByte)) ||
		FAILED(Ready_Mesh(hFile, dwByte)))
		return E_FAIL;

	CloseHandle(hFile);
	vecPath.clear();

	// Material
	CFileUtility::TraverseDirectory(vecSrcPath[1], vecPath);

	dwByte = 0;
	hFile = CreateFile(vecPath[0].c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == 0)
		return E_FAIL;

	if (FAILED(Ready_Material(hFile, dwByte, _szModelName)))
		return E_FAIL;

	CloseHandle(hFile);
	vecPath.clear();

	// Animation
	CFileUtility::TraverseDirectory(vecSrcPath[2], vecPath);
	m_iNumAnimation = static_cast<_uint>(vecPath.size());

	for (size_t i = 0; i < static_cast<size_t>(m_iNumAnimation); i++)
	{
		dwByte = 0;
		hFile = CreateFile(vecPath[i].c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile == 0)
			return E_FAIL;

		if (FAILED(Ready_Animation(hFile, dwByte, static_cast<_uint>(i), vecPath[i])))
			return E_FAIL;

		CloseHandle(hFile);
	}

	vecPath.clear();

	if (FAILED(__super::Init(_szModelName, _matPreTransform)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkeletalMeshComponent::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	FSkeletalMeshComponent* pSkeletalMeshDesc = static_cast<FSkeletalMeshComponent*>(_pDesc);
	m_iCurrAnimIndex = Get_AnimIndex(pSkeletalMeshDesc->szAnimName);
	m_pSkeleton->Set_RootBone(pSkeletalMeshDesc->szRootName);
	m_bAnimLoop = true;

	if (FAILED(__super::Init(_pDesc)))
		return E_FAIL;

	return S_OK;
}

void CSkeletalMeshComponent::Update(_float _fTimeDelta)
{
	Play_Animation(_fTimeDelta);
	Update_RootMotion();

	__super::Update(_fTimeDelta);
}

HRESULT CSkeletalMeshComponent::Render()
{
	if (FAILED(Bind_ShaderResource()))
		return E_FAIL;

	for (auto& Mesh : m_vecMesh)
	{
		if (FAILED(Bind_MeshMaterial(Mesh))		||
			FAILED(Bind_BoneMatrices(Mesh))		||
			FAILED(Begin_Shader())				||
			FAILED(Mesh->Render()))
			return E_FAIL;
	}

	return S_OK;
}

const _uint CSkeletalMeshComponent::Get_BoneIndex(const _char* _pBoneName) const
{
	return m_pSkeleton->Get_BoneIndex(_pBoneName);
}

const _float4x4* CSkeletalMeshComponent::Get_BoneCombined_MatrixPtr(const _char* _pBoneName) const
{
	return m_pSkeleton->Get_BoneCombinedMatrix_Ptr(_pBoneName);
}

const _uint CSkeletalMeshComponent::Get_AnimIndex(const _char* _pAnimName) const
{
	_uint iAnimIndex{};

	auto iter = find_if(m_vecAnimation.begin(), m_vecAnimation.end(), [&](CAnimation* _pAnim)->bool
		{
			if (strcmp(_pAnim->Get_Name(), _pAnimName) == false)
				return true;

			iAnimIndex++;
			return false;
		});

	return iAnimIndex;
}

void CSkeletalMeshComponent::Set_CurrAnim(_uint _iAnimIndex, _bool _bAnimLoop)
{
	if (m_iCurrAnimIndex != _iAnimIndex)
	{
		if (!m_bAnimLoop)
		{
			CAnimation* pCurrAnim = m_vecAnimation[m_iCurrAnimIndex];

			if (pCurrAnim->Get_RemainTime() > pCurrAnim->Get_BlendTime())
				return;
		}

		if (m_bAnimInterp)
			m_vecAnimation[m_iPrevAnimIndex]->Reset();

		m_iPrevAnimIndex = m_iCurrAnimIndex;
		m_iCurrAnimIndex = _iAnimIndex;

		m_bAnimLoop = _bAnimLoop;
		m_bAnimInterp = true;
	}
}

_bool CSkeletalMeshComponent::Play_Animation(_float _fTimeDelta)
{
	m_bAnimFinished = false;

	if (m_bAnimInterp)
		m_bAnimFinished = m_vecAnimation[m_iCurrAnimIndex]->
			Interp_Transform_Matrix(_fTimeDelta, m_vecAnimation[m_iPrevAnimIndex], m_pSkeleton);
	else
		m_bAnimFinished = m_vecAnimation[m_iCurrAnimIndex]->
			Update_Transform_Matrix(_fTimeDelta, m_pSkeleton, m_bAnimLoop);

	if (m_bAnimFinished)
	{
		m_bAnimInterp = false;
		m_pSkeleton->Reset_RootTransform();
	}

	m_pSkeleton->Update_CombinedTransformMatrix(XMLoadFloat4x4(&m_matPreTransform));

	return m_bAnimFinished;
}

void CSkeletalMeshComponent::Update_RootMotion()
{
	//const _float4x4* pRootTranform = m_pSkeleton->Get_DeltaMatrix_Ptr();

	//_vector vScale{}, vRotation{}, vTranslation{};

	//XMMatrixDecompose(&vScale, &vRotation, &vTranslation, XMLoadFloat4x4(pRootTranform));

	////Set_Rotation(vRotation);
	//if (XMVectorGetX(XMVector3Length(vTranslation)) != 0.f)
	//{
	//	_float3 vPos = Get_Transform(ETransform::TRANSFORM_POSITION);

	//	Set_DeltaPosition(vTranslation);

	//	//_vector vFinalPos = XMVectorSet
	//	//(
	//	//	vPos.x + XMVectorGetX(vTranslation),
	//	//	vPos.y + XMVectorGetY(vTranslation),
	//	//	vPos.z + XMVectorGetZ(vTranslation),
	//	//	1.f
	//	//);

	//	//Set_Position(vFinalPos);
	//	//Set_Position(Get_TransformState(ETransformState::TRANSFORMSTATE_POS) + vTranslation);
	//}
}

HRESULT CSkeletalMeshComponent::Bind_BoneMatrices(CMesh* _pSkeletalMesh)
{
	if (FAILED(m_pSkeleton->Bind_BoneMatrices
	(
		m_pShader,
		static_cast<CSkeletalMesh*>(_pSkeletalMesh), 
		"g_BoneMatrices"
	)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkeletalMeshComponent::Ready_Skeleton(HANDLE& _hFile, _ulong& _dwByte, _int _iParentBoneIndex)
{
	m_pSkeleton = CSkeleton::Create(_hFile, _dwByte, _iParentBoneIndex);
	if (m_pSkeleton == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CSkeletalMeshComponent::Ready_Mesh(HANDLE& _hFile, _ulong& _dwByte)
{
	if (!ReadFile(_hFile, &m_iNumMesh, sizeof(_uint), &_dwByte, nullptr))
		return E_FAIL;

	for (size_t i = 0; i < m_iNumMesh; i++)
	{
		CSkeletalMesh* pMesh = CSkeletalMesh::Create
		(
			m_pDevice,
			m_pContext,
			m_pSkeleton,
			_hFile,
			_dwByte
		);
		if (pMesh == nullptr)
			return E_FAIL;

		m_vecMesh.push_back(pMesh);
	}

	return S_OK;
}

HRESULT CSkeletalMeshComponent::Ready_Animation
(
	HANDLE& _hFile, 
	_ulong& _dwByte, 
	_uint _iAnimIndex, 
	_wstring _strAnimName
)
{
	CAnimation* pAnimation = CAnimation::Create
	(
		_hFile, 
		_dwByte,
		m_pSkeleton,
		_strAnimName
	);
	if (pAnimation == nullptr)
		return E_FAIL;

	m_vecAnimation.push_back(pAnimation);

	return S_OK;
}

CSkeletalMeshComponent* CSkeletalMeshComponent::Create
(
	ID3D11Device* _pDevice,
	ID3D11DeviceContext* _pContext,
	const _tchar* _szModelName,
	_fmatrix _matPreTransform
)
{
	CSkeletalMeshComponent* pInstance = new CSkeletalMeshComponent(_pDevice, _pContext);

	if (FAILED(pInstance->Init(_szModelName, _matPreTransform)))
	{
		MSG_BOX("Failed To Create : CSkeletalMeshComponent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CSkeletalMeshComponent::Clone(void* _pDesc)
{
	CSkeletalMeshComponent* pInstance = new CSkeletalMeshComponent(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Create : CSkeletalMeshComponent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkeletalMeshComponent::Free()
{
	__super::Free();

	for (auto& Animation : m_vecAnimation)
		Safe_Release(Animation);

	m_vecAnimation.clear();

	Safe_Release(m_pSkeleton);
}