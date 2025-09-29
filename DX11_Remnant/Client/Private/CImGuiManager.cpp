#include "pch.h"
#include "CImGuiManager.h"

#include "CGameInstance.h"
#include "CActor.h"
#include "CSkeletalMeshComponent.h"
#include "CStaticMeshComponent.h"
#include "CCameraComponent.h"
#include "CSpringArmComponent.h"
#include "CLightComponent.h"
#include "CColliderComponent.h"
#include "CParticleComponent.h"
#include "CCharacterMovementComponent.h"
#include "CFileUtility.h"

#include "CLevel_Loading.h"

#include "CCharacter.h"

CImGuiManager::CImGuiManager()
{
}

HRESULT CImGuiManager::Init(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	if (!ImGui_ImplWin32_Init(g_hWnd) ||
		!ImGui_ImplDX11_Init(_pDevice, _pContext))
		return E_FAIL;

	return S_OK;
}

void CImGuiManager::Pre_Update(_float _fTimeDelta)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (CGameInstance::Key_Press(DIK_F5))
		ImGui_PlayerMode();
}

void CImGuiManager::Update(_float _fTimeDelta)
{
	m_bOnImGui = false;

	// ImGui
	switch (m_eImGuiMode)
	{
	case EImGuiMode::IMGUI_LEVELMODE :
		ImGui_Default();
		ImGui_LevelMode();
		ImGui_ActorList();
		ImGui_Picking();
		break;
	case EImGuiMode::IMGUI_TERRAIN :
		ImGui_Default();
		ImGui_Terrain();
		break;
	case EImGuiMode::IMGUI_PLAYMODE :
		break;
	default:
		break;
	}
}

void CImGuiManager::Post_Update(_float _fTimeDelta)
{
}

HRESULT CImGuiManager::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	ImGui::EndFrame();

	//CActor* pActor = CGameInstance::Get_Controller();
	//if (pActor)
	//{
	//	CCharacter* pCharacter = static_cast<CCharacter*>(pActor);

	//	_float3 vVelocity = pCharacter->Get_MovementComponent()->Get_Velocity();
	//	_vector vVel = XMLoadFloat3(&vVelocity);
	//	m_fSpeed = XMVectorGetX(XMVector3Length(vVel));
	//	
	//	swprintf(m_szSpeed, sizeof(m_szSpeed) / sizeof(_tchar), TEXT("Player Speed : %f"), m_fSpeed);
	//	CGameInstance::Render_Font(TEXT("Font_Gulim_Default"), m_szSpeed, _float2(0.f, 10.f));

	//	if(m_fSpeed <= 4.f * 0.01f)
	//		CGameInstance::Render_Font(TEXT("Font_Gulim_Default"), TEXT("true"), _float2(0.f, 20.f));
	//	else
	//		CGameInstance::Render_Font(TEXT("Font_Gulim_Default"), TEXT("false"), _float2(0.f, 20.f));
	//}

	return S_OK;
}

void CImGuiManager::ImGui_Default()
{
	ImGui::Begin("Default");

	m_bOnImGui |= ImGui::IsWindowHovered();

	ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

	const _char* pLevelItems[] =
	{
		"Level_Stage1",
		"Level_Terrain"
	};

	ELevel eLevel{};
	static _int iCurrLevel{};

	ImGui::Text("Level Selection");
	ImGui::Combo("Level", &iCurrLevel, pLevelItems, IM_ARRAYSIZE(pLevelItems));
	ImGui::SameLine();

	if (ImGui::Button("Load"))
	{
		switch (iCurrLevel)
		{
		case 0 :
			eLevel = ELevel::LEVEL_STAGE1;
			break;
		case 1 :
			eLevel = ELevel::LEVEL_TERRAIN;
			break;
		default :
			break;
		}

		WORLD->Load_MainLevel(CLevel_Loading::Create
		(
			m_pCurrLevel->Get_Device(),
			m_pCurrLevel->Get_Context(),
			eLevel
		));
	}

	ImGui::End();
}

