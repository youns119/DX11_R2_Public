#pragma once

#include "CObject.h"

#include "FKeyFrame.h"

BEGIN(Engine)

class CSkeleton;

class CChannel final
	: public CObject
{
private :
	CChannel();
	virtual ~CChannel() = default;

public :
	HRESULT Init(HANDLE& _hFile, _ulong& _dwByte, const CSkeleton* _pSkeleton);

public :
	const _uint Get_BoneIndex() const { return m_iBoneIndex; }
	const FKeyFrame Get_CurrKeyFrame() const { return m_vecKeyFrame[m_iCurrKeyFrameIndex]; }
	const FKeyFrame Get_InterpKeyFrame() const { return m_tInterpKeyFrame; }

public :
	void Set_InterpFrame(FKeyFrame _tKeyFrame);

public :
	void Update_Transform_Matrix(_float _fCurrTrackPosition, CSkeleton* _pSkeleton);
	void Reset();

private :
	vector<FKeyFrame> m_vecKeyFrame;

private :
	_char m_szName[MAX_PATH]{};
	_uint m_iBoneIndex{};
	_uint m_iNumKeyFrame{};
	_uint m_iCurrKeyFrameIndex{};

private :
	FKeyFrame m_tInterpKeyFrame{};

public :
	static CChannel* Create(HANDLE& _hFile, _ulong& _dwByte, const CSkeleton* _pSkeleton);
	CChannel* Clone() const;
	virtual void Free() override;
};

END