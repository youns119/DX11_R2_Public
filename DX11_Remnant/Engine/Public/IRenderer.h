#pragma once

#include "Engine_Define.h"

#include "ERender.h"

BEGIN(Engine)

class CPrimitiveComponent;
class CLightComponent;

class ENGINE_DLL IRenderer abstract
{
protected :
	virtual ~IRenderer() = default;

public :
	virtual HRESULT Render() PURE;

public :
	virtual HRESULT Add_RenderGroup(ERender _eRender, CPrimitiveComponent* _pComponent) PURE;
	virtual HRESULT Add_Light(CLightComponent* _pComponent) PURE;

#ifdef _DEBUG

public :
	virtual HRESULT Add_DebugComponent(CPrimitiveComponent* _pComponent) PURE;

#endif
};

END