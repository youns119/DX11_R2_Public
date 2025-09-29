#pragma once

#pragma warning (disable : 4251)

#include <iostream>
#include <io.h>

#include <time.h>

#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <map>
#include <unordered_map>
#include <string>
#include <algorithm>
using namespace std;

#pragma region DIRECTX

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

#pragma endregion

//#include "Assimp\scene.h"
//#include "Assimp\Importer.hpp"
//#include "Assimp\postprocess.h"
//using namespace Assimp;

#pragma region FMOD

#include "FMOD/fmod.h"
#include "FMOD/fmod.hpp"
#pragma comment(lib, "fmod_vc.lib")

#pragma endregion

#include "Engine_Function.h"
#include "Engine_Macro.h"
#include "Engine_Typedef.h"
using namespace Engine;

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG