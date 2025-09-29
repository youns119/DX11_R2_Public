#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class CLevel;

class ENGINE_DLL IWorld abstract
{
protected :
	virtual ~IWorld() = default;

public :
	virtual HRESULT Init(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext) PURE;
	virtual void Pre_Update(_float _fTimeDelta) PURE;
	virtual void Update(_float _fTimeDelta) PURE;
	virtual void Post_Update(_float _fTimeDelta) PURE;
	virtual HRESULT Render() PURE;

public :
	virtual ID3D11Device* Get_Device() const PURE;
	virtual ID3D11DeviceContext* Get_Context() const PURE;
	virtual CLevel* Get_MainLevel() const PURE;

public :
	virtual HRESULT Load_MainLevel(CLevel* _pMainLevel) PURE;
	virtual HRESULT Load_SubLevel(const _wstring& _strSubLevelTag, CLevel* _pLevel) PURE;
	virtual HRESULT UnLoad_MainLevel() PURE;
	virtual HRESULT UnLoad_SubLevel(const _wstring& _strSubLevelTag) PURE;
};

END