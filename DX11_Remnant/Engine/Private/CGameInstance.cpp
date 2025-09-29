#include "CGameInstance.h"

IGraphicDevice* CGameInstance::m_pGraphicDevice = nullptr;
IInputDevice* CGameInstance::m_pInputDevice = nullptr;
ITimerManager* CGameInstance::m_pTimerManager = nullptr;
ISoundManager* CGameInstance::m_pSoundManager = nullptr;

IPrototypeManager* CGameInstance::m_pPrototypeManager = nullptr;
ITargetManager* CGameInstance::m_pTargetManager = nullptr;
IRenderer* CGameInstance::m_pRenderer = nullptr;
IPipeLine* CGameInstance::m_pPipeLine = nullptr;
IFrustum* CGameInstance::m_pFrustum = nullptr;
IWorld* CGameInstance::m_pWorld = nullptr;
IFontManager* CGameInstance::m_pFontManager = nullptr;

IPicking* CGameInstance::m_pPicking = nullptr;

_uint CGameInstance::m_iViewportHeight = 0;
_uint CGameInstance::m_iViewportWidth = 0;

CActor* CGameInstance::m_pController = nullptr;

CGameInstance::CGameInstance()
{
}

HRESULT CGameInstance::Init
(
	IGraphicDevice* _pGraphicDevice,
	IInputDevice* _pInputDevice,
	ITimerManager* _pTimerManager,
	ISoundManager* _pSoundManager,
	IPrototypeManager* _pPrototypeManager,
	ITargetManager* _pTargetManager,
	IRenderer* _pRenderer,
	IPipeLine* _pPipeLine,
	IFrustum* _pFrustum,
	IWorld* _pWorld,
	IFontManager* _pFontManager,
	IPicking* _pPicking,
	const FEngine& _tEngineDesc
)
{
	m_pGraphicDevice = _pGraphicDevice;
	m_pInputDevice = _pInputDevice;
	m_pTimerManager = _pTimerManager;
	m_pSoundManager = _pSoundManager;

	m_pPrototypeManager = _pPrototypeManager;
	m_pTargetManager = _pTargetManager;
	m_pRenderer = _pRenderer;
	m_pPipeLine = _pPipeLine;
	m_pFrustum = _pFrustum;
	m_pWorld = _pWorld;
	m_pFontManager = _pFontManager;

	m_pPicking = _pPicking;

	m_iViewportWidth = _tEngineDesc.iViewportWidth;
	m_iViewportHeight = _tEngineDesc.iViewportHeight;

	return S_OK;
}

void CGameInstance::Pre_Update(_float _fTimeDelta)
{
	m_pInputDevice->Update();
	m_pPicking->Update();
	m_pWorld->Pre_Update(_fTimeDelta);
}

void CGameInstance::Update(_float _fTimeDelta)
{
	m_pPipeLine->Update();
	m_pFrustum->Update();
	m_pWorld->Update(_fTimeDelta);
}

void CGameInstance::Post_Update(_float _fTimeDelta)
{
	m_pWorld->Post_Update(_fTimeDelta);
}

