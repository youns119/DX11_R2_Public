#pragma once

#include "CVIBuffer.h"

#include "FVertex_NorTex.h"

BEGIN(Engine)

class CSceneComponent;

class ENGINE_DLL CVIBuffer_Terrain final
	: public CVIBuffer
{
private :
	CVIBuffer_Terrain(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CVIBuffer_Terrain(const CVIBuffer_Terrain& _Prototype);
	virtual ~CVIBuffer_Terrain() = default;

public :
	virtual HRESULT Init(const _tchar* pHeightMapFile);
	virtual HRESULT Init(void* _pDesc) override;

public :
	_uint2 Get_VerticeNum() { return _uint2(m_iNumVerticeX, m_iNumVerticeZ); }
	const _float3* Get_VerticePos() { return m_pVerticePos; }

public :
	_bool Picking_Terrain
	(
		CSceneComponent* _pSceneComponent, 
		_float3 _vRayDir, 
		_float3 _vRayPos, 
		_float3& _pOut
	);

private :
	_uint m_iNumVerticeX{}, m_iNumVerticeZ{};

	_float3* m_pVerticePos{};

public :
	static CVIBuffer_Terrain* Create
	(
		ID3D11Device* _pDevice, 
		ID3D11DeviceContext* _pContext, 
		const _tchar* pHeightMapFile
	);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END