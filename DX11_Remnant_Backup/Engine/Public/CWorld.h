#pragma once

#include "IWorld.h"

#include "CObject.h"

BEGIN(Engine)

class CLevel;

class ENGINE_DLL CWorld final
	: public IWorld
	, public CObject
{
private :
	CWorld(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CWorld() = default;

public :
	virtual HRESULT Init(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext) override;
	virtual void Pre_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Post_Update(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public :
	virtual ID3D11Device* Get_Device() const { return m_pDevice; }
	virtual ID3D11DeviceContext* Get_Context() const { return m_pContext; }
	virtual CLevel* Get_MainLevel() const override { return m_pMainLevel; }

public :
	virtual HRESULT Load_MainLevel(CLevel* _pMainLevel) override;
	virtual HRESULT Load_SubLevel(const _wstring& _strSubLevelTag, CLevel* _pLevel) override;
	virtual HRESULT UnLoad_MainLevel() override;
	virtual HRESULT UnLoad_SubLevel(const _wstring& _strSubLevelTag) override;

private :
	ID3D11Device* m_pDevice{};
	ID3D11DeviceContext* m_pContext{};

private :
	CLevel* m_pMainLevel{};
	map<const _wstring, CLevel*> m_mapSubLevel;
	
private :
	CLevel* Find_SubLevel(const _wstring& _strSubLevelTag);

public :
	static CWorld* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual void Free() override;
};

END