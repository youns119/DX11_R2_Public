#include "CAnimStateMachine.h"

#include "CCharacter.h"
#include "CAnimState.h"

CAnimStateMachine::CAnimStateMachine()
{
}

HRESULT CAnimStateMachine::Init
(
	CCharacter* _pCharacter, 
	_uint* _pState, 
	const _wstring& _strState
)
{
	if (FAILED(Ready_AnimState()))
		return E_FAIL;

	m_pCharacter = _pCharacter;
	Safe_AddRef(m_pCharacter);

	m_pState = _pState;
	ChangeState(_strState);

	return S_OK;
}

void CAnimStateMachine::Update(_float _fTimeDelta)
{
	if(m_pCurrAnimState)
		m_pCurrAnimState->Update(m_pCharacter, m_pState, _fTimeDelta);
}

HRESULT CAnimStateMachine::ChangeState(const _wstring& _strState)
{
	CAnimState* pAnimState = Find_AnimState(_strState);
	if (pAnimState == nullptr)
		return E_FAIL;

	if (m_pCurrAnimState)
		m_pCurrAnimState->Exit(m_pCharacter, m_pState);

	m_pCurrAnimState = pAnimState;
	m_pCurrAnimState->Enter(m_pCharacter, m_pState);

	return S_OK;
}

void CAnimStateMachine::Clear()
{
	Safe_Release(m_pCharacter);
}

HRESULT CAnimStateMachine::Add_AnimState(const _wstring& _strAnimState, CAnimState* _pAnimState)
{
	if (_pAnimState == nullptr ||
		Find_AnimState(_strAnimState) != nullptr)
		return E_FAIL;

	m_mapAnimState.emplace(_strAnimState, _pAnimState);

	return S_OK;
}

CAnimState* CAnimStateMachine::Find_AnimState(const _wstring& _strAnimState)
{
	auto iter = m_mapAnimState.find(_strAnimState);

	if (iter == m_mapAnimState.end())
		return nullptr;

	return iter->second;
}

void CAnimStateMachine::Free()
{
	__super::Free();

	for (auto& AnimState : m_mapAnimState)
		Safe_Release(AnimState.second);

	m_mapAnimState.clear();
}