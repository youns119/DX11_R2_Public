#pragma once

#include "CPrimitiveComponent.h"

#include "FTerrainComponent.h"

BEGIN(Engine)

class CVIBuffer_Terrain;
class CTexture;

class ENGINE_DLL CTerrainComponent final
	: public CPrimitiveComponent
{
private :
	CTerrainComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CTerrainComponent(const CTerrainComponent& _Prototype);
	virtual ~CTerrainComponent() = default;

public :
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;
	virtual HRESULT Render() override;

public :
	_uint2 Get_VerticeNum();
	const _float3* Get_VerticePos();

public :
	HRESULT Add_VIBuffer
	(
		_uint _iPrototypeLevelIndex,
		const _wstring& _strPrototypeTag,
		void* _pDesc = nullptr
	);
	HRESULT Add_Texture
	(
		_uint _iPrototypeLevelIndex,
		_uint _iTextureIndex,
		const _wstring& _strPrototypeTag,
		void* _pDesc = nullptr
	);

public :
	HRESULT Bind_Texture
	(
		_uint _iTextureType, 
		const _char* _pConstantName, 
		_uint _iTextureIndex
	);
	HRESULT Bind_Textures
	(
		_uint _iTextureType, 
		const _char* _pConstantName
	);

private :
	CVIBuffer_Terrain* m_pVIBuffer{};

	vector<CTexture*> m_vecTexture;

private :
	_uint m_iNumTexture{};

public :
	static CTerrainComponent* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END