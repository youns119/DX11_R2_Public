#pragma once

#include "CMesh.h"

#include "FVertex_SkeletalMesh.h"

BEGIN(Engine)

class CSkeleton;

class CSkeletalMesh final
	: public CMesh
{
private :
	CSkeletalMesh(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CSkeletalMesh(const CSkeletalMesh& _Prototype);
	virtual ~CSkeletalMesh() = default;

public:
	virtual HRESULT Init(const CSkeleton* _pSkeleton, HANDLE& _hFile, _ulong& _dwByte);
	virtual HRESULT Init(void* _pDesc) override;

public :
	_uint Get_BoneIndice(_uint _iIndex) { return m_vecBoneIndice[_iIndex]; }
	_float4x4 Get_Offset(_uint _iIndex) { return m_vecOffset[_iIndex]; }
	_uint Get_NumBone() const { return m_iNumBone; }

private :
	vector<_uint> m_vecBoneIndice;
	vector<_float4x4> m_vecOffset;

private :
	_uint m_iNumBone{};

public:
	static CSkeletalMesh* Create
	(
		ID3D11Device* _pDevice,
		ID3D11DeviceContext* _pContext,
		const CSkeleton* _pSkeleton,
		HANDLE& _hFile,
		_ulong& _dwByte
	);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END