void CImGuiManager::ImGui_LevelMode()
{
	ImGui::Begin("LevelMode");

#pragma region FILEBROWSER

	ImGui_File();

#pragma endregion

#pragma region LAYER

	vector<_wstring> vecLayerTag = *m_pCurrLevel->Get_LayerTags();
	vector<string> vecLayerString;
	vector<const _char*> vecLayer;
	static _int iCurrLayer{};

	for (auto& LayerTag : vecLayerTag)
	{
		string strLayer = CFileUtility::ConvertToUTF8(LayerTag);
		vecLayerString.push_back(strLayer);
	}

	for (auto& LayerString : vecLayerString)
		vecLayer.push_back(LayerString.c_str());

	ImGui::Text("Layer Selection");
	ImGui::Combo("Layer", &iCurrLayer, vecLayer.data(), static_cast<_int>(vecLayer.size()));

	m_pLayerTag = vecLayerTag[iCurrLayer];

#pragma endregion

	m_bOnImGui |= ImGui::IsWindowHovered();

	static _uint iSelectedActor{};
	_uint iNum{};

	static CActor* pLevelActor{};

	const list<CActor*>* listActor = m_pCurrLevel->Get_ActorList(TEXT("Layer_Picking"));
	if (listActor == nullptr)
	{
		ImGui::End();
		return;
	}

	for (auto& Actor : *listActor)
	{
		_wstring strActorName = Actor->Get_Name();
		string strName = CFileUtility::ConvertToUTF8(strActorName);
		const _char* szActorName = strName.c_str();

		if (ImGui::RadioButton(szActorName, iSelectedActor == iNum++))
		{
			iSelectedActor = iNum - 1;

			if(m_pPickingActor != nullptr)
				m_pPickingActor->Set_Show(false);

			pLevelActor = m_pPickingActor = Actor;
		}

		if (iSelectedActor == iNum - 1 && pLevelActor != nullptr)
		{
			m_pPickingActor->Set_Show(true);

			ImGui_ActorEditor(pLevelActor);
		}
	}

	if (ImGui::RadioButton("None", iSelectedActor == iNum++))
	{
		iSelectedActor = iNum - 1;

		if (m_pPickingActor != nullptr)
			m_pPickingActor->Set_Show(false);

		pLevelActor = m_pPickingActor = nullptr;
	}

	ImGui::End();
}

void CImGuiManager::ImGui_ActorList()
{
	ImGui::Begin("ActorList");

	m_bOnImGui |= ImGui::IsWindowHovered();

#pragma region LAYER

	_wstring szLayerTag{};
	static CActor* pListActor{};

	{
		vector<_wstring> vecLayerTag = *m_pCurrLevel->Get_LayerTags();
		vector<string> vecLayerString;
		vector<const _char*> vecLayer;
		static _int iCurrActorLayer{};

		for (auto& LayerTag : vecLayerTag)
		{
			string strLayer = CFileUtility::ConvertToUTF8(LayerTag);
			vecLayerString.push_back(strLayer);
		}

		for (auto& LayerString : vecLayerString)
			vecLayer.push_back(LayerString.c_str());

		ImGui::Text("Layer Selection");
		if (ImGui::Combo("Layer", &iCurrActorLayer, vecLayer.data(), static_cast<_int>(vecLayer.size())))
			pListActor = nullptr;

		szLayerTag = vecLayerTag[iCurrActorLayer];
	}

#pragma endregion

	const list<CActor*>* listActor = m_pCurrLevel->Get_ActorList(szLayerTag);
	vector<string> vecActorString;
	vector<const _char*> vecActorName;
	vector<CActor*> vecActor;

	if (listActor == nullptr)
	{
		ImGui::End();
		return;
	}

	for (auto& Actor : *listActor)
	{
		_wstring strActorName = Actor->Get_Name();
		string strActor = CFileUtility::ConvertToUTF8(strActorName);

		vecActorString.push_back(strActor);
		vecActor.push_back(Actor);
	}

	for (auto& ActorString : vecActorString)
		vecActorName.push_back(ActorString.c_str());

	static _int iCurrActor = 0;

	ImGui::Text("Actor Selection");
	if (ImGui::ListBox("ActorList", &iCurrActor, vecActorName.data(), static_cast<_int>(vecActorName.size())))
		pListActor = vecActor[iCurrActor];

	if (pListActor != nullptr)
	{
		ImGui_ActorEditor(pListActor);

		if (ImGui::Button("Delete"))
		{
			m_pCurrLevel->DeSpawn_Actor(szLayerTag, pListActor);
			pListActor = nullptr;
		}
	}

	ImGui::End();
}

