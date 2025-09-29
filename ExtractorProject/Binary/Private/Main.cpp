#include "Main.h"

#include "Model.h"

int main()
{
    SetConsoleTitle(L"FBX Extractor");

    string strSrcPath{}, strDstPath{};

    cout << "Extracting Src Path : ";
    getline(cin, strSrcPath);

    cout << "\nExtracting Dst Path : ";
    getline(cin, strDstPath);

    cout << "\n\n";

    vector<string> vecMeshPath;

    TraverseDirectory(strSrcPath, vecMeshPath);

    for (const auto& SkeletalPath : vecMeshPath)
    {
        const _char* szSkeletalPath = SkeletalPath.c_str();

        CModel* pModel = CModel::Create(szSkeletalPath, strDstPath.c_str());
        Safe_Delete(pModel);
    }

	return 0;
}