#pragma once

#include "CVIBuffer.h"

#include "FVertex_PosTex.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect final 
	: public CVIBuffer
{
private :
	CVIBuffer_Rect(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CVIBuffer_Rect(const CVIBuffer_Rect& _Prototype);
	virtual ~CVIBuffer_Rect() = default;

public :
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;

public :
	static CVIBuffer_Rect* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CClonable* Clone(void* _pDesc);
	virtual void Free() override;
};

END