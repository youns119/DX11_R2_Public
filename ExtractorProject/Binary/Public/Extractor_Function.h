#pragma once

template<typename T>
void Safe_Delete(T& _Pointer)
{
	if (_Pointer != nullptr)
	{
		_Pointer->Free();

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