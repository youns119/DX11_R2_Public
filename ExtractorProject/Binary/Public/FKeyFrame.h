#pragma once

struct FKeyFrame
{
	XMFLOAT3 vScale{};
	XMFLOAT4 vRotation{};
	XMFLOAT3 vTranslation{};

	float fTrackPosition{};
};