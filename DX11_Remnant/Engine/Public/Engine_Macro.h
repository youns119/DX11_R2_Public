#ifndef Engine_Macro_h__
#define Engine_Macro_h__

namespace Engine
{
#define SHADER(path) L##"../../Reference/Shader/"##path

#define RESOURCE(path) L##"../../../Resource/Raw/"##path
#define LEVEL(path) L##"../../../Resource/Binary/Level/"##path
#define FONT(path) L##"../../../Resource/Raw/Font/"##path
#define TEXTURE(path) L##"../../../Resource/Raw/Texture/"##path

#define MODEL_MESH L"../../../Resource/Binary/Model/Mesh/"
#define MODEL_MATERIAL L"../../../Resource/Binary/Model/Material/"
#define MODEL_ANIM L"../../../Resource/Binary/Model/Animation/"
#define MODEL_TEXTURE L"../../../Resource/Raw/Model/Texture/"
}

namespace Engine
{
	// SceneComponent
	static _wstring g_strRootComponent = TEXT("Component_Root");
	static _wstring	g_strSceneComponent = TEXT("Component_Scene");
	static _wstring	g_strCameraComponent = TEXT("Component_Camera");
	static _wstring g_strSpringArmComponent = TEXT("Component_SpringArm");
	static _wstring g_strLightComponent = TEXT("Component_Light");

	// PrimitiveComponent
	static _wstring g_strStaticMeshComponent = TEXT("Component_StaticMesh");
	static _wstring g_strSkeletalMeshComponent = TEXT("Component_SkeletalMesh");
	static _wstring	g_strTerrainComponent = TEXT("Component_Terrain");
	static _wstring g_strColliderComponent_AABB = TEXT("Component_Collider_AABB");
	static _wstring g_strColliderComponent_OBB = TEXT("Component_Collider_OBB");
	static _wstring g_strColliderComponent_Sphere = TEXT("Component_Collider_Sphere");
	static _wstring g_strParticleComponent = TEXT("Component_Particle");
	static _wstring g_strCubeComponent = TEXT("Component_Cube");
	static _wstring g_strUIComponent = TEXT("Component_UI");

	// MovementComponent
	static _wstring	g_strMovementComponent = TEXT("Component_Movement");
	static _wstring	g_strCharacterMovementComponent = TEXT("Component_ChracterMovement");

	// NavigationComponent
	static _wstring g_strNavigationComponent = TEXT("Component_Navigation");
}

namespace Engine
{
#ifndef	MSG_BOX
#define	MSG_BOX(_message)	MessageBox(nullptr, TEXT(_message), L"System Message", MB_OK)
#endif

#define	BEGIN(NAMESPACE)	namespace NAMESPACE {
#define	END					}

#define	USING(NAMESPACE)	using namespace NAMESPACE;

#ifdef	ENGINE_EXPORTS
#define ENGINE_DLL			_declspec(dllexport)
#else
#define ENGINE_DLL			_declspec(dllimport)
#endif
}

#endif // Engine_Macro_h__