#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL IGraphicDevice abstract
{
protected :
	virtual ~IGraphicDevice() = default;

public :
	virtual HRESULT Clear_BackBuffer_View(_float4 _vClearColor) PURE;
	virtual HRESULT Clear_DepthStencil_View() PURE;

	virtual HRESULT Present() PURE;
};

END