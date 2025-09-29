#pragma once

#include "CPrimitiveComponent.h"

#include "FUIComponent.h"

BEGIN(Engine)

class CVIBuffer_Rect;
class CTexture;

class ENGINE_DLL CUIComponent final
	: public CPrimitiveComponent
{
private :
	CUIComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CUIComponent(const CUIComponent& _Prototype);
	virtual ~CUIComponent() = default;

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
	CVIBuffer_Rect* m_pVIBuffer{};
	CTexture* m_pTexture{};

private :
	const _char* m_pConstantName{};

	_uint m_iTextureIndex{};

private :
	HRESULT Bind_Texture();
	virtual HRESULT Bind_ShaderResource() override;

public :
	static CUIComponent* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END