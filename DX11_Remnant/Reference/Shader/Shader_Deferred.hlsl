#include "Shader_Defines.hlsli"

float4x4 g_matWorld, g_matView, g_matProj;
float4x4 g_matViewInv, g_matProjInv;

Texture2D g_Texture;

float4 g_vLightDir;
float4 g_vLightPos;
float g_fRange;

float4 g_vLightDiffuse;
float4 g_vLightAmbient;
float4 g_vLightSpecular;

Texture2D g_DiffuseTexture;
float4 g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
float4 g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

Texture2D g_SpecularTexture;
Texture2D g_NormalTexture;
Texture2D g_DepthTexture;
Texture2D g_ShadeTexture;

float4 g_vCamPos;

struct VS_IN
{
	float3 vPosition : POSITION;
	float2 vTexCoord : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
	float2 vTexCoord : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT) 0;

	matrix matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexCoord = In.vTexCoord;

	return Out;
}

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float2 vTexCoord : TEXCOORD0;
};

struct PS_OUT
{
	float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN_DEBUG(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexCoord);

	return Out;
}

struct PS_OUT_LIGHT
{
	float4 vShade : SV_TARGET0;
	float4 vSpecular : SV_TARGET1;
};

PS_OUT_LIGHT PS_MAIN_DIRECTIONAL(PS_IN In)
{
	PS_OUT_LIGHT Out = (PS_OUT_LIGHT) 0;

	vector vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexCoord);
	vector vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

    vector vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexCoord);
	float fViewZ = vDepthDesc.y;
	
	vector vShade = 
	saturate(max(dot(normalize(g_vLightDir * -1.f), normalize(vNormal)), 0.f) + (g_vLightAmbient * g_vMtrlAmbient));

	Out.vShade = g_vLightDiffuse * vShade;

	vector vWorldPos;

	/* 투영공간상의 위치 */
	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 * 1/w */
	vWorldPos.x = In.vTexCoord.x * 2.f - 1.f;
	vWorldPos.y = In.vTexCoord.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.x;
	vWorldPos.w = 1.f;

	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬  */
	vWorldPos = vWorldPos.xyzw * fViewZ;

	/* 로컬위치 * 월드행렬 * 뷰행렬 */
	vWorldPos = mul(vWorldPos, g_matProjInv);

	/* 월드위치 */
	/* 로컬위치 * 월드행렬  */
	vWorldPos = mul(vWorldPos, g_matViewInv);	

	vector vReflect = reflect(normalize(g_vLightDir), normalize(vNormal));
	vector vLook = vWorldPos - g_vCamPos;

	Out.vSpecular = 
	(g_vLightSpecular * g_vMtrlSpecular) * 
	pow(max(dot(normalize(vReflect * -1.f), normalize(vLook)), 0.f), 500.f);

	return Out;
}

PS_OUT_LIGHT PS_MAIN_POINT(PS_IN In)
{
	PS_OUT_LIGHT Out = (PS_OUT_LIGHT) 0;

	vector vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexCoord);
	vector vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

    vector vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexCoord);
	float fViewZ = vDepthDesc.y;

	vector vWorldPos;

    vWorldPos.x = In.vTexCoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexCoord.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.x;
	vWorldPos.w = 1.f;

	vWorldPos = vWorldPos.xyzw * fViewZ;
	vWorldPos = mul(vWorldPos, g_matProjInv);
	vWorldPos = mul(vWorldPos, g_matViewInv);

	vector vLightDir = vWorldPos - g_vLightPos;
	float fLength = length(vLightDir);

	float fAtt = saturate((g_fRange - fLength) / g_fRange);

	vector vShade = 
	saturate(max(dot(normalize(vLightDir * -1.f), normalize(vNormal)), 0.f) + (g_vLightAmbient * g_vMtrlAmbient));

	Out.vShade = g_vLightDiffuse * vShade * fAtt;

	vector vReflect = reflect(normalize(vLightDir), normalize(vNormal));
	vector vLook = vWorldPos - g_vCamPos;

	Out.vSpecular = 
	(g_vLightSpecular * g_vMtrlSpecular) *
	pow(max(dot(normalize(vReflect * -1.f), normalize(vLook)), 0.f), 500.f) * fAtt;
	
	return Out;
}

PS_OUT PS_MAIN_DEFERRED(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;

	vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexCoord);
	
	if (vDiffuse.a == 0.f)
		discard;

    vector vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexCoord);
    vector vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexCoord);
	
	Out.vColor = vDiffuse * vShade + vSpecular;
	
	return Out;
}

technique11 DefaultTechnique
{
	pass Debug
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEBUG();
	}

	pass Directional
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Skip_Z, 0);
		SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DIRECTIONAL();
	}

	pass Point
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Skip_Z, 0);
		SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_POINT();
	}

	pass Deferred
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Skip_Z, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFERRED();
	}
}
