#pragma once

#include "CVIBuffer.h"

#include "FVertex_Cube.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Cube final 
	: public CVIBuffer
{
private :
	CVIBuffer_Cube(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CVIBuffer_Cube(const CVIBuffer_Cube& _Prototype);
	virtual ~CVIBuffer_Cube() = default;

public :
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;

public :
	static CVIBuffer_Cube* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END