HRESULT CGameInstance::Render_Begin(const _float4& _vClearColor)
{
	if (FAILED(m_pGraphicDevice->Clear_BackBuffer_View(_vClearColor)) ||
		FAILED(m_pGraphicDevice->Clear_DepthStencil_View()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameInstance::Draw()
{
	if (FAILED(m_pWorld->Render()) ||
		FAILED(m_pRenderer->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameInstance::Render_End()
{
	if (FAILED(m_pGraphicDevice->Present()))
		return E_FAIL;

	return S_OK;
}

#pragma region TIMERMANAGER

HRESULT CGameInstance::Add_Timer(const _wstring& _strTimerTag)
{
	return m_pTimerManager->Add_Timer(_strTimerTag);
}

void CGameInstance::Update_Timer(const _wstring& _strTimerTag)
{
	m_pTimerManager->Update_Timer(_strTimerTag);
}

_float CGameInstance::Get_TimeDelta(const _wstring& _strTimerTag)
{
	return m_pTimerManager->Get_TimeDelta(_strTimerTag);
}

#pragma endregion

#pragma region INPUT_DEVICE

_bool CGameInstance::Key_Press(_ubyte _byKeyID)
{
	return m_pInputDevice->Key_Press(_byKeyID);
}

_bool CGameInstance::Key_Hold(_ubyte _byKeyID)
{
	return m_pInputDevice->Key_Hold(_byKeyID);
}

_bool CGameInstance::Key_Release(_ubyte _byKeyID)
{
	return m_pInputDevice->Key_Release(_byKeyID);
}

_bool CGameInstance::Mouse_Press(EMouseKey _eMouseKey)
{
	return m_pInputDevice->Mouse_Press(_eMouseKey);
}

_bool CGameInstance::Mouse_Hold(EMouseKey _eMouseKey)
{
	return m_pInputDevice->Mouse_Hold(_eMouseKey);
}

_bool CGameInstance::Mouse_Release(EMouseKey _eMouseKey)
{
	return m_pInputDevice->Mouse_Release(_eMouseKey);
}

_long CGameInstance::Get_DIMouseMove(EMouseDir _eMouseDir)
{
	return m_pInputDevice->Get_DIMouseMove(_eMouseDir);
}

#pragma endregion

#pragma region PROTOTYPEMANAGER

HRESULT CGameInstance::Add_Prototype
(
	_uint _iPrototypeLevelIndex, 
	const _wstring& _strPrototypeTag, 
	CClonable* _pPrototype
)
{
	return m_pPrototypeManager->Add_Prototype(_iPrototypeLevelIndex, _strPrototypeTag, _pPrototype);
}

CClonable* CGameInstance::Clone_Prototype
(
	_uint _iPrototypeLevelIndex, 
	const _wstring& _strPrototypeTag,
	void* _pDesc
)
{
	return m_pPrototypeManager->Clone_Prototype(_iPrototypeLevelIndex, _strPrototypeTag, _pDesc);
}

void CGameInstance::Clear(_uint _iLevelIndex)
{
	m_pPrototypeManager->Clear(_iLevelIndex);
}

#pragma endregion

#pragma region TARGETMANAGER

HRESULT CGameInstance::Add_RenderTarget
(
	const _wstring& _strRenderTargetTag, 
	_uint _iWidth, _uint _iHeight, 
	DXGI_FORMAT _ePixelFormat, 
	const _float4& _vClearColor
)
{
	return m_pTargetManager->Add_RenderTarget(_strRenderTargetTag, _iWidth, _iHeight, _ePixelFormat, _vClearColor);
}

HRESULT CGameInstance::Add_MRT(const _wstring& _strMRTTag, const _wstring& _strRenderTargetTag)
{
	return m_pTargetManager->Add_MRT(_strMRTTag, _strRenderTargetTag);
}

HRESULT CGameInstance::Begin_MRT(const _wstring& _strMRTTag)
{
	return m_pTargetManager->Begin_MRT(_strMRTTag);
}

HRESULT CGameInstance::End_MRT()
{
	return m_pTargetManager->End_MRT();
}

HRESULT CGameInstance::Bind_RT_ShaderResource
(
	const _wstring& _strRenderTargetTag, 
	CShader* _pShader, 
	const _char* _pConstantName
)
{
	return m_pTargetManager->Bind_RT_ShaderResource(_strRenderTargetTag, _pShader, _pConstantName);
}

HRESULT CGameInstance::Copy_RT_Resource(const _wstring& _strRenderTargetTag, ID3D11Texture2D* _pTexture2D)
{
	return m_pTargetManager->Copy_RT_Resource(_strRenderTargetTag, _pTexture2D);
}

#ifdef _DEBUG

HRESULT CGameInstance::Ready_RT_Debug
(
	const _wstring& _strRenderTargetTag, 
	_float _fX, _float _fY, 
	_float _fSizeX, _float _fSizeY
)
{
	return m_pTargetManager->Ready_RT_Debug(_strRenderTargetTag, _fX, _fY, _fSizeX, _fSizeY);
}

HRESULT CGameInstance::Render_RT_Debug(const _wstring& _strMRTTag, CShader* _pShader, CVIBuffer_Rect* _pVIBuffer)
{
	return m_pTargetManager->Render_RT_Debug(_strMRTTag, _pShader, _pVIBuffer);
}

#endif

#pragma endregion

#pragma region RENDERER

HRESULT CGameInstance::Add_RenderGroup(ERender _eRender, CPrimitiveComponent* _pComponent)
{
	return m_pRenderer->Add_RenderGroup(_eRender, _pComponent);
}

HRESULT CGameInstance::Add_Light(CLightComponent* _pComponent)
{
	return m_pRenderer->Add_Light(_pComponent);
}

#ifdef _DEBUG

HRESULT CGameInstance::Add_DebugComponent(CPrimitiveComponent* _pComponent)
{
	return m_pRenderer->Add_DebugComponent(_pComponent);
}

#endif

#pragma endregion

#pragma region PIPELINE

_float4x4 CGameInstance::Get_TransformFloat4x4(ED3DTransform _eD3DTransform)
{
	return m_pPipeLine->Get_TransformFloat4x4(_eD3DTransform);;
}

_matrix CGameInstance::Get_TransformMatrix(ED3DTransform _eD3DTransform)
{
	return m_pPipeLine->Get_TransformMatrix(_eD3DTransform);
}

_float4x4 CGameInstance::Get_TransformFloat4x4_Inverse(ED3DTransform _eD3DTransform)
{
	return m_pPipeLine->Get_TransformFloat4x4_Inverse(_eD3DTransform);
}

_matrix CGameInstance::Get_TransformMatrix_Inverse(ED3DTransform _eD3DTransform)
{
	return m_pPipeLine->Get_TransformMatrix_Inverse(_eD3DTransform);
}

_float4 CGameInstance::Get_CamPos()
{
	return m_pPipeLine->Get_CamPos();
}

void CGameInstance::Set_Transform(ED3DTransform _eD3DTransform, _fmatrix matTransform)
{
	m_pPipeLine->Set_Transform(_eD3DTransform, matTransform);
}

#pragma endregion

#pragma region FRUSTUM

_bool CGameInstance::IsIn_WorldSpace(_fvector _vPosition, _float _fRange)
{
	return m_pFrustum->IsIn_WorldSpace(_vPosition, _fRange);
}

#pragma endregion

#pragma region FONTMANAGER

HRESULT CGameInstance::Render_Font
(
	const _wstring& _strFontTag, 
	const _tchar* _pText, 
	const _float2& _vPosition, 
	_fvector _vColor, 
	_float _fRotation, 
	const _float2& _vOrigin, 
	const _float _fScale
)
{
	return m_pFontManager->Render_Font(_strFontTag, _pText, _vPosition, _vColor, _fRotation, _vOrigin, _fScale);
}

HRESULT CGameInstance::Add_Font(const _wstring& _strFontTag, const _tchar* _pFontFilePath)
{
	return m_pFontManager->Add_Font(_strFontTag, _pFontFilePath);
}

#pragma endregion

#pragma region PICKING

_bool CGameInstance::Compute_PickPos(_float3* _pOut)
{
	return m_pPicking->Compute_PickPos(_pOut);
}

#pragma endregion