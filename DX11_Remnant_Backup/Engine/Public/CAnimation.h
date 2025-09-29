#pragma once

#include "CObject.h"

BEGIN(Engine)

class CSkeleton;
class CChannel;

class CAnimation final
	: public CObject
{
private:
	CAnimation();
	CAnimation(const CAnimation& _Prototype);
	virtual ~CAnimation() = default;

public:
	HRESULT Init
	(
		HANDLE& _hFile,
		_ulong& _dwByte,
		const CSkeleton* _pSkeleton,
		_wstring _strAnimName
	);

public :
	const _char* Get_Name() { return m_szName; }
	const _float Get_RemainTime() const 
	{ return (m_fDuration / m_fTickPerSecond) - (m_fCurrTrackPosition / m_fTickPerSecond); }

	const _float Get_InterpTime() const { return m_fInterpTime; }
	const _float Get_BlendTime() const { return m_fBlendTime; }

public :
	_bool Update_Transform_Matrix(_float _fTimeDelta, CSkeleton* _pSkeleton, _bool _bAnimLoop);
	_bool Interp_Transform_Matrix
	(
		_float _fTimeDelta, 
		CAnimation* _pPrevAnimation, 
		CSkeleton* _pSkeleton
	);
	void Reset();

private :
	vector<CChannel*> m_vecChannel;

private :
	_char m_szName[MAX_PATH]{};

	_uint m_iNumChannel{};
	_float m_fDuration{};
	_float m_fTickPerSecond{};
	_float m_fCurrTrackPosition{};

private :
	_float m_fInterpTime{ 0.15f };
	_float m_fCurrInterpTime{};
	_float m_fBlendTime{ 1.f };

private :
	_bool m_bRootMotion{};

public :
	static CAnimation* Create
	(
		HANDLE& _hFile,
		_ulong& _dwByte,
		const CSkeleton* _pSkeleton,
		_wstring _strAnimName
	);
	CAnimation* Clone() const;
	virtual void Free() override;
};

END