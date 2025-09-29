#pragma once

#include "CObject.h"

BEGIN(Engine)

class CShader;
class CSkeletalMesh;
class CBone;

class CSkeleton final
	: public CObject
{
private :
	CSkeleton();
	CSkeleton(const CSkeleton& _Prototype);
	virtual ~CSkeleton() = default;

public :
	HRESULT Init(HANDLE& _hFile, _ulong& _dwByte, _int _iParentBoneIndex);

public :
	CBone* Get_Bone(_uint _iIndex) { return m_vecBone[_iIndex]; }
	const _char* Get_RootName() { return m_szRootName; }
	_uint Get_BoneIndex(const _char* _pBoneName) const;

	const _float4x4* Get_BoneCombinedMatrix_Ptr(const _char* _pBoneName) const;
	const _float4x4* Get_DeltaMatrix_Ptr() const
	{ return &m_matDeltaTransform; }

public :
	void Set_RootBone(const _char* _szRootName) { m_szRootName = _szRootName; }
	void Set_TransformMatrix(_uint _iIndex, _fmatrix _matTransform);

public :
	void Update_CombinedTransformMatrix(_fmatrix _matPreTransform);
	void Update_RootTransform(_float4x4& _matRootTransform);

public :
	void Reset_RootTransform();

public :
	HRESULT Bind_BoneMatrices
	(
		CShader* _pShader,
		CSkeletalMesh* _pSkeletalMesh,
		const _char* _pConstantName
	);

private :
	HRESULT Ready_Bone(HANDLE& _hFile, _ulong& _dwByte, _int _iParentBoneIndex);

private :
	vector<CBone*> m_vecBone;

private :
	_float4x4 m_BoneMatrices[256]{};

	_float4x4 m_matRootTransform{};
	_float4x4 m_matDeltaTransform{};

private :
	const _char* m_szRootName{};

public :
	static CSkeleton* Create(HANDLE& _hFile, _ulong& _dwByte, _int _iParentBoneIndex);
	CSkeleton* Clone() const;
	virtual void Free() override;
};

END