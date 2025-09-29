#pragma once

#include "Engine_Define.h"

#include "EChannel.h"

BEGIN(Engine)

class ENGINE_DLL ISoundManager abstract
{
protected :
	virtual ~ISoundManager() = default;

public :
	virtual void Play_Sound(const _tchar* _pSoundKey, EChannel _eID, _float _fVolume) PURE;
	virtual void Play_BGM(const _tchar* _pSoundKey, _float _fVolume) PURE;
	virtual void Play_BGMOnce(const _tchar* _pSoundKey, EChannel _eID, _float _fVolume) PURE;
	virtual void Stop_Sound(EChannel _eID) PURE;
	virtual void Stop_All() PURE;
	virtual void Set_ChannelVolume(EChannel _eID, _float _fVolume) PURE;
};

END