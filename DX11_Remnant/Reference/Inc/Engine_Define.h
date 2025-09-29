#pragma once

#pragma warning (disable : 4251)

#include <iostream>
#include <io.h>

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <d3dcompiler.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "Effect11/d3dx11effect.h"
#include "DirectXTK/DDSTextureLoader.h"
#include "DirectXTK/WICTextureLoader.h"
#include "DirectXTK/ScreenGrab.h"

#include <DirectXTK\PrimitiveBatch.h>
#include <DirectXTK\VertexTypes.h>
#include <DirectXTK\Effects.h>

#include <DirectXTK\SpriteBatch.h>
#include <DirectXTK\SpriteFont.h>
using namespace DirectX;

#include "Assimp\scene.h"
#include "Assimp\Importer.hpp"
#include "Assimp\postprocess.h"
using namespace Assimp;

#include "FMOD/fmod.h"
#include "FMOD/fmod.hpp"
#pragma comment(lib, "fmod_vc.lib")

#include "time.h"

#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <map>
#include <unordered_map>
#include <string>
#include <algorithm>
using namespace std;

#include "Engine_Function.h"
#include "Engine_Macro.h"
#include "Engine_Typedef.h"
using namespace Engine;

namespace Engine
{
	// SceneComponent
	static _wstring g_strRootComponent = TEXT("Component_Root");
	static _wstring	g_strSceneComponent = TEXT("Component_Scene");
	static _wstring g_strStaticMeshComponent = TEXT("Component_StaticMesh");
	static _wstring g_strSkeletalMeshComponent = TEXT("Component_SkeletalMesh");
	static _wstring	g_strTerrainComponent = TEXT("Component_Terrain");
	static _wstring g_strColliderComponent_AABB = TEXT("Component_Collider_AABB");
	static _wstring g_strColliderComponent_OBB = TEXT("Component_Collider_OBB");
	static _wstring g_strColliderComponent_Sphere = TEXT("Component_Collider_Sphere");
	static _wstring g_strParticleComponent = TEXT("Component_Particle");
	static _wstring g_strCubeComponent = TEXT("Component_Cube");
	static _wstring g_strUIComponent = TEXT("Component_UI");
	static _wstring	g_strCameraComponent = TEXT("Component_Camera");
	static _wstring g_strSpringArmComponent = TEXT("Component_SpringArm");
	static _wstring g_strLightComponent = TEXT("Component_Light");

	// MovementComponent
	static _wstring	g_strMovementComponent = TEXT("Component_Movement");
	static _wstring	g_strCharacterMovementComponent = TEXT("Component_ChracterMovement");

	// NavigationComponent
	static _wstring g_strNavigationComponent = TEXT("Component_Navigation");
}

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG