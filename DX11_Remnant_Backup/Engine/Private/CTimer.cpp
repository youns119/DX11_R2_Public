#include "CTimer.h"

CTimer::CTimer() 
{
}

HRESULT CTimer::Init()
{
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_LastTime);
	QueryPerformanceCounter(&m_FixTime);

	QueryPerformanceFrequency(&m_CpuTick);

	return S_OK;
}

void CTimer::Update()
{
	QueryPerformanceCounter(&m_FrameTime);

	if (m_FrameTime.QuadPart - m_FixTime.QuadPart >= m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_FixTime = m_FrameTime;
	}

	m_fTimeDelta = (m_FrameTime.QuadPart - m_LastTime.QuadPart) / (_float)m_CpuTick.QuadPart;
	m_LastTime = m_FrameTime;
}

CTimer* CTimer::Create()
{
	CTimer* pInstance = new CTimer;

	if (FAILED(pInstance->Init()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTimer::Free()
{
	__super::Free();
}