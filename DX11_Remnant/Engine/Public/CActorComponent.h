#pragma once

#include "CClonable.h"

#include "FActorComponent.h"

BEGIN(Engine)

class CActor;

class ENGINE_DLL CActorComponent abstract
	: public CClonable
{
protected :
	CActorComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CActorComponent(const CActorComponent& _Prototype);
	virtual ~CActorComponent() = default;

public :
	virtual HRESULT Init();
	virtual HRESULT Init(void* _pDesc);
	virtual void Pre_Update(_float _fTimeDelta);
	virtual void Update(_float _fTimeDelta);
	virtual void Post_Update(_float _fTimeDelta);
	virtual HRESULT Render();

public :
	CActor* Get_Owner() const { return m_pOwner; }

	_wstring Get_ComponentName() const { return m_strName; }
	const _uint Get_ComponentID() const { return m_iComponentID; }

public :
	virtual void Clear();

protected :
	ID3D11Device* m_pDevice{};
	ID3D11DeviceContext* m_pContext{};

protected :
	CActor* m_pOwner{};

protected :
	_wstring m_strName{};

	_bool m_bCloned{};
	_uint m_iComponentID{};

private :
	static _uint m_iStaticID;

public :
	virtual CClonable* Clone(void* _pDesc) PURE;
	virtual void Free() override;
};

END