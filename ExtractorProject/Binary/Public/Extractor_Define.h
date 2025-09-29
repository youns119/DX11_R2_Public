#pragma once

#pragma warning (disable : 4251)

#include <iostream>

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "DirectXTK/DDSTextureLoader.h"
#include "DirectXTK/WICTextureLoader.h"
#include "DirectXTK/ScreenGrab.h"
using namespace DirectX;

#include "Assimp\scene.h"
#include "Assimp\Importer.hpp"
#include "Assimp\postprocess.h"
using namespace Assimp;

#include "time.h"

#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <string>
#include <algorithm>
using namespace std;

#include "Extractor_Function.h"
#include "Extractor_Typedef.h"
#include "Extractor_Macro.h"

#include "Func.h"

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG