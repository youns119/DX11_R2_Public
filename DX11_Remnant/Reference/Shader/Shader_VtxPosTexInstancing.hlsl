#include "Shader_Defines.hlsli"

float4x4 g_matWorld, g_matView, g_matProj;
Texture2D g_Texture;

struct VS_IN
{
	float3 vPosition : POSITION;	
	float2 vTexCoord : TEXCOORD0;	

	float4 vRight : TEXCOORD1;
	float4 vUp : TEXCOORD2;
	float4 vLook : TEXCOORD3;
	float4 vTranslation : TEXCOORD4;
	float2 vLifeTime : TEXCOORD5;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
    float2 vTexCoord : TEXCOORD0;
	float4 vWorldPos : TEXCOORD1;
	float2 vLifeTime : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{	
	VS_OUT Out = (VS_OUT) 0;

	float4x4 matTransform = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector vPosition = mul(vector(In.vPosition, 1.f), matTransform);

	matrix matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vPosition, matWVP);
    Out.vTexCoord = In.vTexCoord;
	Out.vWorldPos = mul(vPosition, g_matWorld);
	Out.vLifeTime = In.vLifeTime;

	return Out;
}

struct PS_IN
{
	float4 vPosition : SV_POSITION;
    float2 vTexCoord : TEXCOORD0;
	float4 vWorldPos : TEXCOORD1;
	float2 vLifeTime : TEXCOORD2;
};

struct PS_OUT
{
	float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexCoord);

	if (In.vLifeTime.y >= In.vLifeTime.x)
		discard;

	return Out;
}

technique11 DefaultTechnique
{
	pass DefaultPass
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
}
