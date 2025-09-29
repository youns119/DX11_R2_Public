#include "Shader_Defines.hlsli"

float4x4 g_matWorld, g_matView, g_matProj;
Texture2D g_Texture;
float4 g_vCamPos;

struct VS_IN
{
	float3 vPosition : POSITION;	
	float fPSize : PSIZE;	

	row_major float4x4 matTransform : WORLD;	
	float2 vLifeTime : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPosition : POSITION;
	float fPSize : PSIZE;
	float2 vLifeTime : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{	
	VS_OUT Out = (VS_OUT) 0;
	
    vector vPosition = mul(vector(In.vPosition, 1.f), In.matTransform);

	Out.vPosition = mul(vPosition, g_matWorld);
    Out.fPSize = length(In.matTransform._11_12_13);
	Out.vLifeTime = In.vLifeTime;

	return Out;
}

struct GS_IN
{
	float4 vPosition : POSITION;
	float fPSize : PSIZE;
	float2 vLifeTime : TEXCOORD0;
};

struct GS_OUT
{
	float4 vPosition : SV_POSITION;	
	float2 vTexCoord : TEXCOORD0;
	float2 vLifeTime : TEXCOORD1;
};

[maxvertexcount(6)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> DataStream)
{
	GS_OUT Out[4];

	float3 vLook = g_vCamPos.xyz - In[0].vPosition.xyz;
	float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook)) * (In[0].fPSize * 0.5f);
	float3 vUp = normalize(cross(vLook, vRight)) * (In[0].fPSize * 0.5f);

	float4x4 matVP = mul(g_matView, g_matProj);

	Out[0].vPosition = float4(In[0].vPosition.xyz + vRight + vUp, 1.f);
	Out[0].vPosition = mul(Out[0].vPosition, matVP);
	Out[0].vTexCoord = float2(0.f, 0.f);
	Out[0].vLifeTime = In[0].vLifeTime;

	Out[1].vPosition = float4(In[0].vPosition.xyz - vRight + vUp, 1.f);
	Out[1].vPosition = mul(Out[1].vPosition, matVP);
    Out[1].vTexCoord = float2(1.f, 0.f);
	Out[1].vLifeTime = In[0].vLifeTime;

	Out[2].vPosition = float4(In[0].vPosition.xyz - vRight - vUp, 1.f);
	Out[2].vPosition = mul(Out[2].vPosition, matVP);
    Out[2].vTexCoord = float2(1.f, 1.f);
	Out[2].vLifeTime = In[0].vLifeTime;

	Out[3].vPosition = float4(In[0].vPosition.xyz + vRight - vUp, 1.f);
	Out[3].vPosition = mul(Out[3].vPosition, matVP);
    Out[3].vTexCoord = float2(0.f, 1.f);
	Out[3].vLifeTime = In[0].vLifeTime;

	DataStream.Append(Out[0]);
	DataStream.Append(Out[1]);
	DataStream.Append(Out[2]);
	DataStream.RestartStrip();

	DataStream.Append(Out[0]);
	DataStream.Append(Out[2]);
	DataStream.Append(Out[3]);
	DataStream.RestartStrip();
}

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float2 vTexCoord : TEXCOORD0;
	float2 vLifeTime : TEXCOORD1;
};

struct PS_OUT
{
	float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexCoord);	 

	Out.vColor = float4((In.vLifeTime.x - In.vLifeTime.y) * 3.f, 0.f, 0.f, 1.f);

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
		GeometryShader = compile gs_5_0 GS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}
}