void CImGuiManager::ImGui_ActorEditor(CActor* _pActor)
{
	list<CActorComponent*>* listActorComponent = _pActor->Get_UpdateGraph()->Get_ComponentList();

	for (auto& CActorComponent : *listActorComponent)
	{
		_wstring strActorComponent = CActorComponent->Get_ComponentName();
		string strName = CFileUtility::ConvertToUTF8(strActorComponent);
		const _char* szName = strName.c_str();

		ImGui::Text(szName);

		if (strActorComponent == g_strSkeletalMeshComponent)
			ImGui_ComponentEditor_SkeletalMesh(CActorComponent);
		else if (strActorComponent == g_strStaticMeshComponent)
			ImGui_ComponentEditor_StaticMesh(CActorComponent);
		else if (strActorComponent == g_strCameraComponent)
			ImGui_ComponentEditor_Camera(CActorComponent);
		else if (strActorComponent == g_strSpringArmComponent)
			ImGui_ComponentEditor_SpringArm(CActorComponent);
		else if (strActorComponent == g_strLightComponent)
			ImGui_ComponentEditor_Light(CActorComponent);
		else if (strActorComponent == g_strColliderComponent_AABB)
			ImGui_ComponentEditor_Collider_AABB(CActorComponent);
		else if (strActorComponent == g_strColliderComponent_OBB)
			ImGui_ComponentEditor_Collider_OBB(CActorComponent);
		else if (strActorComponent == g_strColliderComponent_Sphere)
			ImGui_ComponentEditor_Collider_Sphere(CActorComponent);
		else if (strActorComponent == g_strParticleComponent)
			ImGui_ComponentEditor_Particle(CActorComponent);
		else if (strActorComponent == g_strMovementComponent)
			ImGui_ComponentEditor_Movement(CActorComponent);
		else if (strActorComponent == g_strCharacterMovementComponent)
			ImGui_ComponentEditor_CharacterMovement(CActorComponent);
	}
}

void CImGuiManager::ImGui_ComponentEditor_SkeletalMesh(CActorComponent* _pActorComponent)
{
	CSkeletalMeshComponent* pComponent = static_cast<CSkeletalMeshComponent*>(_pActorComponent);

	_float3 vScale = pComponent->Get_Transform(ETransform::TRANSFORM_SCALE);
	_float3 vRotation = pComponent->Get_Transform(ETransform::TRANSFORM_ROTATION);
	_float3 vPosition = pComponent->Get_Transform(ETransform::TRANSFORM_POSITION);

	_float fScale[3] = { vScale.x, vScale.y, vScale.z };
	_float fRotation[3] = { vRotation.x, vRotation.y, vRotation.z };
	_float fPosition[3] = { vPosition.x, vPosition.y, vPosition.z };

	if (ImGui::DragFloat3("Scale", fScale, 0.1f)) 
		vScale = _float3(fScale[0], fScale[1], fScale[2]);
	if (ImGui::DragFloat3("Rotation", fRotation, 0.1f, -180.f, 180.f)) 
		vRotation = _float3(fRotation[0], fRotation[1], fRotation[2]);
	if (ImGui::DragFloat3("Position", fPosition, 0.1f))
		vPosition = _float3(fPosition[0], fPosition[1], fPosition[2]);

	pComponent->Update_Transform
	(
		XMLoadFloat3(&vScale),
		XMLoadFloat3(&vRotation),
		XMLoadFloat3(&vPosition)
	);
}

