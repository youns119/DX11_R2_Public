#pragma once

#include "Engine_Define.h"

#include "EImGuiManager.h"

BEGIN(Engine)

class CLevel;

END

BEGIN(Client)

class IImGuiManager abstract
{
protected :
	virtual ~IImGuiManager() = default;

public :
	virtual HRESULT Init(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext) PURE;
	virtual void Pre_Update(_float _fTimeDelta) PURE;
	virtual void Update(_float _fTimeDelta) PURE;
	virtual void Post_Update(_float _fTimeDelta) PURE;
	virtual HRESULT Render() PURE;

public :
	virtual void Set_ImGuiMode(EImGuiMode _eImGuiMode, CLevel* _pLevel) PURE;

public :
	virtual void ImGui_PlayerMode() PURE;
	virtual HRESULT ImGui_Load(string _strFile) PURE;
};

END