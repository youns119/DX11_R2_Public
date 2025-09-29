#include "CSceneComponent.h"

#include "CActor.h"

CSceneComponent::CSceneComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CActorComponent{ _pDevice, _pContext }
{
	m_strName = g_strSceneComponent;
}

CSceneComponent::CSceneComponent(const CSceneComponent& _Prototype)
	: CActorComponent(_Prototype)
	, m_matWorld{ _Prototype.m_matWorld }
	, m_matRelative{ _Prototype.m_matRelative }
{
	for (_uint i = 0; i < static_cast<_uint>(ETransform::TRANSFORM_END); i++)
		m_vRelativeTransform[i] = _Prototype.m_vRelativeTransform[i];
}

HRESULT CSceneComponent::Init()
{
	XMStoreFloat4x4(&m_matWorld, XMMatrixIdentity());
	XMStoreFloat4x4(&m_matRelative, XMMatrixIdentity());

	for (_uint i = 0; i < static_cast<_uint>(ETransform::TRANSFORM_END); i++)
		XMStoreFloat3(&m_vDeltaTransform[i], XMVectorSet(0.f, 0.f, 0.f, 0.f));

	FSceneComponent tSceneDesc{};

	m_vRelativeTransform[static_cast<_uint>(ETransform::TRANSFORM_SCALE)] = tSceneDesc.vScale;
	m_vRelativeTransform[static_cast<_uint>(ETransform::TRANSFORM_ROTATION)] = tSceneDesc.vRotation;
	m_vRelativeTransform[static_cast<_uint>(ETransform::TRANSFORM_POSITION)] = tSceneDesc.vPosition;

	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSceneComponent::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
	{
		FSceneComponent tDesc{};

		Update_Transform
		(
			XMLoadFloat3(&tDesc.vScale),
			XMLoadFloat3(&tDesc.vRotation),
			XMLoadFloat3(&tDesc.vPosition)
		);
	}
	else
	{
		FSceneComponent* pDesc = static_cast<FSceneComponent*>(_pDesc);

		m_vRelativeTransform[static_cast<_uint>(ETransform::TRANSFORM_SCALE)] = pDesc->vScale;
		m_vRelativeTransform[static_cast<_uint>(ETransform::TRANSFORM_ROTATION)] = pDesc->vRotation;
		m_vRelativeTransform[static_cast<_uint>(ETransform::TRANSFORM_POSITION)] = pDesc->vPosition;

		m_bInheritRotation = true;

		Update_Transform
		(
			XMLoadFloat3(&pDesc->vScale),
			XMLoadFloat3(&pDesc->vRotation),
			XMLoadFloat3(&pDesc->vPosition)
		);
	}

	if (FAILED(__super::Init(_pDesc)))
		return E_FAIL;

	return S_OK;
}

void CSceneComponent::Pre_Update(_float _fTimeDelta)
{
	Update_FinalTransform();
}

void CSceneComponent::Update(_float _fTimeDelta)
{
}

void CSceneComponent::Post_Update(_float _fTimeDelta)
{
}

void CSceneComponent::Update_FinalTransform()
{
	if (!m_bDirtyFlag && !m_pSocketMatrix)
		return;

	_vector vFinalScale =
		XMLoadFloat3(&m_vDeltaTransform[static_cast<_uint>(ETransform::TRANSFORM_SCALE)]) +
		XMLoadFloat3(&m_vRelativeTransform[static_cast<_uint>(ETransform::TRANSFORM_SCALE)]);
	_vector vFinalRotation =
		XMLoadFloat3(&m_vDeltaTransform[static_cast<_uint>(ETransform::TRANSFORM_ROTATION)]) +
		XMLoadFloat3(&m_vRelativeTransform[static_cast<_uint>(ETransform::TRANSFORM_ROTATION)]);
	_vector vFinalTranslation =
		XMLoadFloat3(&m_vDeltaTransform[static_cast<_uint>(ETransform::TRANSFORM_POSITION)]) +
		XMLoadFloat3(&m_vRelativeTransform[static_cast<_uint>(ETransform::TRANSFORM_POSITION)]);

	Reset_DeltaTransform();
	Update_Transform(vFinalScale, vFinalRotation, vFinalTranslation);
}

