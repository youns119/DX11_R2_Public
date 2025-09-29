#pragma once

#include "CPrimitiveComponent.h"

#include "FParticleComponent.h"

BEGIN(Engine)

class CVIBuffer_Instancing;
class CTexture;

class ENGINE_DLL CParticleComponent final
	: public CPrimitiveComponent
{
private :
	CParticleComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CParticleComponent(const CParticleComponent& _Prototype);
	virtual ~CParticleComponent() = default;

public:
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;
	virtual HRESULT Render() override;

public:
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

public:
	HRESULT Bind_Texture
	(
		_uint _iTextureType,
		const _char* _pConstantName,
		_uint _iTextureIndex
	);

private :
	CVIBuffer_Instancing* m_pVIBuffer{};

	CTexture* m_pTexture{};

public :
	static CParticleComponent* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END