#pragma once

#include "IPipeLine.h"

#include "CObject.h"

BEGIN(Engine)

class ENGINE_DLL CPipeLine final
	: public IPipeLine
	, public CObject
{
private :
	CPipeLine();
	virtual ~CPipeLine() = default;

public :
	HRESULT Init();
	virtual void Update() override;

public :
	virtual _float4x4 Get_TransformFloat4x4(ED3DTransform _eD3DTransform) const override
	{ return m_matTransforms[static_cast<_uint>(_eD3DTransform)]; }
	virtual _matrix Get_TransformMatrix(ED3DTransform _eD3DTransform) const override
	{ return XMLoadFloat4x4(&m_matTransforms[static_cast<_uint>(_eD3DTransform)]); }
	virtual _float4x4 Get_TransformFloat4x4_Inverse(ED3DTransform _eD3DTransform) const override
	{ return m_matTransformsInverse[static_cast<_uint>(_eD3DTransform)]; }
	virtual _matrix Get_TransformMatrix_Inverse(ED3DTransform _eD3DTransform) const override
	{ return XMLoadFloat4x4(&m_matTransformsInverse[static_cast<_uint>(_eD3DTransform)]); }
	virtual _float4 Get_CamPos() override
	{ return m_vCamPos;}

	virtual void Set_Transform(ED3DTransform _eD3DTransform, _fmatrix matTransform) override
	{ XMStoreFloat4x4(&m_matTransforms[static_cast<_uint>(_eD3DTransform)], matTransform); }

private :
	_float4x4 m_matTransforms[static_cast<_uint>(ED3DTransform::D3DTS_END)]{};
	_float4x4 m_matTransformsInverse[static_cast<_uint>(ED3DTransform::D3DTS_END)]{};

	_float4 m_vCamPos{};

public :
	static CPipeLine* Create();
	virtual void Free() override;
};

END