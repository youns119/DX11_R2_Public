// Ŀ�ǵ� ��ɾ�				�ɼ�				���� ������ �ִ� ��ġ			�纻 ������ ������ ��ġ

xcopy			/y/s			.\Engine\Public\*.*						.\Reference\Inc\
xcopy			/y				.\Engine\Bin\Engine.lib					.\Reference\Lib\

xcopy			/y				.\Engine\Bin\Engine.dll					.\Client\Bin\

xcopy			/d /e /y		.\Reference\DLL							.\Client\Bin\