#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CFileUtility
{
public :
	static void TraverseDirectory(const _wstring& _strDirectory, vector<_wstring>& _vecFilePath);

	static string ConvertToUTF8(_wstring& _strSrc);
	static _wstring ConvertFromUTF8(string& _strSrc);
};

END