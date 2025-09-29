#include "pch.h"
#include "CSky_Stage1.h"

#include "CGameInstance.h"
#include "CCubeComponent.h"

#include "ELevel.h"

CSky_Stage1::CSky_Stage1(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CSkyBox{ _pDevice, _pContext }
{
}

CSky_Stage1::CSky_Stage1(const CSky_Stage1& _Prototype)
	: CSkyBox(_Prototype)
{
}

HRESULT CSky_Stage1::Init()
{
	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSky_Stage1::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	FSkyBox* pSkyBoxDesc = static_cast<FSkyBox*>(_pDesc);
	pSkyBoxDesc->eRenderType = ERender::RENDER_PRIORITY;

	if (FAILED(__super::Init(pSkyBoxDesc)))
		return E_FAIL;

	return S_OK;
}

void CSky_Stage1::Pre_Update(_float _fTimeDelta)
{
	__super::Pre_Update(_fTimeDelta);
}

void CSky_Stage1::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);
}

void CSky_Stage1::Post_Update(_float _fTimeDelta)
{
	__super::Post_Update(_fTimeDelta);
}

HRESULT CSky_Stage1::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSky_Stage1::Ready_Component()
{
	return S_OK;
}

HRESULT CSky_Stage1::Ready_Resource()
{
	// Shader
	if(FAILED(m_pCubeComponent->Add_Shader
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		TEXT("Shader_VtxCube")
	)))
		return E_FAIL;

	// Texture
	if (FAILED(m_pCubeComponent->Add_Texture
	(
		static_cast<_uint>(ELevel::LEVEL_STAGE1),
		TEXT("Texture_Sky")
	)))
		return E_FAIL;

	if (FAILED(m_pCubeComponent->Bind_Material("g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

CSky_Stage1* CSky_Stage1::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CSky_Stage1* pInstance = new CSky_Stage1(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Create : CSky_Level1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CSky_Stage1::Clone(void* _pDesc)
{
	CSky_Stage1* pInstance = new CSky_Stage1(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Create : CSky_Stage1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSky_Stage1::Free()
{
	__super::Free();
}