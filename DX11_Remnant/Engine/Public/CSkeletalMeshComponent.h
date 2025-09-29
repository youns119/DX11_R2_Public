#pragma once

#include "CMeshComponent.h"

#include "FSkeletalMeshComponent.h"

BEGIN(Engine)

class CSkeleton;
class CAnimation;

class ENGINE_DLL CSkeletalMeshComponent final
	: public CMeshComponent
{
private :
	CSkeletalMeshComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CSkeletalMeshComponent(const CSkeletalMeshComponent& _Prototype);
	virtual ~CSkeletalMeshComponent() = default;

public :
	virtual HRESULT Init(const _tchar* _szModelName, _fmatrix _matPreTransform);
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public :
	const _uint Get_BoneIndex(const _char* _pBoneName) const;
	const _float4x4* Get_BoneCombined_MatrixPtr(const _char* _pBoneName) const;

	const _bool Get_AnimFinish() const { return m_bAnimFinished; }
	const _uint Get_NumAnimation() const { return m_iNumAnimation; }
	const _uint Get_CurrAnimIndex() const
	{ return m_iCurrAnimIndex; }
	const _uint Get_AnimIndex(const _char* _pAnimName) const;

public :
	void Set_CurrAnim(_uint _iAnimIndex, _bool _bAnimLoop = false);

public :
	_bool Play_Animation(_float _fTimeDelta);
	void Update_RootMotion();

private :
	CSkeleton* m_pSkeleton{};
	vector<CAnimation*> m_vecAnimation;

private :
	_bool m_bAnimFinished{};
	_bool m_bAnimLoop{};
	_bool m_bAnimInterp{};
	_uint m_iNumAnimation{};
	_uint m_iPrevAnimIndex{};
	_uint m_iCurrAnimIndex{};

private :
	HRESULT Bind_BoneMatrices(CMesh* _pSkeletalMesh);

private :
	HRESULT Ready_Skeleton(HANDLE& _hFile, _ulong& _dwByte, _int _iParentBoneIndex = -1);
	virtual HRESULT Ready_Mesh(HANDLE& _hFile, _ulong& _dwByte);
	HRESULT Ready_Animation
	(
		HANDLE& _hFile, 
		_ulong& _dwByte, 
		_uint _iAnimIndex, 
		_wstring _strAnimName
	);

public :
	static CSkeletalMeshComponent* Create
	(
		ID3D11Device* _pDevice,
		ID3D11DeviceContext* _pContext,
		const _tchar* _szModelName,
		_fmatrix _matPreTransform = XMMatrixIdentity()
	);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END