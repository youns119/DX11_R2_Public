#pragma once

#include "CPrimitiveComponent.h"

#include "FCubeComponent.h"

BEGIN(Engine)

class CVIBuffer_Cube;
class CTexture;

class ENGINE_DLL CCubeComponent final
	: public CPrimitiveComponent
{
private :
	CCubeComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CCubeComponent(const CCubeComponent& _Prototype);
	virtual ~CCubeComponent() = default;

public :
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;
	virtual HRESULT Render() override;

public :
	HRESULT Add_Texture
	(
		_uint _iPrototypeLevelIndex,
		const _wstring& _strPrototypeTag,
		void* _pDesc = nullptr
	);

public :
	HRESULT Bind_Material
	(
		const _char* _pConstantName,
		_uint _iTextureIndex
	);

private :
	CVIBuffer_Cube* m_pVIBuffer{};
	CTexture* m_pTexture{};

private :
	const _char* m_pConstantName{};

	_uint m_iTextureIndex{};

private :
	HRESULT Bind_Texture();

public :
	static CCubeComponent* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END