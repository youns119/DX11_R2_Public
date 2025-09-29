#pragma once

#include "ISoundManager.h"

#include "CObject.h"

BEGIN(Engine)

class ENGINE_DLL CSoundManager
	: public ISoundManager
	, public CObject
{
private :
	CSoundManager();
	virtual ~CSoundManager() = default;

public :
	HRESULT Init();

public :
	virtual void Play_Sound(const _tchar* _pSoundKey, EChannel _eID, _float _fVolume) override;
	virtual void Play_BGM(const _tchar* _pSoundKey, _float _fVolume) override;
	virtual void Play_BGMOnce(const _tchar* _pSoundKey, EChannel _eID, _float _fVolume) override;
	virtual void Stop_Sound(EChannel _eID) override;
	virtual void Stop_All() override;
	virtual void Set_ChannelVolume(EChannel _eID, _float _fVolume) override;

private :
	HRESULT Ready_SoundFile();

private :
	map<const _tchar*, FMOD::Sound*> m_mapSound;
	FMOD::Channel* m_pChannelArr[static_cast<_uint>(EChannel::ECHANNEL_END)]{};
	FMOD::System* m_pSystem{};

public :
	static CSoundManager* Create();
	virtual void Free() override;
};

END