void CImGuiManager::ImGui_ComponentEditor_StaticMesh(CActorComponent* _pActorComponent)
{
	CStaticMeshComponent* pComponent = static_cast<CStaticMeshComponent*>(_pActorComponent);

	_float3 vScale = pComponent->Get_Transform(ETransform::TRANSFORM_SCALE);
	_float3 vRotation = pComponent->Get_Transform(ETransform::TRANSFORM_ROTATION);
	_float3 vPosition = pComponent->Get_Transform(ETransform::TRANSFORM_POSITION);

	_float fScale[3] = { vScale.x, vScale.y, vScale.z };
	_float fRotation[3] = { vRotation.x, vRotation.y, vRotation.z };
	_float fPosition[3] = { vPosition.x, vPosition.y, vPosition.z };

	if (ImGui::DragFloat3("Scale", fScale, 0.1f))
		vScale = _float3(fScale[0], fScale[1], fScale[2]);
	if (ImGui::DragFloat3("Rotation", fRotation, 0.1f, -180.f, 180.f))
		vRotation = _float3(fRotation[0], fRotation[1], fRotation[2]);
	if (ImGui::DragFloat3("Position", fPosition, 0.1f))
		vPosition = _float3(fPosition[0], fPosition[1], fPosition[2]);

	pComponent->Update_Transform
	(
		XMLoadFloat3(&vScale),
		XMLoadFloat3(&vRotation),
		XMLoadFloat3(&vPosition)
	);
}

void CImGuiManager::ImGui_ComponentEditor_Camera(CActorComponent* _pActorComponent)
{

}

void CImGuiManager::ImGui_ComponentEditor_SpringArm(CActorComponent* _pActorComponent)
{
}

void CImGuiManager::ImGui_ComponentEditor_Light(CActorComponent* _pActorComponent)
{
	CLightComponent* pComponent = static_cast<CLightComponent*>(_pActorComponent);

	_float3 vScale = pComponent->Get_Transform(ETransform::TRANSFORM_SCALE);
	_float3 vRotation = pComponent->Get_Transform(ETransform::TRANSFORM_ROTATION);
	_float3 vPosition = pComponent->Get_Transform(ETransform::TRANSFORM_POSITION);

	_float fScale[3] = { vScale.x, vScale.y, vScale.z };
	_float fRotation[3] = { vRotation.x, vRotation.y, vRotation.z };
	_float fPosition[3] = { vPosition.x, vPosition.y, vPosition.z };

	if (ImGui::DragFloat3("Scale", fScale, 0.1f))
		vScale = _float3(fScale[0], fScale[1], fScale[2]);
	if (ImGui::DragFloat3("Rotation", fRotation, 0.1f, -180.f, 180.f))
		vRotation = _float3(fRotation[0], fRotation[1], fRotation[2]);
	if (ImGui::DragFloat3("Position", fPosition, 0.1f))
		vPosition = _float3(fPosition[0], fPosition[1], fPosition[2]);

	pComponent->Update_Transform
	(
		XMLoadFloat3(&vScale),
		XMLoadFloat3(&vRotation),
		XMLoadFloat3(&vPosition)
	);

	_float4 vDiffuse = pComponent->Get_Diffuse();
	_float4 vAmbient = pComponent->Get_Ambient();
	_float4 vSpecular = pComponent->Get_Specular();

	_float fDiffuse[4] = { vDiffuse.x, vDiffuse.y, vDiffuse.z, vDiffuse.w };
	_float fAmbient[4] = { vAmbient.x, vAmbient.y, vAmbient.z, vAmbient.w };
	_float fSpecular[4] = { vSpecular.x, vSpecular.y, vSpecular.z, vSpecular.w };

	if (ImGui::DragFloat4("Diffuse", fDiffuse, 0.1f, 0.f, 1.f))
		vDiffuse = _float4(fDiffuse[0], fDiffuse[1], fDiffuse[2], fDiffuse[3]);
	if (ImGui::DragFloat4("Ambient", fAmbient, 0.1f, -180.f, 180.f))
		vAmbient = _float4(fAmbient[0], fAmbient[1], fAmbient[2], fAmbient[3]);
	if (ImGui::DragFloat4("Specular", fSpecular, 0.1f, 0.f, 1.f))
		vSpecular = _float4(fSpecular[0], fSpecular[1], fSpecular[2], fSpecular[3]);

	pComponent->Set_Diffuse(XMLoadFloat4(&vDiffuse));
	pComponent->Set_Ambient(XMLoadFloat4(&vAmbient));
	pComponent->Set_Specular(XMLoadFloat4(&vSpecular));
}

