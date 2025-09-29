#include "CNavigationComponent.h"

#include "CCell.h"

#ifdef _DEBUG

#include "CGameInstance.h"
#include "CShader.h"

#include "FVertex_Pos.h"

#endif

_float4x4 CNavigationComponent::m_matWorld{};
_float4x4 CNavigationComponent::m_matWorldInverse{};

CNavigationComponent::CNavigationComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CActorComponent{ _pDevice, _pContext }
{
	m_strName = g_strNavigationComponent;
}

CNavigationComponent::CNavigationComponent(const CNavigationComponent& _Prototype)
	: CActorComponent(_Prototype)
	, m_vecCell{ _Prototype.m_vecCell }
#ifdef _DEBUG

	, m_pShader{ _Prototype.m_pShader }

#endif
{
	for (auto& Cell : m_vecCell)
		Safe_AddRef(Cell);

#ifdef _DEBUG

	Safe_AddRef(m_pShader);

#endif
}

HRESULT CNavigationComponent::Init(const _tchar* _pNavigationDataFile)
{
	_ulong dwByte = {};
	HANDLE hFile = CreateFile(_pNavigationDataFile, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == 0)
		return E_FAIL;

	while (true)
	{
		_float3 vPoints[static_cast<_uint>(EPoint::POINT_END)]{};

		if (!ReadFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr))
			return E_FAIL;

		if (dwByte == 0)
			break;

		CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, static_cast<_int>(m_vecCell.size()));
		if (pCell == nullptr)
			return E_FAIL;

		m_vecCell.push_back(pCell);
	}

	CloseHandle(hFile);

	XMStoreFloat4x4(&m_matWorld, XMMatrixIdentity());
	XMStoreFloat4x4(&m_matWorldInverse, XMMatrixIdentity());

#ifdef _DEBUG

	m_pShader = CShader::Create
	(
		m_pDevice, 
		m_pContext, 
		SHADER("Shader_VtxPos.hlsl"),
		VTXPOS::tElements, 
		VTXPOS::iNumElement
	);
	if (nullptr == m_pShader)
		return E_FAIL;

#endif

	if (FAILED(SetUp_Neighbors()))
		return E_FAIL;

	const _char* temp = "asdf";

	SHADER(temp);

	return S_OK;
}

HRESULT CNavigationComponent::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	FNavigationComponent* pNavigationDesc = static_cast<FNavigationComponent*>(_pDesc);
	m_iCurrCellIndex = pNavigationDesc->iCurrCellIndex;

	if (FAILED(__super::Init(_pDesc)))
		return E_FAIL;

	return S_OK;
}

#ifdef _DEBUG

HRESULT CNavigationComponent::Render()
{
	//return S_OK;

	_float4x4 matView{}, matProj{};

	matView = CGameInstance::Get_TransformFloat4x4(ED3DTransform::D3DTS_VIEW);
	matProj = CGameInstance::Get_TransformFloat4x4(ED3DTransform::D3DTS_PROJ);

	if (FAILED(m_pShader->Bind_Matrix("g_matWorld", &m_matWorld))	||
		FAILED(m_pShader->Bind_Matrix("g_matView", &matView))		||
		FAILED(m_pShader->Bind_Matrix("g_matProj", &matProj)))
		return E_FAIL;

	_float4 vColor = m_iCurrCellIndex == -1 ? _float4(0.f, 1.f, 0.f, 1.f) : _float4(1.f, 0.f, 0.f, 1.f);
	if (FAILED(m_pShader->Bind_RawValue("g_vColor", &vColor, sizeof(_float4))))
		return E_FAIL;

	if (m_iCurrCellIndex == -1)
	{
		m_pShader->Begin(0);

		for (auto& pCell : m_vecCell)
			pCell->Render();
	}
	else
	{
		_float4x4 matWorld = m_matWorld;

		matWorld.m[3][1] += 0.1f;

		if (FAILED(m_pShader->Bind_Matrix("g_matWorld", &matWorld)))
			return E_FAIL;

		m_pShader->Begin(0);

		m_vecCell[m_iCurrCellIndex]->Render();
	}

	return S_OK;
}

#endif

_bool CNavigationComponent::IsMove(_fvector _vWorldPos)
{
	_vector vPosition = XMVector3TransformCoord(_vWorldPos, XMLoadFloat4x4(&m_matWorldInverse));

	_int iNeighborIndex = { -1 };

	if (m_vecCell[m_iCurrCellIndex]->IsIn(vPosition, &iNeighborIndex) == false)
	{
		if (iNeighborIndex != -1)
		{
			while (true)
				if (m_vecCell[iNeighborIndex]->IsIn(vPosition, &iNeighborIndex))
					break;

			m_iCurrCellIndex = iNeighborIndex;

			return true;
		}
		else
			return false;
	}
	else
		return true;
}

_float CNavigationComponent::Compute_Height(_fvector _vWorldPos)
{
	if (m_iCurrCellIndex == -1)
		return 0.f;

	_vector vPosition = XMVector3TransformCoord(_vWorldPos, XMLoadFloat4x4(&m_matWorldInverse));

	vPosition = XMVectorSetY(vPosition, m_vecCell[m_iCurrCellIndex]->Compute_Height(vPosition));
	vPosition = XMVector3TransformCoord(vPosition, XMLoadFloat4x4(&m_matWorld));

	return XMVectorGetY(vPosition);
}

HRESULT CNavigationComponent::SetUp_Neighbors()
{
	for (auto& SrcCell : m_vecCell)
	{
		for (auto& DstCell : m_vecCell)
		{
			if (SrcCell == DstCell)
				continue;

			if (DstCell->Compare_Points(SrcCell->Get_Point(EPoint::POINT_A), SrcCell->Get_Point(EPoint::POINT_B)))
				SrcCell->Set_Neighbor(ELine::LINE_AB, DstCell);
			if (DstCell->Compare_Points(SrcCell->Get_Point(EPoint::POINT_B), SrcCell->Get_Point(EPoint::POINT_C)))
				SrcCell->Set_Neighbor(ELine::LINE_BC, DstCell);
			if (DstCell->Compare_Points(SrcCell->Get_Point(EPoint::POINT_C), SrcCell->Get_Point(EPoint::POINT_A)))
				SrcCell->Set_Neighbor(ELine::LINE_CA, DstCell);
		}
	}

	return S_OK;
}

CNavigationComponent* CNavigationComponent::Create
(
	ID3D11Device* _pDevice, 
	ID3D11DeviceContext* _pContext, 
	const _tchar* _pNavigationDataFile
)
{
	CNavigationComponent* pInstance = new CNavigationComponent(_pDevice, _pContext);

	if (FAILED(pInstance->Init(_pNavigationDataFile)))
	{
		MSG_BOX("Failed To Create : CNavigationComponent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CNavigationComponent::Clone(void* _pDesc)
{
	CNavigationComponent* pInstance = new CNavigationComponent(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Create : CNavigationComponent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigationComponent::Free()
{
	__super::Free();

#ifdef _DEBUG

	Safe_Release(m_pShader);

#endif

	for (auto& Cell : m_vecCell)
		Safe_Release(Cell);

	m_vecCell.clear();
}