void CSceneComponent::Update_Transform(_fvector _vScale, _gvector _vRotation, _hvector _vPosition)
{
	Update_Scale(_vScale);
	Update_Rotation(_vRotation);
	Update_Position(_vPosition);

	if (m_pAttachComponent)
	{
		_matrix matSocket = XMMatrixIdentity();

		if (m_pSocketMatrix)
			matSocket = XMLoadFloat4x4(m_pSocketMatrix);

		for (size_t i = 0; i < 3; i++)
			matSocket.r[i] = XMVector3Normalize(matSocket.r[i]);

		if (m_bInheritRotation)
		{
			XMStoreFloat4x4
			(
				&m_matWorld,
				XMLoadFloat4x4(&m_matRelative) *
				matSocket *
				XMLoadFloat4x4(&m_pAttachComponent->m_matWorld)
			);
		}
		else
		{
			_vector vScale{}, vRotation{}, vTranslation{};
			XMMatrixDecompose(&vScale, &vRotation, &vTranslation, XMLoadFloat4x4(&m_pAttachComponent->m_matWorld));

			_matrix matParent = XMMatrixScalingFromVector(vScale) * XMMatrixTranslationFromVector(vTranslation);

			XMStoreFloat4x4
			(
				&m_matWorld,
				XMLoadFloat4x4(&m_matRelative) *
				matSocket * 
				matParent
			);
		}
	}
	else
		m_matWorld = m_matRelative;

	m_bDirtyFlag = false;
	for (auto& ChildComponent : m_listChildComponent)
		ChildComponent->m_bDirtyFlag = true;
}

void CSceneComponent::Update_Scale(_fvector _vScale)
{
	m_bDirtyFlag = true;

	_matrix matWorld = XMLoadFloat4x4(&m_matRelative);

	_vector vRight = XMLoadFloat4x4(&m_matRelative).r[static_cast<_uint>(ETransformState::TRANSFORMSTATE_RIGHT)];
	_vector vUp = XMLoadFloat4x4(&m_matRelative).r[static_cast<_uint>(ETransformState::TRANSFORMSTATE_UP)];
	_vector vLook = XMLoadFloat4x4(&m_matRelative).r[static_cast<_uint>(ETransformState::TRANSFORMSTATE_LOOK)];

	XMStoreFloat3(&m_vRelativeTransform[static_cast<_uint>(ETransform::TRANSFORM_SCALE)], _vScale);

	matWorld.r[static_cast<_uint>(ETransformState::TRANSFORMSTATE_RIGHT)] =
		XMVector3Normalize(vRight) * m_vRelativeTransform[static_cast<_uint>(ETransform::TRANSFORM_SCALE)].x;
	matWorld.r[static_cast<_uint>(ETransformState::TRANSFORMSTATE_UP)] =
		XMVector3Normalize(vUp) * m_vRelativeTransform[static_cast<_uint>(ETransform::TRANSFORM_SCALE)].y;
	matWorld.r[static_cast<_uint>(ETransformState::TRANSFORMSTATE_LOOK)] =
		XMVector3Normalize(vLook) * m_vRelativeTransform[static_cast<_uint>(ETransform::TRANSFORM_SCALE)].z;

	XMStoreFloat4x4(&m_matRelative, matWorld);
}

void CSceneComponent::Update_Rotation(_fvector _vRotation)
{
	m_bDirtyFlag = true;

	_matrix matWorld = XMLoadFloat4x4(&m_matRelative);
	_float3 vScale = Get_Transform(ETransform::TRANSFORM_SCALE);

	_vector vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScale.x;
	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScale.y;
	_vector vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScale.z;

	_float3 vRotation = AdjustRotation(_vRotation);
	m_vRelativeTransform[static_cast<_uint>(ETransform::TRANSFORM_ROTATION)] = vRotation;

	_vector vQuaternion = XMQuaternionRotationRollPitchYaw
	(
		XMConvertToRadians(m_vRelativeTransform[static_cast<_uint>(ETransform::TRANSFORM_ROTATION)].x),
		XMConvertToRadians(m_vRelativeTransform[static_cast<_uint>(ETransform::TRANSFORM_ROTATION)].y),
		XMConvertToRadians(m_vRelativeTransform[static_cast<_uint>(ETransform::TRANSFORM_ROTATION)].z)
	);
	_matrix matRotation = XMMatrixRotationQuaternion(vQuaternion);

	matWorld.r[static_cast<_uint>(ETransformState::TRANSFORMSTATE_RIGHT)] =
		XMVector3TransformNormal(vRight, matRotation);
	matWorld.r[static_cast<_uint>(ETransformState::TRANSFORMSTATE_UP)] =
		XMVector3TransformNormal(vUp, matRotation);
	matWorld.r[static_cast<_uint>(ETransformState::TRANSFORMSTATE_LOOK)] =
		XMVector3TransformNormal(vLook, matRotation);

	XMStoreFloat4x4(&m_matRelative, matWorld);
}

void CSceneComponent::Update_Position(_fvector _vPosition)
{
	m_bDirtyFlag = true;

	_matrix matWorld = XMLoadFloat4x4(&m_matRelative);

	_vector vPosition = XMVectorSetW(_vPosition, 1.f);
	XMStoreFloat3(&m_vRelativeTransform[static_cast<_uint>(ETransform::TRANSFORM_POSITION)], vPosition);

	matWorld.r[static_cast<_uint>(ETransformState::TRANSFORMSTATE_POS)] = vPosition;

	XMStoreFloat4x4(&m_matRelative, matWorld);
}

