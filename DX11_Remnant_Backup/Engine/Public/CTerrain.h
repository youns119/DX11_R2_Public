#pragma once

#include "CActor.h"

#include "FTerrain.h"

BEGIN(Engine)

class CTerrainComponent;

class ENGINE_DLL CTerrain abstract
	: public CActor
{
protected :
	CTerrain(ID3D11Device * _pDevice, ID3D11DeviceContext * _pContext);
	CTerrain(const CTerrain& _Prototype);
	virtual ~CTerrain() = default;

public :
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;

public :
	_float Get_Height(const _float3* _pPos);

protected :
	CTerrainComponent* m_pTerrainComponent{};

public :
	virtual CClonable* Clone(void* _pDesc) PURE;
	virtual void Free() override;
};

END