#pragma once

#include "Engine_Define.h"

#include "IGraphicDevice.h"
#include "IInputDevice.h"
#include "ITimerManager.h"
#include "ISoundManager.h"

#include "IPrototypeManager.h"
#include "ITargetManager.h"
#include "IRenderer.h"
#include "IPipeLine.h"
#include "IFrustum.h"
#include "IWorld.h"
#include "IFontManager.h"

#include "IPicking.h"

#include "FEngine.h"

BEGIN(Engine)

class CClonable;
class CActor;
class CPrimitiveComponent;

class ENGINE_DLL CGameInstance final 
{
private :
	CGameInstance();
	virtual ~CGameInstance() = default;

public :
	static HRESULT Init
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
	);
	static void Pre_Update(_float _fTimeDelta);
	static void Update(_float _fTimeDelta);
	static void Post_Update(_float _fTimeDelta);
	static HRESULT Render_Begin(const _float4& _vClearColor);
	static HRESULT Draw();
	static HRESULT Render_End();

public :
	static IGraphicDevice* Get_GraphicDevice() { return m_pGraphicDevice; }
	static IInputDevice* Get_InputDevice() { return m_pInputDevice; }
	static ITimerManager* Get_TimerManager() { return m_pTimerManager; }
	static ISoundManager* Get_SoundManager() { return m_pSoundManager; }

	static IPrototypeManager* Get_PrototypeManager() { return m_pPrototypeManager; }
	static ITargetManager* Get_TargetManager() { return m_pTargetManager; }
	static IRenderer* Get_Renderer() { return m_pRenderer; }
	static IPipeLine* Get_PipeLine() { return m_pPipeLine; }
	static IFrustum* Get_Frustum() { return m_pFrustum; }
	static IWorld* Get_World() { return m_pWorld; }
	static IFontManager* Get_FontManager() { return m_pFontManager; }

	static IPicking* Get_Picking() { return m_pPicking; }

#pragma region TIMERMANAGER

public :
	static HRESULT Add_Timer(const _wstring& _strTimerTag);
	static void Update_Timer(const _wstring& _strTimerTag);

	static _float Get_TimeDelta(const _wstring& _strTimerTag);

#pragma endregion

#pragma region INPUTMANAGER

public :
	static _bool Key_Press(_ubyte _byKeyID);
	static _bool Key_Hold(_ubyte _byKeyID);
	static _bool Key_Release(_ubyte _byKeyID);

	static _bool Mouse_Press(EMouseKey _eMouseKey);
	static _bool Mouse_Hold(EMouseKey _eMouseKey);
	static _bool Mouse_Release(EMouseKey _eMouseKey);

	static _long Get_DIMouseMove(EMouseDir _eMouseDir);

#pragma endregion

#pragma region PROTOTYPEMANAGER

	static HRESULT Add_Prototype
	(
		_uint _iPrototypeLevelIndex, 
		const _wstring& _strPrototypeTag, 
		CClonable* _pPrototype
	);
	static CClonable* Clone_Prototype
	(
		_uint _iPrototypeLevelIndex, 
		const _wstring& _strPrototypeTag, 
		void* _pDesc = nullptr
	);
	static void Clear(_uint _iLevelIndex);

#pragma endregion

#pragma region TARGETMANAGER

public :
	static HRESULT Add_RenderTarget
	(
		const _wstring& _strRenderTargetTag,
		_uint _iWidth, _uint _iHeight,
		DXGI_FORMAT _ePixelFormat,
		const _float4& _vClearColor
	);
	static HRESULT Add_MRT(const _wstring& _strMRTTag, const _wstring& _strRenderTargetTag);
	static HRESULT Begin_MRT(const _wstring& _strMRTTag);
	static HRESULT End_MRT();

public :
	static HRESULT Bind_RT_ShaderResource
	(
		const _wstring& _strRenderTargetTag,
		CShader* _pShader,
		const _char* _pConstantName
	);
	static HRESULT Copy_RT_Resource(const _wstring& _strRenderTargetTag, ID3D11Texture2D* _pTexture2D);

#ifdef _DEBUG

public :
	static HRESULT Ready_RT_Debug
	(
		const _wstring& _strRenderTargetTag,
		_float _fX, _float _fY,
		_float _fSizeX, _float _fSizeY
	);
	static HRESULT Render_RT_Debug(const _wstring& _strMRTTag, CShader* _pShader, CVIBuffer_Rect* _pVIBuffer);

#endif

#pragma endregion

#pragma region RENDERER

public :
	static HRESULT Add_RenderGroup(ERender _eRender, CPrimitiveComponent* _pComponent);
	static HRESULT Add_Light(CLightComponent* _pComponent);

#ifdef _DEBUG

public :
	static HRESULT Add_DebugComponent(CPrimitiveComponent* _pComponent);

#endif

#pragma endregion

#pragma region PIPELINE

public :
	static _float4x4 Get_TransformFloat4x4(ED3DTransform _eD3DTransform);
	static _matrix Get_TransformMatrix(ED3DTransform _eD3DTransform);
	static _float4x4 Get_TransformFloat4x4_Inverse(ED3DTransform _eD3DTransform);
	static _matrix Get_TransformMatrix_Inverse(ED3DTransform _eD3DTransform);
	static _float4 Get_CamPos();

	static void Set_Transform(ED3DTransform _eD3DTransform, _fmatrix matTransform);

#pragma endregion

#pragma region FRUSTUM

	static _bool IsIn_WorldSpace(_fvector _vPosition, _float _fRange = 0.f);

#pragma endregion

#pragma region FONTMANAGER

public :
	static HRESULT Render_Font
	(
		const _wstring& _strFontTag,
		const _tchar* _pText,
		const _float2& _vPosition,
		_fvector _vColor = XMVectorSet(0.f, 1.f, 0.f, 1.f),
		_float _fRotation = 0.f,
		const _float2& _vOrigin = _float2(0.f, 0.f),
		const _float _fScale = 1.f
	);
	static HRESULT Add_Font(const _wstring& _strFontTag, const _tchar* _pFontFilePath);

#pragma endregion

#pragma region PICKING

public :
	static _bool Compute_PickPos(_float3* _pOut);

#pragma endregion

public :
	static _uint2 Get_ViewportSize()
	{ return _uint2(m_iViewportWidth, m_iViewportHeight); }

public :
	static CActor* Get_Controller()
	{ return m_pController; }

	static void Set_Controller(CActor* _pActor)
	{ m_pController = _pActor; }

private :
	static IGraphicDevice* m_pGraphicDevice;
	static IInputDevice* m_pInputDevice;
	static ITimerManager* m_pTimerManager;
	static ISoundManager* m_pSoundManager;
	
	static IPrototypeManager* m_pPrototypeManager;
	static ITargetManager* m_pTargetManager;
	static IRenderer* m_pRenderer;
	static IPipeLine* m_pPipeLine;
	static IFrustum* m_pFrustum;
	static IWorld* m_pWorld;
	static IFontManager* m_pFontManager;

	static IPicking* m_pPicking;

private :
	static _uint m_iViewportWidth;
	static _uint m_iViewportHeight;

private :
	static CActor* m_pController;
};

#define WORLD CGameInstance::Get_World()
#define SOUND CGameInstance::Get_SoundManager()

END