#include "CFileUtility.h"

void CFileUtility::TraverseDirectory(const _wstring& _strDirectory, vector<_wstring>& _vecFilePath)
{
	WIN32_FIND_DATA tFindData{};
	HANDLE hFind = FindFirstFile((_strDirectory + L"/*").c_str(), &tFindData);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		wcerr << "Failed To Open Directory : " << _strDirectory << endl;
		return;
	}

	do
	{
		const _wstring strItemName = tFindData.cFileName;

		if (strItemName == L"." || strItemName == L"..")
			continue;

		_wstring strFullPath = _strDirectory + L"/" + strItemName;

		if (tFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			TraverseDirectory(strFullPath, _vecFilePath);
		else
			_vecFilePath.push_back(strFullPath);
	} while (FindNextFile(hFind, &tFindData) != 0);

	FindClose(hFind);
}

string CFileUtility::ConvertToUTF8(_wstring& _strSrc)
{
	_int iSize = WideCharToMultiByte(CP_UTF8, 0, _strSrc.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (iSize == 0) 
	{
		wcerr << "Failed To ConvertToUTF8" << std::endl;
		return "";
	}

	string strDst(iSize, 0);
	WideCharToMultiByte(CP_UTF8, 0, _strSrc.c_str(), -1, &strDst[0], iSize, nullptr, nullptr);

	return strDst;
}

_wstring CFileUtility::ConvertFromUTF8(string& _strSrc)
{
	_int iSize = MultiByteToWideChar(CP_UTF8, 0, _strSrc.c_str(), -1, nullptr, 0);
	if (iSize == 0) 
	{
		wcerr << "Failed To ConvertFromUTF8" << std::endl;
		return L"";
	}

	wstring strDst(iSize, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, _strSrc.c_str(), -1, &strDst[0], iSize);

	return strDst;
}