#pragma once

#include "Client_Define.h"

#include "IImGuiManager.h"

#include "CObject.h"

BEGIN(Engine)

class CActor;
class CActorComponent;

END

BEGIN(Client)

class CImGuiManager final
	: public IImGuiManager
	, public CObject
{
private :
	CImGuiManager();
	virtual ~CImGuiManager() = default;

public :
	virtual HRESULT Init(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext) override;
	virtual void Pre_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Post_Update(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public :
	virtual void Set_ImGuiMode(EImGuiMode _eImGuiMode, CLevel* _pLevel) override
	{
		m_eImGuiMode = _eImGuiMode;
		m_pCurrLevel = _pLevel;
	}

public :
	void ImGui_Default();

	void ImGui_LevelMode();
	void ImGui_ActorList();
	void ImGui_ActorEditor(CActor* _pActor);
	void ImGui_ComponentEditor_SkeletalMesh(CActorComponent* _pActorComponent);
	void ImGui_ComponentEditor_StaticMesh(CActorComponent* _pActorComponent);
	void ImGui_ComponentEditor_Camera(CActorComponent* _pActorComponent);
	void ImGui_ComponentEditor_SpringArm(CActorComponent* _pActorComponent);
	void ImGui_ComponentEditor_Light(CActorComponent* _pActorComponent);
	void ImGui_ComponentEditor_Collider_AABB(CActorComponent* _pActorComponent);
	void ImGui_ComponentEditor_Collider_OBB(CActorComponent* _pActorComponent);
	void ImGui_ComponentEditor_Collider_Sphere(CActorComponent* _pActorComponent);
	void ImGui_ComponentEditor_Particle(CActorComponent* _pActorComponent);
	void ImGui_ComponentEditor_Movement(CActorComponent* _pActorComponent);
	void ImGui_ComponentEditor_CharacterMovement(CActorComponent* _pActorComponent);

	void ImGui_Picking();

	void ImGui_Terrain();

	virtual void ImGui_PlayerMode() override;

	HRESULT ImGui_File();
	HRESULT ImGui_Save(string _strFile);
	virtual HRESULT ImGui_Load(string _strFile) override;

private :
	EImGuiMode m_eImGuiMode{ EImGuiMode::IMGUI_END };

private :
	CLevel* m_pCurrLevel{};
	CActor* m_pPickingActor{};

	_wstring m_pLayerTag{};

private :
	_bool m_bOnImGui{};

	_tchar m_szSpeed[MAX_PATH]{};
	_float m_fSpeed{};

public :
	static CImGuiManager* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual void Free() override;
};

END