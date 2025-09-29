#pragma once

#include "IRenderer.h"

#include "CObject.h"

BEGIN(Engine)

class CTargetManager;
class CVIBuffer_Rect;
class CShader;

class ENGINE_DLL CRenderer final 
	: public IRenderer
	, public CObject
{
private :
	CRenderer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CRenderer() = default;

public :
	HRESULT Init(CTargetManager* _pTargetManager);
	virtual HRESULT Render() override;

public :
	virtual HRESULT Add_RenderGroup(ERender _eRender, CPrimitiveComponent* _pComponent) override;
	virtual HRESULT Add_Light(CLightComponent* _pComponent) override;

#ifdef _DEBUG

public :
	virtual HRESULT Add_DebugComponent(CPrimitiveComponent* _pComponent) override;

#endif

private :
	ID3D11Device* m_pDevice{};
	ID3D11DeviceContext* m_pContext{};

private :
	CShader* m_pShader{};
	CVIBuffer_Rect* m_pVIBuffer{};

private :
	_float4x4 m_matWorld{}, m_matView{}, m_matProj{};

private :
	list<CPrimitiveComponent*> m_listRender[static_cast<_uint>(ERender::RENDER_END)];
	list<CLightComponent*> m_listLight;

#ifdef _DEBUG

private :
	list<CPrimitiveComponent*> m_listDebugComponent;

#endif

private :
	HRESULT Render_Priority();
	HRESULT Render_NonBlend();
	HRESULT Render_LightAcc();
	HRESULT Render_Deferred();
	HRESULT Render_NonLight();
	HRESULT Render_Blend();
	HRESULT Render_UI();

#ifdef _DEBUG

private :
	HRESULT Render_Debug();

#endif

public :
	static CRenderer* Create
	(
		ID3D11Device* _pDevice, 
		ID3D11DeviceContext* _pContext, 
		CTargetManager* _pTargetManager
	);
	virtual void Free() override;
};

END