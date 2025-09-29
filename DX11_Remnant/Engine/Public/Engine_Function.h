#ifndef Engine_Function_h__
#define Engine_Function_h__

namespace Engine
{
	template<typename T>
	void Safe_Delete(T& _Pointer)
	{
		if (_Pointer != nullptr)
		{
			delete _Pointer;
			_Pointer = nullptr;
		}
	}

	template<typename T>
	void Safe_Delete_Array(T& _Pointer)
	{
		if (_Pointer != nullptr)
		{
			delete[] _Pointer;
			_Pointer = nullptr;
		}
	}

	template<typename T>
	unsigned int Safe_Release(T& _Instance)
	{
		unsigned int dwRefCnt = 0;

		if (_Instance != nullptr)
		{
			dwRefCnt = _Instance->Release();

			if (dwRefCnt == 0)
				_Instance = nullptr;
		}

		return dwRefCnt;
	}

	template<typename T>
	unsigned int Safe_AddRef(T& _Instance)
	{
		unsigned int dwRefCnt = 0;

		if (_Instance != nullptr)
			dwRefCnt = _Instance->AddRef();

		return dwRefCnt;
	}
}

#endif // Engine_Function_h__