void CImGuiManager::ImGui_ComponentEditor_Collider_AABB(CActorComponent* _pActorComponent)
{
}

void CImGuiManager::ImGui_ComponentEditor_Collider_OBB(CActorComponent* _pActorComponent)
{
}

void CImGuiManager::ImGui_ComponentEditor_Collider_Sphere(CActorComponent* _pActorComponent)
{
}

void CImGuiManager::ImGui_ComponentEditor_Particle(CActorComponent* _pActorComponent)
{
}

void CImGuiManager::ImGui_ComponentEditor_Movement(CActorComponent* _pActorComponent)
{
}

void CImGuiManager::ImGui_ComponentEditor_CharacterMovement(CActorComponent* _pActorComponent)
{
}

void CImGuiManager::ImGui_Picking()
{
	if (m_pPickingActor == nullptr)
		return;

	CSceneComponent* pSceneComponent = m_pPickingActor->Get_RootComponent();

	//_float3	vPos;
	_float3 vSrcScale{};
	_float3 vSrcRotation{};
	_float3 vSrcPosition{};

	if (CGameInstance::Compute_PickPos(&vSrcPosition))
		pSceneComponent->Update_Position(XMLoadFloat3(&vSrcPosition));

	vSrcScale = pSceneComponent->Get_Transform(ETransform::TRANSFORM_SCALE);
	vSrcRotation = pSceneComponent->Get_Transform(ETransform::TRANSFORM_ROTATION);
	vSrcPosition = pSceneComponent->Get_Transform(ETransform::TRANSFORM_POSITION);

	if (CGameInstance::Mouse_Press(EMouseKey::MOUSEKEY_RB) && !m_bOnImGui)
	{
		FActor tActor{};
		tActor.pOuter = m_pCurrLevel;

		CActor* pActor = m_pCurrLevel->Spawn_Actor
		(
			m_pCurrLevel->Get_LevelIndex(),
			m_pPickingActor->Get_Name(),
			m_pLayerTag,
			&tActor
		);
		if (pActor == nullptr)
			return;

		//_float3 vSrcScale = pSceneComponent->Get_Transform(ETransform::TRANSFORM_SCALE);
		//_float3 vSrcRotation = pSceneComponent->Get_Transform(ETransform::TRANSFORM_ROTATION);
		//_float3 vSrcPosition = pSceneComponent->Get_Transform(ETransform::TRANSFORM_POSITION);

		_vector vScale = XMLoadFloat3(&vSrcScale);
		//_vector vScale = XMVectorSet(1.f, 1.f, 1.f, 0.f);
		_vector vRotation = XMLoadFloat3(&vSrcRotation);
		_vector vPosition = XMLoadFloat3(&vSrcPosition);
		pActor->Get_RootComponent()->Update_Transform(vScale, vRotation, vPosition);
	}
}

void CImGuiManager::ImGui_Terrain()
{
	static float f = 0.0f;
	static int counter = 0;
	static _bool show_demo_window, show_another_window;
	static _float clear_color = 0.f;

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	ImGui::Checkbox("Another Window", &show_another_window);

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();

	//_float3 vPickingPos = TOOL->Get_PickingPos();
	//ImGui::Text("X = %.2f Y = %.2f, Z = %.2f", vPickingPos.x, vPickingPos.y, vPickingPos.z);
	//ImGui::Text("counter = %d", counter);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::ShowDemoWindow();
}

