#pragma once

#include "CSceneComponent.h"

#include "FPrimitiveComponent.h"

BEGIN(Engine)

class CShader;

class ENGINE_DLL CPrimitiveComponent abstract
	: public CSceneComponent
{
protected :
	CPrimitiveComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CPrimitiveComponent(const CPrimitiveComponent& _Prototype);
	virtual ~CPrimitiveComponent() = default;

public :
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Pre_Update(_float _fTimeDelta);
	virtual void Update(_float _fTimeDelta);
	virtual void Post_Update(_float _fTimeDelta);
	virtual HRESULT Render() override;

public :
	CShader* Get_Shader() const { return m_pShader; }

public :
	void Set_Show(_bool _bShow) { m_bShow = _bShow; }
	void Set_Pass(_uint _iPassIndex) { m_iPassIndex = _iPassIndex; }

public :
	HRESULT Add_Shader(_uint _iPrototypeLevelIndex, const _wstring& _strPrototypeTag);

protected :
	_float4x4 m_matView{}, m_matProj{};

protected :
	CShader* m_pShader{};

protected :
	ERender m_eRenderType{ ERender::RENDER_END };

	_bool m_bShow{ true };
	_bool m_bCulling{ false };
	_uint m_iPassIndex{};
	_float m_fCullingRange{};

protected :
	HRESULT Begin_Shader();

protected :
	virtual HRESULT Bind_ShaderResource();

public :
	virtual void Free() override;
};

END