#pragma once

#include "CSceneComponent.h"

#include "FLightComponent.h"

BEGIN(Engine)

class CVIBuffer_Rect;
class CShader;

class ENGINE_DLL CLightComponent final
	: public CSceneComponent
{
protected :
	CLightComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CLightComponent(const CLightComponent& _Prototype);
	virtual ~CLightComponent() = default;

public :
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Post_Update(_float _fTimeDelta) override;
	virtual HRESULT Render(CShader* _pShader, CVIBuffer_Rect* _pVIBuffer);

public :
	_float4 Get_Diffuse() { return m_vDiffuse; }
	_float4 Get_Ambient() { return m_vAmbient; }
	_float4 Get_Specular() { return m_vSpecular; }

public :
	void Set_Diffuse(_fvector _vDiffuse) { XMStoreFloat4(&m_vDiffuse, _vDiffuse); }
	void Set_Ambient(_fvector _vAmbient) { XMStoreFloat4(&m_vAmbient, _vAmbient); }
	void Set_Specular(_fvector _vSpecular) { XMStoreFloat4(&m_vSpecular, _vSpecular); }

	void Set_OnOff(_bool _bOnOff) { m_bOnOff = _bOnOff; }

private :
	ELight m_eLight{ ELight::LIGHT_END };

	_float4 m_vDiffuse{};
	_float4 m_vAmbient{};
	_float4 m_vSpecular{};

	_float m_fRange{};

private :
	_bool m_bOnOff{ true };
	_uint m_iPassIndex{};

public :
	static CLightComponent* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END