void CImGuiManager::ImGui_PlayerMode()
{
	CActor* pActor{};

	if (m_eImGuiMode == EImGuiMode::IMGUI_LEVELMODE)
	{
		m_eImGuiMode = EImGuiMode::IMGUI_PLAYMODE;

		pActor = m_pCurrLevel->Get_Actor(L"Layer_Player", L"Actor_Player");
		if (pActor == nullptr)
			return;
	}
	else if (m_eImGuiMode == EImGuiMode::IMGUI_PLAYMODE)
	{
		m_eImGuiMode = EImGuiMode::IMGUI_LEVELMODE;

		pActor = m_pCurrLevel->Get_Actor(L"Layer_Camera", L"Actor_Camera_Free");
		if (pActor == nullptr)
			return;
	}

	CGameInstance::Set_Controller(pActor);
}

HRESULT CImGuiManager::ImGui_File()
{
	if (ImGui::Button("Save"))
	{
		IGFD::FileDialogConfig config;
		config.path = "../../../Resource/Binary/";

		ImGuiFileDialog::Instance()->OpenDialog("Save File", "Choose File", ".data", config);
	}

	if (ImGuiFileDialog::Instance()->Display("Save File"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();

			if (FAILED(ImGui_Save(filePathName)))
				return E_FAIL;
		}

		ImGuiFileDialog::Instance()->Close();
	}

	ImGui::SameLine();

	if (ImGui::Button("Load"))
	{
		IGFD::FileDialogConfig config;
		config.path = "../../../Resource/Binary/";

		ImGuiFileDialog::Instance()->OpenDialog("Load File", "Choose File", ".data", config);
	}

	if (ImGuiFileDialog::Instance()->Display("Load File"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();

			if (FAILED(ImGui_Load(filePathName)))
				return E_FAIL;
		}

		ImGuiFileDialog::Instance()->Close();
	}

	return S_OK;
}

