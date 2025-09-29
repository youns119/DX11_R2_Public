#include "CPipeLine.h"

CPipeLine::CPipeLine()
{
}

HRESULT CPipeLine::Init()
{
	for (size_t i = 0; i < static_cast<size_t>(ED3DTransform::D3DTS_END); i++)
	{
		XMStoreFloat4x4(&m_matTransforms[i], XMMatrixIdentity());
		XMStoreFloat4x4(&m_matTransformsInverse[i], XMMatrixIdentity());
	}

	m_vCamPos = _float4(0.f, 0.f, 0.f, 1.f);

	return S_OK;
}

void CPipeLine::Update()
{
	for (size_t i = 0; i < static_cast<size_t>(ED3DTransform::D3DTS_END); i++)
		XMStoreFloat4x4
		(
			&m_matTransformsInverse[i],
			XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_matTransforms[i]))
		);

	XMStoreFloat4
	(
		&m_vCamPos,
		XMLoadFloat4x4(&m_matTransformsInverse[static_cast<_uint>(ED3DTransform::D3DTS_VIEW)]).r[3]
	);
}

CPipeLine* CPipeLine::Create()
{
	CPipeLine* pInstance = new CPipeLine();

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Create : CPipeLine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPipeLine::Free()
{
	__super::Free();
}