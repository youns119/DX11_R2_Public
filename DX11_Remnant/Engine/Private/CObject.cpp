#include "CObject.h"

CObject::CObject()
{
}

_uint CObject::AddRef()
{
	return ++m_iRefCnt;
}

_uint CObject::Release()
{
	if (m_iRefCnt == 0)
	{
		Free();

		delete this;

		return 0;
	}
	else return m_iRefCnt--;
}

void CObject::Free()
{
}