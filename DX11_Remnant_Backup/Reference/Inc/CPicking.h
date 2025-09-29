#pragma once

#include "IPicking.h"

#include "CObject.h"

BEGIN(Engine)

class ENGINE_DLL CPicking final
	: public IPicking
	, public CObject
{
private :
	CPicking(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CPicking() = default;

public :
	HRESULT Init(HWND _hWnd);
	virtual void Update() override;

public :
	virtual _bool Compute_PickPos(_float3* _pOut) override;

private :
	ID3D11Device* m_pDevice{};
	ID3D11DeviceContext* m_pContext{};

private :
	HWND m_hWnd{};

private :
	ID3D11Texture2D* m_pTexture2D{};

private :
	_float m_fViewportWidth{}, m_fViewportHeight{};

public :
	static CPicking* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, HWND _hWnd);
	virtual void Free() override;
};

END