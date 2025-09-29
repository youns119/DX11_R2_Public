#include "pch.h"
#include "CSky_Default.h"

#include "CGameInstance.h"
#include "CCubeComponent.h"

#include "ELevel.h"

CSky_Default::CSky_Default(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CSkyBox{ _pDevice, _pContext }
{
}

CSky_Default::CSky_Default(const CSky_Default& _Prototype)
	: CSkyBox(_Prototype)
{
}

HRESULT CSky_Default::Init()
{
	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSky_Default::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	FSkyBox* pSkyBoxDesc = static_cast<FSkyBox*>(_pDesc);
	pSkyBoxDesc->eRenderType = ERender::RENDER_PRIORITY;

	if (FAILED(__super::Init(pSkyBoxDesc)))
		return E_FAIL;

	return S_OK;
}

void CSky_Default::Pre_Update(_float _fTimeDelta)
{
	__super::Pre_Update(_fTimeDelta);
}

void CSky_Default::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);
}

void CSky_Default::Post_Update(_float _fTimeDelta)
{
	__super::Post_Update(_fTimeDelta);
}

HRESULT CSky_Default::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSky_Default::Ready_Component()
{
	return S_OK;
}

HRESULT CSky_Default::Ready_Resource()
{
	// Shader
	if (FAILED(m_pCubeComponent->Add_Shader
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

	if (FAILED(m_pCubeComponent->Bind_Material("g_Texture", 2)))
		return E_FAIL;

	return S_OK;
}

CSky_Default* CSky_Default::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CSky_Default* pInstance = new CSky_Default(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Create : CSky_Level1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CSky_Default::Clone(void* _pDesc)
{
	CSky_Default* pInstance = new CSky_Default(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Create : CSky_Default");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSky_Default::Free()
{
	__super::Free();
}