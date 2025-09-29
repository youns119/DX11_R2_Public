#pragma once

#include "CVIBuffer.h"

#include "FVIBuffer_Instancing.h"
#include "FVertex_Instancing.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Instancing abstract 
	: public CVIBuffer
{
protected:
	CVIBuffer_Instancing(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CVIBuffer_Instancing(const CVIBuffer_Instancing& _Prototype);
	virtual ~CVIBuffer_Instancing() = default;

public:
	virtual HRESULT Init(const FVIBuffer_Instancing* _pDesc);
	virtual HRESULT Init(void* _pDesc) override;
	virtual HRESULT Render() override;

public :
	virtual HRESULT Bind_InputAssembler() override;

public:
	void Drop(_float _fTimeDelta);
	void Rise(_float _fTimeDelta);
	void Explosion(_float _fTimeDelta);

protected :
	D3D11_BUFFER_DESC m_tInstanceBufferDesc{};
	D3D11_SUBRESOURCE_DATA m_tInstanceInitialData{};

protected :
	_uint m_iInstanceVertexStride{};
	_uint m_iNumIndexPerInstance{};
	_uint m_iNumInstance{};

protected :
	VTXINSTANCING* m_pInstanceVertices{};
	ID3D11Buffer* m_pVBInstance{};

protected :
	_float* m_pSpeeds{};

	_bool m_bLoop{};
	_float3 m_vPivot{};

public :
	virtual CClonable* Clone(void* _pDesc) PURE;
	virtual void Free();
};

END