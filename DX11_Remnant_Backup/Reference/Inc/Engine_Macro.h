#ifndef Engine_Macro_h__
#define Engine_Macro_h__

namespace Engine
{
#define MESH L"../../../Resource/Binary/Model/Mesh/"
#define MATERIAL L"../../../Resource/Binary/Model/Material/"
#define ANIM L"../../../Resource/Binary/Model/Animation/"
#define MESHTEXTURE L"../../../Resource/Raw/Texture/Model/"
#define SHADER(path) L##"../../Reference/Shader/"##path

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