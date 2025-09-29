#pragma once

#include "CVIBuffer_Instancing.h"

#include "FVertex_PosTex.h"
#include "FVertex_Instancing_Rect.h"
#include "FVIBuffer_Instancing_Rect.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Instancing_Rect final
	: public CVIBuffer_Instancing
{
private :
	CVIBuffer_Instancing_Rect(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CVIBuffer_Instancing_Rect(const CVIBuffer_Instancing_Rect& _Prototype);
	virtual ~CVIBuffer_Instancing_Rect() = default;

public :
	virtual HRESULT Init(const FVIBuffer_Instancing* _pDesc) override;
	virtual HRESULT Init(void* _pDesc) override;

public :
	static CVIBuffer_Instancing_Rect* Create
	(
		ID3D11Device* _pDevice, 
		ID3D11DeviceContext* _pContext, 
		const FVIBuffer_Instancing* _pDesc
	);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END