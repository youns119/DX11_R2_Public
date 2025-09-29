#include "Shader_Defines.hlsli"

float4x4 g_matWorld, g_matView, g_matProj;
textureCUBE	g_Texture;

struct VS_IN
{
	float3 vPosition : POSITION;	
    float3 vTexCoord : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
    float3 vTexCoord : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{	
	VS_OUT Out = (VS_OUT) 0;

	Out.vPosition = mul(float4(In.vPosition, 1.f), g_matWorld);
	Out.vPosition = mul(Out.vPosition, g_matView);
	Out.vPosition = mul(Out.vPosition, g_matProj);	
    Out.vTexCoord = In.vTexCoord;

	return Out;
}

struct PS_IN
{
	float4 vPosition : SV_POSITION;
    float3 vTexCoord : TEXCOORD0;
};

struct PS_OUT
{
	float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexCoord);
	
	return Out;
}

technique11 DefaultTechnique
{
	pass DefaultPass
	{
		SetRasterizerState(RS_Cull_CW);
		SetDepthStencilState(DSS_Skip_Z, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
}
