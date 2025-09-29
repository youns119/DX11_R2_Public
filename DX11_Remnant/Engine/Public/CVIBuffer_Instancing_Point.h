#pragma once

#include "CVIBuffer_Instancing.h"

#include "FVertex_Instancing_Point.h"
#include "FVIBuffer_Instancing_Point.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Instancing_Point final 
	: public CVIBuffer_Instancing
{
private :
	CVIBuffer_Instancing_Point(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CVIBuffer_Instancing_Point(const CVIBuffer_Instancing_Point& _Prototype);
	virtual ~CVIBuffer_Instancing_Point() = default;

public :
	virtual HRESULT Init(const FVIBuffer_Instancing* _pDesc) override;
	virtual HRESULT Init(void* _pDesc) override;

public :
	static CVIBuffer_Instancing_Point* Create
	(
		ID3D11Device* _pDevice, 
		ID3D11DeviceContext* _pContext, 
		const FVIBuffer_Instancing* _pDesc
	);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END