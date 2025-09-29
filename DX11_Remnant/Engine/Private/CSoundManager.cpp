#include "CSoundManager.h"

CSoundManager::CSoundManager()
{
}

HRESULT CSoundManager::Init()
{
    FMOD::System_Create(&m_pSystem);
    m_pSystem->init(64, FMOD_INIT_NORMAL, nullptr);

    if (FAILED(Ready_SoundFile()))
        return E_FAIL;

    return S_OK;
}

void CSoundManager::Play_Sound(const _tchar* _pSoundKey, EChannel _eID, _float _fVolume)
{
	map<const _tchar*, FMOD::Sound*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& iter)->_bool
		{
			return !lstrcmp(_pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE;

	m_pSystem->playSound(iter->second, nullptr, false, &m_pChannelArr[static_cast<_uint>(_eID)]);
	m_pChannelArr[static_cast<_uint>(_eID)]->setVolume(_fVolume);

	m_pSystem->update();
}

void CSoundManager::Play_BGM(const _tchar* _pSoundKey, _float _fVolume)
{
	map<const _tchar*, FMOD::Sound*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)->_bool
		{
			return !lstrcmp(_pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	m_pSystem->playSound(iter->second, nullptr, false, &m_pChannelArr[static_cast<_uint>(EChannel::ECHANNEL_BGM)]);
	m_pChannelArr[static_cast<_uint>(EChannel::ECHANNEL_BGM)]->setMode(FMOD_LOOP_NORMAL);
	m_pChannelArr[static_cast<_uint>(EChannel::ECHANNEL_BGM)]->setVolume(_fVolume);

	m_pSystem->update();
}

void CSoundManager::Play_BGMOnce(const _tchar* _pSoundKey, EChannel _eID, _float _fVolume)
{
	map<const _tchar*, FMOD::Sound*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)->_bool
		{
			return !lstrcmp(_pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	_bool bPlaying = false;
	if (m_pChannelArr[static_cast<_uint>(_eID)])
	{
		m_pChannelArr[static_cast<_uint>(_eID)]->isPlaying(&bPlaying);
		if (bPlaying)
		{
			m_pChannelArr[static_cast<_uint>(_eID)]->stop();
		}
	}

	m_pSystem->playSound(iter->second, nullptr, false, &m_pChannelArr[static_cast<_uint>(_eID)]);
	m_pChannelArr[static_cast<_uint>(_eID)]->setVolume(_fVolume);

	m_pSystem->update();
}

void CSoundManager::Stop_Sound(EChannel _eID)
{
	if (m_pChannelArr[static_cast<_uint>(_eID)])
		m_pChannelArr[static_cast<_uint>(_eID)]->stop();
}

void CSoundManager::Stop_All()
{
	for (_uint i = 0; i < static_cast<_uint>(EChannel::ECHANNEL_END); i++)
		if (m_pChannelArr[i])
			m_pChannelArr[i]->stop();
}

void CSoundManager::Set_ChannelVolume(EChannel _eID, _float _fVolume)
{
	if (m_pChannelArr[static_cast<_uint>(_eID)])
		m_pChannelArr[static_cast<_uint>(_eID)]->setVolume(_fVolume);

	m_pSystem->update();
}

HRESULT CSoundManager::Ready_SoundFile()
{
	_finddata_t fd;

	intptr_t handle = _findfirst("../../../Resource/Raw/Sound/*.*", &fd);

	if (handle == -1)
		return E_FAIL;

	_int iResult = 0;

	_char szCurPath[128] = "../../../Resource/Raw/Sound/";
	_char szFullPath[128] = "";

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);
		strcat_s(szFullPath, fd.name);

		FMOD::Sound* pSound = nullptr;

		FMOD_RESULT eRes = m_pSystem->createSound(szFullPath, FMOD_DEFAULT, 0, &pSound);

		if (eRes == FMOD_OK)
		{
			_int iLength = (_int)strlen(fd.name) + 1;

			_tchar* pSoundKey = new _tchar[iLength];
			ZeroMemory(pSoundKey, sizeof(_tchar) * iLength);

			MultiByteToWideChar(CP_ACP, 0, fd.name, -1, pSoundKey, static_cast<_int>(iLength));

			m_mapSound.emplace(pSoundKey, pSound);
		}

		iResult = _findnext(handle, &fd);
	}

	m_pSystem->update();

	_findclose(handle);

	return S_OK;
}

CSoundManager* CSoundManager::Create()
{
    CSoundManager* pInstance = new CSoundManager();

    if (FAILED(pInstance->Init()))
    {
        MSG_BOX("Failed To Create : CSoundManager");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSoundManager::Free()
{
    __super::Free();

	for (auto& Mypair : m_mapSound)
	{
		delete[] Mypair.first;
		Mypair.second->release();
	}

	m_mapSound.clear();

	m_pSystem->close();
	m_pSystem->release();
}