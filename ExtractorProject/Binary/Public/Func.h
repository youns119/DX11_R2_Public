#pragma once

#include "Extractor_Define.h"

static void TraverseDirectory(const string& _strDirectory, vector<string>& _vecFilePath);
static _bool SortFBX(string& _strFilePath);
static _bool CreateDirectories(const string& _strDirectory);
static void ValidateFileName(string& _strFileName);

static void TraverseDirectory(const string& _strDirectory, vector<string>& _vecFilePath)
{
    WIN32_FIND_DATAA tFindData{};
    HANDLE hFind = FindFirstFileA((_strDirectory + "/*").c_str(), &tFindData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        cerr << "Failed to open directory: " << _strDirectory << endl;
        return;
    }

    do
    {
        const string itemName = tFindData.cFileName;

        if (itemName == "." || itemName == "..")
            continue;

        string fullPath = _strDirectory + "/" + itemName;

        if (tFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            TraverseDirectory(fullPath, _vecFilePath);
        else
        {
            if(SortFBX(fullPath))
                _vecFilePath.push_back(fullPath);
        }
    } 
    while (FindNextFileA(hFind, &tFindData) != 0);

    FindClose(hFind);
}

static _bool SortFBX(string& _strFilePath)
{
    const char* szFilePath = _strFilePath.c_str();
    
    _char szExt[MAX_PATH] = "";
    _splitpath_s(szFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);
    
    if (strcmp(szExt, ".fbx") == false)
        return true;

    return false;
}

static _bool CreateDirectories(const string& _strDirectory)
{
    size_t Pos{};
    string strPath;

    while ((Pos = _strDirectory.find_first_of("\\/", Pos)) != string::npos)
    {
        strPath = _strDirectory.substr(0, Pos++);

        if (!strPath.empty() && GetFileAttributesA(strPath.c_str()) == INVALID_FILE_ATTRIBUTES)
            if (!CreateDirectoryA(strPath.c_str(), NULL))
                if (GetLastError() != ERROR_ALREADY_EXISTS)
                {
                    cerr << "Failed to create directory: " << strPath << endl;
                    return false;
                }
    }

    if (!_strDirectory.empty() && GetFileAttributesA(_strDirectory.c_str()) == INVALID_FILE_ATTRIBUTES)
        if (!CreateDirectoryA(_strDirectory.c_str(), NULL))
            if (GetLastError() != ERROR_ALREADY_EXISTS)
            {
                cerr << "Failed to create directory: " << _strDirectory << endl;
                return false;
            }

    return true;
}

static void ValidateFileName(string& _strFileName)
{
    string strInvalid = "\\/:*?\"<>|";
    
    _bool bValid = _strFileName.find_first_of(strInvalid) == string::npos;

    if (!bValid)
        _strFileName.replace(_strFileName.find("|"), 1, "_");
}

static string ConvertToUTF8(const _char* _szFileName) 
{
    _tchar szTemp[MAX_PATH] = TEXT("");
    MultiByteToWideChar(CP_ACP, 0, _szFileName, static_cast<_int>(strlen(_szFileName)), szTemp, MAX_PATH);

    _int iSize = WideCharToMultiByte(CP_UTF8, 0, szTemp, -1, nullptr, 0, nullptr, nullptr);
    if (iSize <= 0)
        return "";

    string strTemp(iSize, 0);
    WideCharToMultiByte(CP_UTF8, 0, szTemp, -1, &strTemp[0], iSize, nullptr, nullptr);

    return strTemp;
}