HRESULT CImGuiManager::ImGui_Save(string _strFile)
{
	_wstring strFile = CFileUtility::ConvertFromUTF8(_strFile);
	const _tchar* szFile = strFile.c_str();

	_ulong dwByte{};
	HANDLE hFile = CreateFile(szFile, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == 0)
		return E_FAIL;

	vector<_wstring> vecLayerTag = *m_pCurrLevel->Get_LayerTags();

	_uint iSize = static_cast<_uint>(vecLayerTag.size()) - 1;
	WriteFile(hFile, &iSize, sizeof(_uint), &dwByte, nullptr);

	for (auto& LayerTag : vecLayerTag)
	{
		if (LayerTag == L"Layer_Picking")
			continue;

		_tchar szLayerTag[MAX_PATH]{};
		lstrcpy(szLayerTag, LayerTag.c_str());

		WriteFile(hFile, &szLayerTag, sizeof(_tchar) * MAX_PATH, &dwByte, nullptr);
	}

	for (auto& LayerTag : vecLayerTag)
	{
		if (m_pCurrLevel->Find_Layer(LayerTag) == nullptr || LayerTag == L"Layer_Picking")
			continue;

		const list<CActor*> listActor = *m_pCurrLevel->Get_ActorList(LayerTag);

		for (auto& Actor : listActor)
		{
			_wstring strName = Actor->Get_Name();
			_float3 vScale = Actor->Get_RootComponent()->Get_Transform(ETransform::TRANSFORM_SCALE);
			_float3 vRotation = Actor->Get_RootComponent()->Get_Transform(ETransform::TRANSFORM_ROTATION);
			_float3 vPosition = Actor->Get_RootComponent()->Get_Transform(ETransform::TRANSFORM_POSITION);

			_tchar szLayerTag[MAX_PATH]{};
			_tchar szActorName[MAX_PATH]{};
			lstrcpy(szLayerTag, LayerTag.c_str());
			lstrcpy(szActorName, strName.c_str());
			
			WriteFile(hFile, &szLayerTag, sizeof(_tchar) * MAX_PATH, &dwByte, nullptr);
			WriteFile(hFile, &szActorName, sizeof(_tchar) * MAX_PATH, &dwByte, nullptr);
			WriteFile(hFile, &vScale, sizeof(_float3), &dwByte, nullptr);
			WriteFile(hFile, &vRotation, sizeof(_float3), &dwByte, nullptr);
			WriteFile(hFile, &vPosition, sizeof(_float3), &dwByte, nullptr);
		}
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CImGuiManager::ImGui_Load(string _strFile)
{
	_wstring strFile = CFileUtility::ConvertFromUTF8(_strFile);
	const _tchar* szFile = strFile.c_str();

	_ulong dwByte{};
	HANDLE hFile = CreateFile(szFile, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == 0)
		return E_FAIL;

	m_pCurrLevel->Clear();

	_uint iSize{};
	vector<_wstring>* vecLayerTag = m_pCurrLevel->Get_LayerTags();

	if (!ReadFile(hFile, &iSize, sizeof(_uint), &dwByte, nullptr))
		return E_FAIL;

	for (_uint i = 0; i < iSize; i++)
	{
		_tchar szLayerTag[MAX_PATH]{};

		if (!ReadFile(hFile, &szLayerTag, sizeof(_tchar) * MAX_PATH, &dwByte, nullptr))
			return E_FAIL;

		_wstring strLayerTag = _wstring(szLayerTag);
		vecLayerTag->push_back(strLayerTag);
	}

	while (true)
	{
		_tchar szLayerTag[MAX_PATH]{};
		_tchar szActorName[MAX_PATH]{};
		_float3 vSrcScale{}, vSrcRotation{}, vSrcPosition{};
	
		if (!ReadFile(hFile, &szLayerTag, sizeof(_tchar) * MAX_PATH, &dwByte, nullptr)	||
			!ReadFile(hFile, &szActorName, sizeof(_tchar) * MAX_PATH, &dwByte, nullptr) ||
			!ReadFile(hFile, &vSrcScale, sizeof(_float3), &dwByte, nullptr)				||
			!ReadFile(hFile, &vSrcRotation, sizeof(_float3), &dwByte, nullptr)			||
			!ReadFile(hFile, &vSrcPosition, sizeof(_float3), &dwByte, nullptr))
			return E_FAIL;
	
		if (dwByte == 0)
		{
			CloseHandle(hFile);
			break;
		}
	
		_vector vScale = XMLoadFloat3(&vSrcScale);
		_vector vRotation = XMLoadFloat3(&vSrcRotation);
		_vector vPosition = XMLoadFloat3(&vSrcPosition);
	
		FActor tActor{};
		tActor.pOuter = m_pCurrLevel;
	
		CActor* pActor{};

		pActor = m_pCurrLevel->Spawn_Actor
		(
			m_pCurrLevel->Get_LevelIndex(),
			szActorName,
			szLayerTag,
			&tActor
		);
		if (pActor == nullptr)
			return E_FAIL;

		if (!lstrcmp(szActorName, L"Actor_Sky_Stage1"))
			vScale = vScale * 5.f;

		pActor->Get_RootComponent()->Update_Transform(vScale, vRotation, vPosition);

		if (lstrcmp(szLayerTag, L"Layer_Picking") == false)
			pActor->Set_Show(false);
	}

	CActor* pActor = m_pCurrLevel->Get_Actor(L"Layer_Camera", L"Actor_Camera_Free");
	if (pActor == nullptr)
		return E_FAIL;

	CGameInstance::Set_Controller(pActor);

	return S_OK;
}

CImGuiManager* CImGuiManager::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CImGuiManager* pInstance = new CImGuiManager();

	if (FAILED(pInstance->Init(_pDevice, _pContext)))
	{
		MSG_BOX("Failed To Create : CImGuiManager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGuiManager::Free()
{
	__super::Free();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}