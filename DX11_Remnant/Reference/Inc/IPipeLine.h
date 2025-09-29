#pragma once

#include "Engine_Define.h"

#include "ED3DTransform.h"

BEGIN(Engine)

class ENGINE_DLL IPipeLine abstract
{
protected :
	virtual ~IPipeLine() = default;

public :
	virtual void Update() PURE;

public :
	virtual _float4x4 Get_TransformFloat4x4(ED3DTransform _eD3DTransform) const PURE;
	virtual _matrix Get_TransformMatrix(ED3DTransform _eD3DTransform) const PURE;
	virtual _float4x4 Get_TransformFloat4x4_Inverse(ED3DTransform _eD3DTransform) const PURE;
	virtual _matrix Get_TransformMatrix_Inverse(ED3DTransform _eD3DTransform) const PURE;
	virtual _float4 Get_CamPos() PURE;

	virtual void Set_Transform(ED3DTransform _eD3DTransform, _fmatrix matTransform) PURE;
};

END