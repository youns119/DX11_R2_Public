#include "CCollider.h"

CCollider::CCollider(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice{ _pDevice }
	, m_pContext{ _pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

void CCollider::Free()
{
	__super::Free();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}