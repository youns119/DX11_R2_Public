#pragma once

#include "CVIBuffer.h"

#include "FVertex_Pos.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Cell final
	: public CVIBuffer
{
private :
	CVIBuffer_Cell(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CVIBuffer_Cell(const CVIBuffer_Cell& _Prototype);
	virtual ~CVIBuffer_Cell() = default;

public :
	virtual HRESULT Init(const _float3* _pPoints);
	virtual HRESULT Init(void* _pDesc) override;

public :
	static CVIBuffer_Cell* Create
	(
		ID3D11Device* _pDevice, 
		ID3D11DeviceContext* _pContext, 
		const _float3* _pPoints
	);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END