void CSceneComponent::LookAt(_fvector _vAt)
{
	m_bDirtyFlag = true;

	_matrix matWorld = XMLoadFloat4x4(&m_matRelative);
	_float3 vPos = Get_Transform(ETransform::TRANSFORM_POSITION);

	_vector	vLook = _vAt - XMVectorSetW(XMLoadFloat3(&vPos), 1.f);
	_vector	vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector	vUp = XMVector3Cross(vLook, vRight);

	matWorld.r[static_cast<_uint>(ETransformState::TRANSFORMSTATE_RIGHT)] =
		XMVector3Normalize(vRight) * m_vRelativeTransform[static_cast<_uint>(ETransform::TRANSFORM_SCALE)].x;
	matWorld.r[static_cast<_uint>(ETransformState::TRANSFORMSTATE_UP)] =
		XMVector3Normalize(vUp) * m_vRelativeTransform[static_cast<_uint>(ETransform::TRANSFORM_SCALE)].y;
	matWorld.r[static_cast<_uint>(ETransformState::TRANSFORMSTATE_LOOK)] =
		XMVector3Normalize(vLook) * m_vRelativeTransform[static_cast<_uint>(ETransform::TRANSFORM_SCALE)].z;

	XMStoreFloat4x4(&m_matRelative, matWorld);
}

void CSceneComponent::AttachToComponent(CSceneComponent* _pAttachComponent)
{
	m_pAttachComponent = _pAttachComponent;
	m_pAttachComponent->m_listChildComponent.push_back(this);
	Safe_AddRef(m_pAttachComponent);
	Safe_AddRef(m_pAttachComponent->m_listChildComponent.back());

	_uint iCurrID = m_iComponentID;
	_uint iAttachID = _pAttachComponent->Get_ComponentID();

	m_pOwner->Get_UpdateGraph()->Add_Edge(iAttachID, iCurrID);
	m_pOwner->Update_Graph();
}

void CSceneComponent::AttachToComponent(CSceneComponent* _pAttachComponent, const _float4x4* _pSocketMatrix)
{
	m_pAttachComponent = _pAttachComponent;
	m_pAttachComponent->m_listChildComponent.push_back(this);
	Safe_AddRef(m_pAttachComponent);
	Safe_AddRef(m_pAttachComponent->m_listChildComponent.back());

	m_pSocketMatrix = _pSocketMatrix;
}

void CSceneComponent::Clear()
{
	for (auto& ChildComponent : m_listChildComponent)
		Safe_Release(ChildComponent);

	m_listChildComponent.clear();

	Safe_Release(m_pAttachComponent);

	__super::Clear();
}

_float3 CSceneComponent::QuaternionToEuler(_fvector _vQuaternion)
{
	_float fX = XMVectorGetX(_vQuaternion);
	_float fY = XMVectorGetY(_vQuaternion);
	_float fZ = XMVectorGetZ(_vQuaternion);
	_float fW = XMVectorGetW(_vQuaternion);

	_float fSinp = 2.0f * (fW * fX - fY * fZ);
	_float fPitch;
	if (fabs(fSinp) >= 1.0f) fPitch = (fSinp > 0 ? XM_PIDIV2 : -XM_PIDIV2);
	else fPitch = asin(fSinp);

	_float fYaw = atan2(2.0f * (fY * fW + fX * fZ), 1.0f - 2.0f * (fX * fX + fY * fY));
	_float fRoll = atan2(2.0f * (fZ * fW + fX * fY), 1.0f - 2.0f * (fX * fX + fZ * fZ));

	return _float3(fPitch, fYaw, fRoll);
}

_float3 CSceneComponent::AdjustRotation(_fvector _vRotation)
{
	_float3 vRotation{};

	XMStoreFloat3(&vRotation, _vRotation);

	vRotation.x = fmod(vRotation.x + 180.f, 360.f);
	if (vRotation.x < 0)
		vRotation.x += 360.f;

	vRotation.y = fmod(vRotation.y + 180.f, 360.f);
	if (vRotation.y < 0)
		vRotation.y += 360.f;

	vRotation.z = fmod(vRotation.z + 180.f, 360.f);
	if (vRotation.z < 0)
		vRotation.z += 360.f;

	vRotation.x -= 180.f;
	vRotation.y -= 180.f;
	vRotation.z -= 180.f;

	return vRotation;
}

void CSceneComponent::Reset_DeltaTransform()
{
	for (_uint i = 0; i < static_cast<_uint>(ETransform::TRANSFORM_END); i++)
		m_vDeltaTransform[i] = _float3(0.f, 0.f, 0.f);
}

CSceneComponent* CSceneComponent::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CSceneComponent* pInstance = new CSceneComponent(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Create : CSceneComponent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CSceneComponent::Clone(void* _pDesc)
{
	CSceneComponent* pInstance = new CSceneComponent(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Create : CSceneComponent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSceneComponent::Free()
{
	__super::Free();
}