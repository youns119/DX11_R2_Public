#pragma once

#include "CObject.h"

BEGIN(Engine)

class CSkeleton;

class CBone final
	: public CObject
{
private :
	CBone();
	virtual ~CBone() = default;

public :
	HRESULT Init(HANDLE& _hFile, _ulong& _dwByte, _int _iParentBoneIndex);

public :
	_matrix Get_CombinedTransformMatrix() const { return XMLoadFloat4x4(&m_matCombinedTransform); }
	const _float4x4* Get_CombinedTransformMatrix_Ptr() const { return &m_matCombinedTransform; }

	const _char* Get_Name() const { return m_szName; }
	_int Get_ParentBoneIndex() const { return m_iParentBoneIndex; }

public :
	void Set_TransformMatrix(_fmatrix _matTransform) { XMStoreFloat4x4(&m_matTransform, _matTransform); }

public :
	void Update_CombinedTransformMatrix(CSkeleton* _pSkeleton, _fmatrix _matPreTransform);

private :
	_float4x4 m_matTransform{};
	_float4x4 m_matCombinedTransform{};

private :
	_char m_szName[MAX_PATH]{};
	_int m_iParentBoneIndex{ -1 };

public :
	static CBone* Create(HANDLE& _hFile, _ulong& _dwByte, _int _iParentBoneIndex);
	CBone* Clone() const;
	virtual void Free() override;
};

END